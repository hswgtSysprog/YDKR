/*
 ============================================================================
 Name        : question.c
 Author      : Kathrin Holzmann
 Version     : May 25, 2011 
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
#include "question.h"
#include <getopt.h>

void *question_thread(void *data)
{
    int receive =-1;
    
    while(receive!=0)
    {
       send_QR();
       receive=getQuestion();
      //waitAnswer
       //semaphor 
       receive =0;
    }
    return 0;
}

/*
 ========================================================================
 Funktion:
        send_QR
 Kurzbeschreibung:
        Fordert die nächste Frage an.
 ========================================================================
 */

void send_QR()
{
  t_msg_header hdr;
  hdr.type = RFC_QUESTIONREQUEST;
  printf("cr: %i \n", hdr.type);
    //umdrehen <=16 Bit werte
  hdr.length = 0;       
  send(GCI.sock, &hdr, sizeof(hdr), 0);
  printf("QR was send \n"); 
}

int getQuestion()
{
  int receiver, ret, i;
  t_msg_header hdr;
  receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);
  // timer fuer timeout hier setzen
  
  
  if(receiver == 0 || receiver < sizeof(hdr)) 
  {
    printf("message wrong size \n");
    return -1;
  }
  
   printf("hdr length: %d \n",hdr.length);
  
  //hdr.length = ntohs(hdr.length);
  
  //is it what whe want?
  if(hdr.type != RFC_QUESTION)
  {
    printf("message wrong type %d \n", hdr.type);
    return -1;
  }
  printf("Type: Question:\n");
  char* frage = malloc(256);
//receive rest of package and save the getted client ID
  ret = recv(GCI.sock, frage, hdr.length, MSG_WAITALL);
  printf("Frage: %s\n",frage);
  game_setQuestion(frage);
 //did we receive great things?
  free(frage);
  
  for(i=0; i<4; i++)
  {
       char* antwort=malloc(128);
       if( antwort == NULL) { printf("shit happend\n"); exit(-1);}
        printf("mallociren %d \n",i);
        if(i==3)
        {
            ret= recv(GCI.sock, antwort, 128, 0);
        }
        else
        {
            ret= recv(GCI.sock, antwort, 128, MSG_WAITALL);
        }
   
        if(ret == 0 || ret < hdr.length)
        {
            printf("rest vom paket falsch \n");
            return -1;
        }
        printf("Antwort: %s\n", antwort);
        game_setAnswer(i+1, antwort);
        
        free(antwort);
  }
 
 return 0;
    
}
