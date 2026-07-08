#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/select.h>

typedef struct s_data{

	struct addrinfo *res;
	
	int ttl;
	char *arg;
	int destination;
	int dns;
	int icmp_sockfd;
	int sockfd;
	int compteur_de_sonde;
	int arrived;
}	t_data;

int test_ac(int ac, char **av);
int	ft_atoi(const char *str);
int	ft_str_is_digit(const char *str);
struct sockaddr_in *init_data(t_data *data, int ac, char **av);
void free_exit(char *str, t_data *data, int sortie);
int	ft_strcmp(const char *s1, const char *s2);
char	*ft_strncpy(char *dest, const char *src, size_t n);
size_t	ft_strlen(const char *str);
void	*ft_memset(void *pointer, int value, size_t count);

#endif