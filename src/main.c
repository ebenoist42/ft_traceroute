/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 11:12:10 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/01 14:33:04 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"


static void	init_data(t_data *data, int ac, char **av){
	(void) ac;

// traduction dns, stocker dans la struct res, hints sert a mettre la condition IPV4
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	data->dns = getaddrinfo(av[1], NULL, &hints, &data->res);
	if(data->dns != 0){
		printf("%s: Temporary failure in name resolution\nCannot handle ""host"" cmdline arg `%s' on position 1 (argc 1)\n", av[1], av[1]);
		free_exit(NULL, data);
	}

// init socket
	data->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(data->sockfd < 0){
		free_exit("socket", data);
	}

}

int main (int ac, char **av){
	if((test_ac(ac, av))> 0)
		return(1);

	t_data data;
	memset(&data, 0,sizeof(data));
	init_data(&data, ac, av);

	free_exit(NULL, &data);
	return(0);

}