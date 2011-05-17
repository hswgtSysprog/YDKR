/*
 ============================================================================
 Name        : network.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 Description : network stuff header
 ============================================================================
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>


#include "network.h"

/*
 * Internal used struct
 */
struct sock_info
{
	int sock_count;
	int *sock;
};

typedef struct sock_info t_sock_info;


/*
 * Network functions
 */
int sock_acct( int sock );
int sock_open( int port );
void sock_close( int *sock );


#endif /* NETWORK_H_ */
