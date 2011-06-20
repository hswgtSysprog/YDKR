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
        char message[50];
	
	
	while(1)
	{
		receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);		
		if (receiver ==0 || receiver < sizeof(hdr))
		{
                 
		 sprintf(message,"Die Verbindung zum Server wurde getrennt!");
                 guiShowErrorDialog(message,1);
                 raise(SIGINT);
		
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
                {
                    preparation_clearPlayers();
                    
                }else
                {
                  //TODO: clearup playerlist while playing
					int clearP;
					for(clearP=1;clearP<=6;clearP++){
						game_setPlayerName(clearP,NULL);
						game_setPlayerScore(clearP,0);
					}

                }
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
		
                if( ret == 0 || ret < hdr->length )
                {
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

                
                if( ret == 0 || ret < hdr->length )
                {
			guiShowErrorDialog("Spielstart fehlgeschlagen.", 1);
                        raise(SIGINT);
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
/*#########################################QUESTION COMES IN###################################################*/
	else if(hdr->type == RFC_QUESTION)
        {
            printf("get question: \n");
            int i, ret;
            char message[50];
            memset(message,0,50);
            game_setStatusIcon(0);

            QuestionMessage fragen;
                
            printf("\nsize of header: %i\n", hdr->length);
            
            if( hdr->length != 0 )
            {
                ret = recv(GCI.sock, &fragen, hdr->length, 0);
                
                printf("frage: %s", fragen.question);
                game_setQuestion(fragen.question);
                bzero(fragen.question,sizeof(fragen.question));
                //fragen.question = NULL;
                //show answers
                for(i=0;i<=3;i++)
                {
                    printf("answer: %s", fragen.answers[i]);
                    game_setAnswer(i, fragen.answers[i]);                      
                }
                   game_setAnswerButtonsEnabled(1);
                    //show Time
                    sprintf(message,"Sie haben %i Sekunden Zeit",ntohs(fragen.timeout));
               }
               else
               {
                   sprintf(message,"Alle Fragen beantwortet, bitte Warten.");
               }
                
              game_setStatusText(message);
              sprintf(message," ");
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
            
            send_QR(3);
            game_unmarkAnswers();
        }
/*########################GAME OVER#####################################################*/        
        else if(hdr->type == RFC_GAMEOVER)
        {
            uint8_t rank;
            char message[50];
            GCI.status=end;
            
           
            ret = recv(GCI.sock, &rank, hdr->length, MSG_WAITALL);
            
            
            sprintf(message,"Game Over! Du hast Rang: %i erreicht!",rank);
            guiShowMessageDialog(message, 1);
            raise(SIGINT);      
        }
        /*##########################ERROR HONEY###########################################*/
        else if (hdr->type == RFC_ERRORWARNING)
        {
                uint8_t error_code;
                char *error_message = "Fail!";
                
                
                ret = recv(GCI.sock, &error_code, sizeof(uint8_t), MSG_WAITALL);
                
                
                if(ret == 0 || ret < sizeof(uint8_t))
                {
                        raise(SIGINT);
                }

                if(hdr->length != 1)
                {
                        error_message = malloc(hdr->length + 1);
                        if(!error_message) raise(SIGINT);
                        
                        
                        ret = recv(GCI.sock, error_message, hdr->length - 1, MSG_WAITALL);
                       
                        
                        if( ret == 0 || ret < hdr->length )
                        { /* disconnect or error */
                                guiShowMessageDialog("Unbekannter Fehler", 1);
                                raise(SIGINT);
                        }
                        error_message[hdr->length] = '\0';
                }
                // TODO: Print Error on Terminal and GUI Interface
                printf("Error <code %x>\n", error_code);
                switch(error_code)
                {
                    case RFC_ERROR_WARN:
                                guiShowMessageDialog("Warnung!", 0);
                    break;
                    case RFC_ERROR_FATAL:
                        guiShowMessageDialog("Fehler! Das Programm muss beendet werden", 1);
                        raise(SIGINT);
                    break;
                    
                    case RFC_ERROR_LOGIN_FAIL:
                        printf("Sie koennen sich nicht anmelden");
                        raise(SIGINT);
                    break;
                    default:
                        printf("unbekannter fehler, das programm wird abgebrochen");
                        raise(SIGINT);
                }
        }

    return 0;
}
