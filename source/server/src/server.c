/*
 ============================================================================
 Name        : server.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 Description : Server fuer das Spiel "You dont know Rainer"
 ============================================================================
 */

#include "server.h"

int main(int argc, char ** argv)
{
	char *server = "0.0.0.0";
	int port = 0;

	// int ret;

	if (argc == 2)
	{
		/**
		 * Port = argv[2]
		 */
		port = atoi(argv[1]);
		printf("Starting with server=%s and port=%s\n", server, argv[1]);
	}
	else
	{
		/**
		 * Bei Aufruf ohne Argumente
		 */
		printf("YDKR-Server\nUsage: %s [SERVICE]\n", argv[0]);
		printf("\nExample: %s 8080\n", argv[0]);
		exit(1);
	}

	printf("waiting for clients\n");
	while( 1 ) {
		// auf Spieler warten
		loginThread(port);
	}
}
