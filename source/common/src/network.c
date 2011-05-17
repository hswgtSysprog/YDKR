/*
 ============================================================================
 Name        : network.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 Description : network stuff
 ============================================================================
 */

#include "network.h"

/*
 * Accept socket connection
 */
int sock_acct( int sock ) {
	return accept( sock, NULL, NULL );
}

/*
 * Open socket
 */
int sock_open( int port ) {
	struct sockaddr_in6 server_address;
	int sock = socket(AF_INET6, SOCK_STREAM, 0);

	if (sock < 0) {
		return -1;
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin6_family = AF_INET6;
	server_address.sin6_addr = in6addr_any;
	server_address.sin6_port = htons(port);

	int ret = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));

	if( ret < 0 ) {
		sock_close(&sock);
		return -1;
	}

	if (listen(sock, 1) < 0) {
		sock_close(&sock);
		return -1;
	}

	return sock;
}


/*
 * Close socket
 */
void sock_close( int *sock ) {
    if ( *sock == 0 ) {
        return;
    }

    close(*sock);
    *sock = 0;
}
