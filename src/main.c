/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 11:12:10 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/08 14:25:28 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

static void send_recv(t_data *data, struct sockaddr_in *addr)
{
	struct timeval start;
	struct timeval end;
	struct timeval timeout;
	double rtt;
	char last_ip[15] = {0};
	char host[NI_MAXHOST] = {0};
	ssize_t recive;

	char sendbuf[32] = {0}; // envoie paquet de 32 oct, + 20 d'en tete ip + 8 en tete udp = 60 comme traceroute off
	char recvbuf[1500];
	struct sockaddr_in recv;
	socklen_t len = sizeof(recv);
	// pour les 3 envoie par ttl
	for (int sonde = 0; sonde < 3; sonde++)
	{
		addr->sin_port = htons(33434 + data->compteur_de_sonde);
		data->compteur_de_sonde++;

		gettimeofday(&start, NULL);
		// send
		ssize_t send = sendto(data->sockfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *)addr, sizeof(*addr));
		if (send < 0)
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
			&timeout);
		// select si + 1s / timeout
		if (ret == 0)
		{
			printf("* ");
			continue;
		}
		// erreur systeme
		else if (ret < 0)
			free_exit("select", data, 1);
		else
			recive = recvfrom(data->icmp_sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&recv, &len);
		gettimeofday(&end, NULL);
		if (recive < 0)
			free_exit("recvfrom", data, 1);
		else
		{// impression < ip/nom de domaine (ip brute) >
			getnameinfo((struct sockaddr *)&recv, sizeof(recv), host, sizeof(host), NULL, 0, 0);
			if (sonde == 0 || ft_strcmp(last_ip, inet_ntoa(recv.sin_addr)) != 0)
			{
				printf("%s (%s) ", host, inet_ntoa(recv.sin_addr));
				ft_strncpy(last_ip, inet_ntoa(recv.sin_addr), sizeof(last_ip));
			}
			rtt = (end.tv_sec - start.tv_sec) * 1000.0;
			rtt += (end.tv_usec - start.tv_usec) / 1000.0;
			printf("%.3f ms ", rtt);
		}
		// naviguer dans le paquet recu, check commencer apres l'en tete IP
		int check = (recvbuf[0] & 0x0f) * 4;
		unsigned char type = recvbuf[check];
		unsigned char code = recvbuf[check + 1];
		if (type == 3 && code == 3)
			data->arrived = 1;
	}
	printf("\n");
}

static void ft_traceroute(t_data *data, struct sockaddr_in *addr, char *str)
{

	printf("traceroute to %s (%s), 30 hops max, 60 byte packets\n", str, inet_ntoa(addr->sin_addr));

	for (int ttl = 1; ttl <= data->ttl ; ttl++)
	{
		// definition ttl = 1, sinon ttl = par default systeme (64)
		if (setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
			free_exit("setsockopt", data, 1);
		printf("%2d ", ttl);
		send_recv(data, addr);
		if (data->arrived)
			break;
	}
}

int main(int ac, char **av)
{
	if ((test_ac(ac, av)) > 0)
		return (1);

	t_data data;
	ft_memset(&data, 0, sizeof(data));
	struct sockaddr_in *addr = init_data(&data, ac, av);
	ft_traceroute(&data, addr, av[1]);
	free_exit(NULL, &data, 0);
}
