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
#include "client.h"
#include "command.h"

pthread_t gui_thread_id, command_thread_id, listener_thread_id;
// hauptfunktion erwartet server und portnummer
int main(int argc, char **argv)
{
	char *name   = "Eric";
	char *server = "localhost";
	char *port   = "50000";
	int thread;


	GCI.name = name;
	struct addrinfo *addr_info, *p, hints;
	//signal(SIGINT, sigint_handler);
	

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
		//raise(SIGINT)
	  
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

	  guiInit(0, NULL);  // GUI initialisieren */

	     /* eigene Parameter verarbeiten */
	     /* Verbindung aufbauen, Threads erzeugen */

	   guiMain();       /* Hauptschleife der GUI */
	   
	   //TODO: darf das hier so stehen bleiben ???
	   //void guiDestroy(void);
}