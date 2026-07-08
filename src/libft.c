/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 12:36:08 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/08 14:32:01 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"


int	ft_str_is_digit(const char *str)
{
	if (!str || !*str)       
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int	nb;
	int	value;
	int	i;

	nb = 0;
	value = 1;
	i = 0;
	while ((str[i] > 8 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			value *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (value *= nb);
}

void	*ft_memset(void *pointer, int value, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		((unsigned char *)pointer)[i] = (unsigned char)value;
		i++;
	}
	return (pointer);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i = 0;

	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n){
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i = 0;

	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}