/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 11:12:10 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/06 12:08:35 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"


static struct sockaddr_in *	init_data(t_data *data, int ac, char **av){
	(void) ac;

// traduction dns, stocker dans la struct res, hints sert a mettre la condition IPV4
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	data->dns = getaddrinfo(av[1], NULL, &hints, &data->res);
	if(data->dns != 0){
		printf("%s: Temporary failure in name resolution\nCannot handle ""host"" cmdline arg `%s' on position 1 (argc 1)\n", av[1], av[1]);
		free_exit(NULL, data, 2);
	}

// init socket
	data->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(data->sockfd < 0){
		free_exit("socket", data, 1);
	}

// cast de res (sockaddr qui n'expose pas le sin_port)
	struct sockaddr_in *addr = (struct sockaddr_in *)data->res->ai_addr;
// 33434 port inutilisable, pour pouvoir recv le msg d'erreur d'arriver a destination.
//
	addr->sin_port = htons(33434 + data->compteur_de_sonde);
	return addr;
}
/*
static void ft_send(t_data *data){
	
}*/

static	void ft_traceroute(t_data *data){

	for(int ttl = 0; ttl <= 30; ttl++){
// definition ttl = 1, sinon ttl = par default systeme (64)
		setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
		//ft_send(&data);
		//att reponse
		//int ret = recvfrom();
		//if(ret == timeout)
		//	printf("* ");
		//afficher routeur ou "*"

	}
}

int main (int ac, char **av){
	if((test_ac(ac, av))> 0)
		return(1);

	t_data data;
	memset(&data, 0,sizeof(data));
	struct sockaddr_in *addr = init_data(&data, ac, av);
	(void) addr;
	ft_traceroute(&data);
	free_exit(NULL, &data, 0);


}