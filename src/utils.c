/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:28:41 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/08 15:09:15 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

static void	ft_help(){
	printf(
		"Usage:\n"
		"  ft_traceroute [ --help ] host\n"
		"Options:\n"
		"  --help                      Read this help and exit\n"
		"Arguments:\n"
		"+     host          The host to traceroute to\n");
}

int test_ac(int ac, char **av)
{
	for(int i = 1; i < ac; i++){
		if(ft_strcmp(av[i], "--help") == 0){
			ft_help();
			exit(0);}
		else if(ft_strcmp(av[i], "-m") == 0)
				return(0);}

	return(0);
}

void free_exit(char *str, t_data *data, int sortie)
{
	if(str)
		perror(str);
	if(data->res)
		freeaddrinfo(data->res);
	if(data->icmp_sockfd)
		close (data->icmp_sockfd);
	if(data->sockfd)
		close(data->sockfd);
	exit(sortie);
}