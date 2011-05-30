/*
 ============================================================================
 Name        : gui.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : Apr 16, 2011 9:01:56 AM
 Project     : client
 ============================================================================
 */

#include <stdio.h>
#include "gui_interface.h"
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
#include "listener.h"
#include <getopt.h>


/*
 ========================================================================
 Funktion:
         preparation_onCatalogChanged
 Kurzbeschreibung:
         Callback Funktion.
         Wird aufgerufen wenn der Spielleiter einen neuen Catalog
         auswaehlt.
 ========================================================================
 */


void preparation_onCatalogChanged(const char *newSelection)
{
  printf("cataloge change \n");
  printf("selection name?: %s\n", newSelection);
  
  if(strlen(newSelection)==0)
      return;
  
  t_msg_header hdr;
        
  hdr.type = RFC_CATALOGCHANGE;
  hdr.length = htons(strlen(newSelection));   
  send(GCI.sock, &hdr, sizeof(hdr), MSG_MORE);
  send(GCI.sock, newSelection, strlen(newSelection), 0);

}

void preparation_onStartClicked(const char *currentSelection){
    
    printf("cataloge change \n");
    printf("selection name?: %s\n", currentSelection);
  
    if(strlen(currentSelection)==0)
      return;
  
    t_msg_header hdr;
    
    hdr.type = RFC_STARTGAME;
    hdr.length = htons(strlen(currentSelection));   
    send(GCI.sock, &hdr, sizeof(hdr), MSG_MORE);
    send(GCI.sock, currentSelection, strlen(currentSelection), 0);

}
void preparation_onWindowClosed(void){
    
  guiShowMessageDialog("Wollen Sie das Programm wirklich beenden?", 0);
  sleep(2);
        guiQuit();
}

void game_onAnswerClicked(int index){

}
/*
 ========================================================================
 Funktion:
	 game_onWindowClosed
 Kurzbeschreibung:
	 Callback Funktion.
	 Wird aufgerufen wenn ein Benutzer den Schliessen Button (x) des
	 Anwendungsfensters betaetigt.

 ========================================================================
 */

void game_onWindowClosed(void) {
  /*TODO: rueckgabe von guiShowMessageDialog, welche Reihenfolge zum beenden?
   */
	guiShowMessageDialog("Wollen Sie das Programm wirklich beenden?", 0);
        sleep(2);
	guiQuit();
	 
}
