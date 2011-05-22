#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "listener.h"
#include "client.h"
#include "gui.h"
#include "command.h"
#include "gui_interface.h"

/*============================================================================
 Name        : listener.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 16, 2011 08:38:47 PM
 Project     : client
 =============================================================================
*/


/* TODO: Handshake zwischen listener und anderen threads? semaphor?
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
		 printf("Fehler");
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
    if(hdr->type == RFC_PLAYERLIST)
    {
	//Rangliste aktualisieren
	
	printf("Spielerliste erhalten \n");
	
	uint32_t punktestand;
	uint8_t playerid;
	int i, anzahl, length, ret;
	
	 
	anzahl= (hdr->length/37);
	printf("Anzahl der Spieler: %d \n", anzahl);
	
	// erstmal aufraeumen
	
	preparation_clearPlayers();
	
	for(i=0; i < anzahl; ++i) 
	{
	  // for each player
	  char*  playername = malloc(32); // speicher vorbereiten
	  if( playername == NULL) { printf("shit happend\n"); exit(-1);}
	 
	 // spielername
	 ret= recv(GCI.sock, playername, 32, MSG_WAITALL);
	 if( ret ==0 || ret < 32)
	 {
	    printf("zu wenig byte gelesen");
	    return -1;
	  }
	  recv(GCI.sock, &punktestand, 4, MSG_WAITALL);
	  recv(GCI.sock, &playerid, 1, 0);
	  
	  length = strlen(playername);
	  printf("Playername: %s \n", playername);
	  preparation_addPlayer(playername);
	  
	  // speicher wieder freigeben
	  free(playername);
	  
	}
  
    }
  /*  else if(hdr->type == RFC_CATALOGRESPONSE)
    {
      int ret, laenge;
      
     // ret= recv(GCI.sock, &laenge, 32, 0);
      
      //printf("laenge: %d \n", laenge);
      char * katalogname;
      
     // preparation_addCatalog(katalogname);
      
    }*/
    else if (hdr->type == RFC_ERRORWARNING)
    {
      //TODO: Fehlerbehandlungsmethoden entwickeln
      //Warnung ausgeben
    }
    else 
        {
	  printf("WAHH KENN ICH NED %i\n", hdr->type);
	}  
    return 0;
}
