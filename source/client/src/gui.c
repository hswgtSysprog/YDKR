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

void preparation_onCatalogChanged(const char *newSelection){

}
void preparation_onStartClicked(const char *currentSelection){

}
void preparation_onWindowClosed(void){

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
	guiShowMessageDialog("Wollen Sie das Programm wirklich beenden?", 1);
	/*
	 *void guiQuit(void);
	 */
}
