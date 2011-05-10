/*
 ============================================================================
 Name        : main.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : Apr 12, 2011 - 10:44:39 AM
 Project     : client
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "gui_interface.h"

// hauptfunktion erwartet server und portnummer
int main(int argc, char ** argv)
{
		int id 		 = 0;
		char *name   = "Eric";
		char *server = "localhost";
		char *port   = "50000";
		int thread;

		GCI.name = name;
		struct addrinfo *addr_info, *p, hints;

		//zu wenig parameter
		if(argc<=2)
		{

			printf("Simple Echo-Client\nUsage: %s [SERVER] [PORT]\n", argv[0]);
			printf("\nExample: %s www.sixxs.net 80\n", argv[0]);
			exit(1);
		}

		//hinweise zur verbindung setzen
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = 0 /* | AI_ADDRCONFIG */;



		/* RTFM: getaddrinfo */
			thread = getaddrinfo(argv[1], argv[2], &hints, &addr_info);
			if (thread)
		{
			printf("getaddrinfo: %s\n", gai_strerror(ret));
			exit(1);
		}

		printf("\n");
			p = addr_info;

		//Threads starten
		while (p)
		{
			if(p->ai_socktype != SOCK_STREAM)/* we only care about TCP */
			{
				p = p->ai_next;
				continue;
			}
			int s = socket(p->ai_family, p->ai_socktype, 0);
			if(s == -1)
			{
				perror("socket");
				exit(-1);
			}

			if(connect(s, p->ai_addr, p->ai_addrlen) == 0)
			{
				GCI.sock = s;


				thread = pthread_create(&gui_thread_id, NULL, &gui_thread, NULL);
				if(thread)
				{
					printf("Failed to start GUI-Thread\n");
					exit(0);
				}

				sleep(1); /* mutex me please */

				thread = pthread_create(&command_thread_id, NULL, &command_thread, NULL);
				if(thread)
				{
					printf("Failed to start Command-Thread\n");
					exit(0);
				}
			}

			close(s);
			p = p->ai_next;
		}
		printf("Could not connect to server :/\n");
		raise(SIGINT);
		freeaddrinfo(addr_info);
		return 0;
	}


/*
 ======================================================================
***************************** GUI Thread*******************************
 ======================================================================
 */

void *gui_thread(void *data)
{

	  guiInit(&argc, &argv);  /* GUI initialisieren */

	     /* eigene Parameter verarbeiten */
	     /* Verbindung aufbauen, Threads erzeugen */

	   guiMain();       /* Hauptschleife der GUI */


}

)
