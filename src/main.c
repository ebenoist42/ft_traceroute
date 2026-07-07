/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 11:12:10 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/07 15:05:39 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"


static struct sockaddr_in *	init_data(t_data *data, int ac, char **av){
	(void) ac;
	data->arrived = 0;

// traduction dns, stocker dans la struct res, hints sert a mettre la condition IPV4
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	data->dns = getaddrinfo(av[1], NULL, &hints, &data->res);
	if(data->dns != 0){
		printf("%s: Temporary failure in name resolution\nCannot handle ""host"" cmdline arg `%s' on position 1 (argc 1)\n", av[1], av[1]);
		free_exit(NULL, data, 2);
	}

// init socket udp pour envoie
	data->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(data->sockfd < 0)
		free_exit("socket udp", data, 1);
	
// socket pour reception obligatoirement icmp 
	data->icmp_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);   // ← ajout
	if(data->icmp_sockfd < 0)
		free_exit("socket icmp", data, 1);

// cast de res (sockaddr qui n'expose pas le sin_port)
	struct sockaddr_in *addr = (struct sockaddr_in *)data->res->ai_addr;
	
// 33434 port inutilisable, pour pouvoir recv le msg d'erreur d'arriver a destination.
	data->compteur_de_sonde = 1;
	addr->sin_port = htons(33434 + data->compteur_de_sonde);
	return addr;
}

static void send_recv(t_data *data, struct sockaddr_in *addr){
	struct timeval start;
	struct timeval end;
	struct timeval timeout;
	double rtt;
	ssize_t recive;

	char sendbuf[32] = {0}; // envoie paquet de 32 oct, + 20 d'en tete ip + 8 en tete udp = 60 comme traceroute off
	char recvbuf[1500];
	struct sockaddr_in recv;
	socklen_t len = sizeof(recv);
// pour les 3 envoie par ttl
	for(int sonde = 0; sonde < 3; sonde++){
		addr->sin_port = htons(33434 + data->compteur_de_sonde);
		data->compteur_de_sonde++;
		
		gettimeofday(&start, NULL);
//send
		ssize_t send = sendto(data->sockfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *) addr, sizeof(*addr));
		if(send < 0)
			free_exit("sendto", data, 1);
// select
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(data->icmp_sockfd, &readfds);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int ret = select(
			data->icmp_sockfd + 1,
			&readfds,
			NULL,
			NULL,
			&timeout
		);
// select si + 1s / timeout
		if(ret == 0){
			printf("* ");
			continue;}
// erreur systeme
		else if(ret < 0)
			free_exit("select", data, 1);
		else
			recive = recvfrom(data->icmp_sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&recv, &len);
		gettimeofday(&end, NULL);
		if(recive < 0 )
				free_exit("recvfrom", data, 1);
		else{
			if (sonde == 0)
				printf("%s ", inet_ntoa(recv.sin_addr));
			rtt = (end.tv_sec - start.tv_sec) * 1000.0;
			rtt += (end.tv_usec - start.tv_usec) / 1000.0;
			printf("%.3f ms ", rtt);}
// naviguer dans le paquet recu, check commencer apres l'en tet IP
			int check = (recvbuf[0] & 0x0f) * 4;
			unsigned char type = recvbuf[check];
			unsigned char code = recvbuf[check + 1];
			if (type == 3 && code == 3)
				data->arrived = 1;
	}
	printf("\n");
}

static	void ft_traceroute(t_data *data, struct sockaddr_in *addr, char *str){
	printf("traceroute to %s (%s), 30 hops max, 60 byte packets\n", str , inet_ntoa(addr->sin_addr));
	for(int ttl = 1; ttl <= 30; ttl++){
// definition ttl = 1, sinon ttl = par default systeme (64)
		if (setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
			free_exit("setsockopt", data, 1);
		printf("%d ", ttl);
		send_recv(data, addr);
		if (data->arrived)
			break;

	}
}

int main (int ac, char **av){
	if((test_ac(ac, av)) > 0)
		return(1);

	t_data data;
	memset(&data, 0,sizeof(data));
	struct sockaddr_in *addr = init_data(&data, ac, av);
	ft_traceroute(&data, addr, av[1]);
	free_exit(NULL, &data, 0);


}