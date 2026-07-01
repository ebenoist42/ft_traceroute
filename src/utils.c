/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:28:41 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/01 14:12:09 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"


int test_ac(int ac, char **av)
{
	if(ac >= 4)
	{
		printf("Extra arg `%s' (position 3, argc 3)\n", av[3]);
		return(1);
	}
	return(0);
}

void free_exit(char *str, t_data *data)
{
	if(str)
		perror(str);
	if(data->res)
		freeaddrinfo(data->res);
}