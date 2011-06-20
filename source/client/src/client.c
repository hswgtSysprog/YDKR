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
#include <getopt.h>
#include "gui_interface.h"
#include "client.h"
#include "fragewechsel.h"
#include "listener.h"

pthread_t listener_thread_id, fragen_thread_id;

void print_help(char *self) {
	printf("%s [params] [server]\n"
	       "   parameters:\n"
	       "                 -h  --help\n"
	       "                 -n  --name <Guest>\n"
	       "                 -p  --port <54321>\n"
	       "\n", self);
	exit(0);
}


// hauptfunktion erwartet server und portnummer
int main(int argc, char **argv)
{
	char *name   = "Guest";
	char *server = "localhost";
	char *port   = "54321";
	int thread;
	int ret, c, conn=0;

	while(optind < argc) {
		int option_index = 0;
		static struct option long_options[] = {
			{"name",    required_argument, 0, 'n'},
			{"port",    optional_argument, 0, 'p'},
			{"help",    no_argument,       0, 'h'},
			{0,0,0,0}
		};
		c = getopt_long(argc, argv, "hp:n:", long_options, &option_index);
		if(c == -1) break;

		switch(c) {
			case '?': /* unknown parameter */
			case ':': /* missing argument */
				print_help(argv[0]);
				break;
			case 'n':
				name = strdup(optarg);
				if(!name) return -1;
				break;
			case 'p':
				port = strdup(optarg);
				break;
			case 'h':
				print_help(argv[0]);
				break;
			default:
				break;
		}
	}
	while(optind < argc) {
		server = argv[optind++];
	}

	GCI.name = name;
	
	printf("Benutzername: %s\n", GCI.name);
        printf("server: %s", server);
	struct addrinfo *addr_info, *p;

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
			    signal(SIGINT, sigint_handler);
                            // move to better positioon
                            sem_V(keymng_local(KEY_GCI_SEM));
                           
                            
                            printf("Socket OK");
                            GCI.sock = sock;
                            send_login(GCI.name);
                            int state = wait_loginOK();
                                
                            if(state !=0)
                            {
                                printf("Keine antwort erhalten \n");
                                return 0;
                            }

                            printf("juhu ich bin eingeloggt \n");
                            GCI.status = preparation;
                            conn =1;

                            guiInit(&argc, &argv);
                            printf("GUI init \n");

                            setClientMode();
                            preparation_showWindow();
                            guiShowMessageDialog("Willkommen bei You Dont Know Rainer",0);
                                                        
                            // start the threads
                            thread = pthread_create(&listener_thread_id, NULL, &listener_thread, NULL);
                            
                            if(thread)
                            {
                                printf("Failed to start Listener Thread\n");
                                exit(0);
                            }
                            
                              thread = pthread_create(&fragen_thread_id, NULL, &fragen_thread, NULL);
                            
                            if(thread)
                            {
                                printf("Failed to start Fragewechsel Thread\n");
                                exit(0);
                            }
				//katalog request
				sendCR();
				
				guiMain();
				guiDestroy();
			}

			close(sock);
			p = p->ai_next;
		}
		if(conn==0)
		printf("Could not connect to server :/\n");
                
                freeaddrinfo(addr_info);
		exit(0);
}


/*-----------void send_login(char* name)----------------------
@description: sends the login name to the server
-----------------------------------------------------------*/
void send_login(char* name)
{
  t_msg_header hdr;
	
  hdr.type = RFC_LOGINREQUEST;
  //umdrehen <=16 Bit werte
  hdr.length = htons(strlen(name));	
  send(GCI.sock, &hdr, sizeof(hdr), MSG_MORE);
  send(GCI.sock, name, strlen(name), 0);	
  printf("login was send \n");
}

/*-----------int wat_loginOK()---------------------------
@description: waits until login response is ok
-----------------------------------------------------------*/
int wait_loginOK()
{
  int receiver, ret;
  t_msg_header hdr;
  receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);
  
  if(receiver == 0 || receiver < sizeof(hdr)) 
  {
    printf("message wrong size \n");
    return -1;
  }
  
   
  
  hdr.length = ntohs(hdr.length);
  
  //is it what whe want?
  if(hdr.type != RFC_LOGINRESPONSEOK)
  {
      
    if(hdr.type ==RFC_ERRORWARNING)
    {
     uint8_t error_code;
     char *error_message = "Fail!";
     int ret;
          
        ret = recv(GCI.sock, &error_code, sizeof(uint8_t), MSG_WAITALL);
        
        if(hdr.length != 1)
        {
            error_message = malloc(hdr.length + 1);
            if(!error_message) raise(SIGINT);
            ret = recv(GCI.sock, error_message, hdr.length - 1, MSG_WAITALL);
            error_message[ strlen(error_message )] = '\0';
        }          
         
      printf("Fehler: %s \n", error_message);
        raise(SIGINT);
    }
    printf("unbekannter fehler!\n");
        raise(SIGINT);
       
    
  }
//receive rest of package and save the getted client ID
  ret = recv(GCI.sock, &GCI.ID, hdr.length, 0);
  
  
  printf("Client ID: %d",GCI.ID);
 //did we receive great things?
  if(ret == 0 || ret < hdr.length)
  {
    printf("rest vom paket falsch \n");
      return -1;
  } 
 return 0;
}

/*-----------void setClientMode()---------------------------
@description: sets the Client Mode to privileged or normal
-----------------------------------------------------------*/

void setClientMode()
{
    if(GCI.ID ==0)
    {
      printf("Spielleiter\n");
      preparation_setMode(PREPARATION_MODE_PRIVILEGED);
    }else
    {
      printf("Spieler \n");
      preparation_setMode(PREPARATION_MODE_NORMAL);
    }
}


/*-----------void sendCR()---------------------------
@description: sends the Catalouge Request
-----------------------------------------------------------*/

void sendCR()
{
  
  t_msg_header hdr;
  hdr.type = RFC_CATALOGREQUEST;
  printf("cr: %i \n", hdr.type);
    //umdrehen <=16 Bit werte
  hdr.length = 0;
  
  sem_P(keymng_local(KEY_GCI_SEM));
  send(GCI.sock, &hdr, sizeof(hdr), 0);
  sem_V(keymng_local(KEY_GCI_SEM));
  
  printf("CR was send \n"); 
}

/* ========================================================================
 Funktion:
        send_QR
 Kurzbeschreibung:
        Fordert die nächste Frage an.
 ========================================================================
 */

void send_QR(int sleepTime)
{
   sleep(sleepTime);
  t_msg_header hdr;
  hdr.type = RFC_QUESTIONREQUEST;
  printf("cr: %i \n", hdr.type);
    //umdrehen <=16 Bit werte
  hdr.length = 0;       
  
  sem_P(keymng_local(KEY_GCI_SEM));
  send(GCI.sock, &hdr, sizeof(hdr), 0);
  sem_V(keymng_local(KEY_GCI_SEM));
  
  printf("QR was send \n"); 
}

void sigint_handler(int sig) {
       signal(sig, SIG_IGN);
        
        printf("Shutting down... ");
        // fuck off, doing things aem random things break all 
        pthread_cancel(listener_thread_id);
        sem_remove(keymng_local(KEY_GCI_SEM));
     
        printf("OK\n");
       
        
        exit(0);
}
