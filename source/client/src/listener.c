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
/*####################PLAYERLISTE######################*/
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
				 
				 
			sem_P(keymng_local(KEY_GCI_SEM));	 
			 // spielername 
			ret= recv(GCI.sock, playername, 32, MSG_WAITALL);
                        sem_V(keymng_local(KEY_GCI_SEM));
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
                        else if(GCI.status==playing)
                        {
                            game_setPlayerName(i+1,playername);
                            if(GCI.ID==playerid){ game_highlightPlayer(i+1);} //highlight player
                            game_setPlayerScore(i+1,ntohl(punktestand));
                        }
			
			// speicher wieder freigeben
			free(playername);
		}
	}
	
/*###############################CATALOG HANDLING#################################*/
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
	
/*#################################GAME ON STARTUP ####################################*/
	else if (hdr->type == RFC_STARTGAME) {
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
                game_showWindow();
                    //get first Question
                send_QR(0);

                          
		// TODO: Check if filename 0 (it's allowed)

		free(filename);
	}
/*##########################ERROR HONEY###########################################*/
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
/*#########################################QUESTION COMES IN###################################################*/
	else if(hdr->type == RFC_QUESTION)
        {
            printf("get question: \n");
          int i, ret;
                char message[50];
                memset(message,0,50);
                game_setStatusIcon(0);

                QuestionMessage fragen;
                ret = recv(GCI.sock, &fragen, hdr->length, MSG_WAITALL);
                printf("frage: %s", fragen.question);
                game_setQuestion(fragen.question);

                //show answers
                for(i=0;i<=3;i++)
                {
                    printf("answer: %s", fragen.answers[i]);
                    game_setAnswer(i, fragen.answers[i]);
                   
                }

                //show Time
                sprintf(message,"Sie haben %i Sekunden Zeit",ntohs(fragen.timeout));
                game_setStatusText(message);

      }
/*#######################Question Result#####################*/
        else if(hdr->type== RFC_QUESTIONRESULT)
        {
            printf("Questionresul: \n");
            int ret;
            game_unmarkAnswers();
            questionResult result;
            ret = recv(GCI.sock, &result, hdr->length, MSG_WAITALL);
            
            if(ret ==0 || ret <hdr->length)
            {
                printf("Falsche paketgroesse \n");
                return -1;
            }
            
            game_markAnswerCorrect(result.correct);
            
            //parse result
            
            if(result.selection == 255)
            {
               game_setStatusText("Zeit abgelaufen");
                game_setStatusIcon(3);
            }
            else if(result.selection!=result.correct)
            {
                game_setStatusText("Schade, leider Falsch!");
                game_setStatusIcon(2);
                game_markAnswerWrong(result.selection);
            }else
            {
                game_setStatusText("Super richtige Antwort!");
                game_setStatusIcon(1);
            }
            
            send_QR(5);
            game_unmarkAnswers();
        }
    return 0;
}
