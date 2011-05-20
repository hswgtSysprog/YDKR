#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "listener.h"
#include "client.h"
#include "gui.h"
#include "command.h"

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
	// void refresh_playerlist(&hdr);
    }
    else if(hdr->type == RFC_QUESTION)
    {
      //Frage anzeigen
      //TODO: darf ich das hier aufrufen?
      // void game_setQuestion(const char *text);
    }
    else if (hdr->type == RFC_ERRORWARNING)
    {
      //TODO: Fehlerbehandlungsmethoden entwickeln
      //Warnung ausgeben
    }
    
    return 0;
}

/*
 ========================================================================
 Funktion:
	 refresh_playerlist()
 Kurzbeschreibung:
	Funktion zum aktualisieren der Spielerliste
========================================================================
 */

void refresh_playerlist(t_msg_header *hdr)
{
  //hier die Spieler Reihenfolge anhand der uebrgebenen Datenaktualisieren
  /*
   * TODO: herausfinden ob das ein array ist auf das sizeof anwenden geht
  int anzSpieler = sizeof(array)/sizeof(int);
  int i=0;
  
    while(i<anzSpieler)
    {
    TODO: darf ich das hier aufrufen?
      game_setPlayerName(int position, const char *name);
      game_setPlayerScore(int position, unsigned long score);
    }
    
      eigenen Usernamen hervorheben:
    void game_highlightPlayer(GCI.ID);
    */
  return;

}