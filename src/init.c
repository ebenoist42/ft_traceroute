/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:48:45 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/08 15:11:57 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

static int check_m(int ac, char **av,int i, t_data *data)
{
	
	if((i + 1) == ac){
		printf("Option `-m' (argc 2) requires an argument: `-m max_ttl'\n");
		return(1);}
	else if(ft_str_is_digit(av[i + 1]) == 0 ){
		printf("Cannot handle `-m' option with arg `%s' (argc %d)\n", av[i + 1], (i+1));
		return(1);
	}
	else if(ft_atoi(av[i + 1]) == 0){
		printf("first hop out of range\n");
		return(1);}
	else if(ac == 5)
	{
		printf("Extra arg `%s' (position 3, argc 3)\n", av[3]);
		return(1);
	}
	else if (ac > 5)
	{
		printf("Extra arg `%s' (position 3, argc 3)\n", av[3]);
		return(1);
	}
	else
		data->ttl = ft_atoi(av[i + 1]);
	//printf("TTL = %d\n", data->ttl);
	//printf("AV[i] = %s\n", av[i]);
	return(0);
}

static int check_flags(t_data *data, int ac, char **av){
    data->ttl = 30;
    for(int i = 1; i < ac; i++)
	{
		if(ft_strcmp(av[i], "-m") == 0)
		{
			if(check_m(ac, av, i , data) == 0)
				return(0);
			}
	}
	return(1);
}

struct sockaddr_in *init_data(t_data *data, int ac, char **av)
{
	if(check_flags(data, ac, av))
		free_exit(NULL, data, 1);
	data->arrived = 0;

	// traduction dns, stocker dans la struct res, hints sert a mettre la condition IPV4
	struct addrinfo hints;
	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	data->dns = getaddrinfo(av[1], NULL, &hints, &data->res);
	if (data->dns != 0)
	{
		printf("%s: Temporary failure in name resolution\nCannot handle "
			   "host"
			   " cmdline arg `%s' on position 1 (argc 1)\n",
			   av[1], av[1]);
		free_exit(NULL, data, 2);
	}

	// init socket udp pour envoie
	data->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (data->sockfd < 0)
		free_exit("socket udp", data, 1);

	// socket pour reception obligatoirement icmp
	data->icmp_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (data->icmp_sockfd < 0)
		free_exit("socket icmp", data, 1);

	// cast de res (sockaddr qui n'expose pas le sin_port)
	struct sockaddr_in *addr = (struct sockaddr_in *)data->res->ai_addr;

	// 33434 port inutilisable, pour pouvoir recv le msg d'erreur d'arriver a destination
	data->compteur_de_sonde = 1;
	addr->sin_port = htons(33434 + data->compteur_de_sonde);
	return addr;
}