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
#include "listener.h"
#include "client.h"
#include "gui.h"
#include "command.h"
#include "question.h"
#include "gui_interface.h"

pthread_t question_thread_id;
/*============================================================================
 Name        : listener.c
 Author      :Kathrin Holzmann
 Version     : May 16, 2011 08:38:47 PM
 Project     : client
 =============================================================================
*/

void *listener_thread(void *data)
{
	t_msg_header hdr;
	int receiver=0;
	int parser=0;
	
	
	while(1)
	{
		receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);		
		if (receiver ==0 || receiver < sizeof(hdr))
		{
		 printf("Fehler\n");
		 return 0;
		}	
		
		hdr.length = ntohs(hdr.length);
		parser = parse_msg(&hdr);
	}
}


/*
 ========================================================================
 Funktion:
	 refresh_playerlist()
 Kurzbeschreibung:
	Funktion zum aktualisieren der Spielerliste
========================================================================
 */


int parse_msg(t_msg_header *hdr)
{
	size_t ret = 0;

	if(hdr->type == RFC_PLAYERLIST)
	{
		//Rangliste aktualisieren
		
		printf("Spielerliste erhalten \n");
		
		uint32_t punktestand;
		uint8_t playerid;
		int i, anzahl, length;
		
		 
		anzahl= (hdr->length/37);
		printf("Anzahl der Spieler: %d \n", anzahl);
		
		// erstmal aufraeumen
		if(GCI.status==preparation)
		preparation_clearPlayers();
		
		for(i=0; i < anzahl; ++i) 
		{
			// for each player
			char*  playername = malloc(32); // speicher vorbereiten
			if( playername == NULL) { printf("shit happend\n"); exit(-1);}
				 // spielername
			ret= recv(GCI.sock, playername, 32, MSG_WAITALL);
			if( ret ==0 || ret < 32) {
				return ERR_KILL_CLIENT;
			}
			recv(GCI.sock, &punktestand, 4, MSG_WAITALL);
			recv(GCI.sock, &playerid, 1, 0);
			
			length = strlen(playername);
			printf("Playername: %s \n", playername);
                        if(GCI.status==preparation)
                        {
                            preparation_addPlayer(playername);
                        }
			
			// speicher wieder freigeben
			free(playername);
		}
	}
	else if(hdr->type == RFC_CATALOGRESPONSE || hdr->type == RFC_CATALOGCHANGE) {
		char *filename = malloc(hdr->length + 1);
		if(!filename) return ERR_OOM;

		ret = recv(GCI.sock, filename, hdr->length, 0);
		if( ret == 0 || ret < hdr->length ) {
			return ERR_KILL_CLIENT;
		}
		filename[hdr->length] = '\0';

		if( hdr->type == RFC_CATALOGRESPONSE )
			preparation_addCatalog(filename);
		else if( hdr->type == RFC_CATALOGCHANGE )
			preparation_selectCatalog(filename);

		free(filename);
	}
	else if (hdr->type == RFC_STARTGAME) {
                int thread;
		char *filename = malloc(hdr->length + 1);
		if(!filename) return ERR_OOM;

		ret = recv(GCI.sock, filename, hdr->length, 0);
		if( ret == 0 || ret < hdr->length ) {
			return ERR_KILL_CLIENT;
		}
		filename[hdr->length] = '\0';
		GCI.status = playing;

		// TODO: Tell the gui to start the game
                preparation_hideWindow();
                
                thread = pthread_create(&question_thread_id, NULL, &question_thread, NULL);
                if(thread)
                {
                    printf("Failed to start Listener Thread\n");
                    exit(0);
                }
                game_showWindow();
                          
		// TODO: Check if filename 0 (it's allowed)

		free(filename);
	}
	else if (hdr->type == RFC_ERRORWARNING) {
		uint8_t error_code;
		char *error_message = "Fail!";
		ret = recv(GCI.sock, &error_code, sizeof(uint8_t), MSG_WAITALL);
		if(ret == 0 || ret < sizeof(uint8_t)) {
			return ERR_KILL_CLIENT;
		}

		if(hdr->length != 1) {
			error_message = malloc(hdr->length + 1);
			if(!error_message) return ERR_OOM;
			ret = recv(GCI.sock, error_message, hdr->length - 1, MSG_WAITALL);
			if( ret == 0 || ret < hdr->length ) { /* disconnect or error */
				return ERR_KILL_CLIENT;
			}
			error_message[hdr->length] = '\0';
		}
		// TODO: Print Error on Terminal and GUI Interface
		printf("Error <code %x>\n", error_code);
		switch(error_code) {
			case RFC_ERROR_WARN:
				return 0;
			default:
				/* unknown error type */
				return ERR_KILL_CLIENT;		
		}
	}
    return 0;
}
