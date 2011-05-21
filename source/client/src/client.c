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

pthread_t command_thread_id, listener_thread_id;
// hauptfunktion erwartet server und portnummer
int main(int argc, char **argv)
{
	char *name   = "Eric";
	char *server = "localhost";
	char *port   = "54321";
	int thread;
	int ret;


	GCI.name = name;
	struct addrinfo *addr_info, *p, hints;

	ret = getaddrinfo(server, port, NULL, &addr_info);
	if(ret) {
		printf("getaddrinfo: %s\n", gai_strerror(ret));
		exit(-1);
	}

	p = addr_info;

		while (p)
		{
			if(p->ai_socktype != SOCK_STREAM)/* we only care about TCP */
			{
				p = p->ai_next;
				continue;
			}
			int sock = socket(p->ai_family, p->ai_socktype, 0);
			if(sock == -1)
			{
				perror("socket");
				exit(-1);
			}

			if(connect(sock, p->ai_addr, p->ai_addrlen) == 0)
			{
			    printf("Socket OK");
				GCI.sock = sock;
				send_login(GCI.name);
				int state = wait_loginOK();
				if(state !=0){
				 printf("Keine antwort erhalten \n");
				 return 0;
				}
				printf("juhu ich bin eingeloggt \n");
				
				guiInit(&argc, &argv);
				guiMain();
				/*
				 * thread = pthread_create(&listener_thread_id, NULL, &listener_thread, NULL);
				if(thread)
				{
					printf("Failed to start Listener Thread\n");
					exit(0);
				}
				//
				thread = pthread_create(&command_thread_id, NULL, &command_thread, NULL);
				if(thread)
				{
					printf("Failed to start Command-Thread\n");
					exit(0);
				}
				*/
			}

			close(sock);
			p = p->ai_next;
		}
		printf("Could not connect to server :/\n");
	  
		freeaddrinfo(addr_info);
		return 0;
}

void send_login(char* name)
{
	t_msg_header hdr;
	
	hdr.type = RFC_LOGINREQUEST;
	//umdrehen <=16 Bit werte
	hdr.length = htons(strlen(name) + 1);
	
	send(GCI.sock, &hdr, sizeof(hdr), MSG_MORE);
	send(GCI.sock, &name, sizeof(char) + strlen(name), 0);	
	printf("login was send \n");
}


int wait_loginOK()
{
  int receiver, ret;
  t_msg_header hdr;
  receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);
  // timer fuer timeout hier setzen
  
  
  if(receiver == 0 || receiver < sizeof(hdr)) 
  {
    printf("message wrong size \n");
    return -1;
  }
  
   printf("hdr length erstes: %d \n",hdr.length);
  
  hdr.length = ntohs(hdr.length);
  
  //is it what whe want?
  if(hdr.type != RFC_LOGINRESPONSEOK)
  {
    printf("message wrong type \n");
    return -1;
  }
//receive rest of package
  ret = recv(GCI.sock, &GCI.ID, sizeof(GCI.ID), 0);
    
 //did we receive great things?
  if(ret == 0 || ret < sizeof(GCI.ID))
  {
    printf("rest vom paket falsch \n");
      return -1;
  } 
 return 0;
}