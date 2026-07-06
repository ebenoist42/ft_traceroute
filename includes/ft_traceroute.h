#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct s_data{

	struct addrinfo *res;
	
	int destination;
	int dns;
	int sockfd;
	int compteur_de_sonde;
}	t_data;

int test_ac(int ac, char **av);
void free_exit(char *str, t_data *data, int sortie);


#endif