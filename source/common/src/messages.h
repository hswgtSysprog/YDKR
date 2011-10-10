#ifndef MESSAGES_H_
#define MESSAGES_H_

/**
 * ============================================================================
 * @file        : messages.h
 * @author      : Kathrin Holzmann
 * @date        : May 12, 2011
 * ============================================================================
 */

#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include "./question.h"

/*
 * RFC type defines
 */
#define RFC_LOGINREQUEST         1
#define RFC_LOGINRESPONSEOK      2
#define RFC_PLAYERLIST           3
#define RFC_CATALOGREQUEST       4  
#define RFC_CATALOGRESPONSE      5        
#define RFC_CATALOGCHANGE        6
#define RFC_STARTGAME            7
#define RFC_QUESTIONREQUEST      8 
#define RFC_QUESTION             9
#define RFC_QUESTIONANSWERED     10
#define RFC_QUESTIONRESULT       11
#define RFC_GAMEOVER             12

#define RFC_ERRORWARNING         255

/*
 * RFC role defines
 */

#define RFC_ROLE_DONTCARE    0
#define RFC_ROLE_SLAVE	     1
#define RFC_ROLE_MASTER      2



/*
 * RFC Error Codes
 */

#define RFC_ERROR_WARN        0
#define RFC_ERROR_FATAL       1
#define RFC_ERROR_LOGIN_FAIL 10

//Datenrahmen

struct msg_header {
	uint8_t type;    // describe the type of the message
	uint16_t length; // length of the next bytes
} __attribute__((__packed__));

typedef struct msg_header t_msg_header;


/*
 * Type:1 LoginRequest
 * Client --> Server
 */
typedef struct login_request{
        const uint8_t type;
        uint16_t length;
        char* name;
}LOGINREQUEST;


/*
 * Type:2 LoginResponseOk
 * Server --> Client
 */
typedef struct login_response_OK{
        const uint8_t type;
        uint16_t length;
        uint8_t clientID;
}LOGINRESPONSEOK;


/*
 * Type:3 PlayerList
 * Client --> Server
 */
typedef struct player_list{
        const uint8_t type;
        uint16_t length;
        char* players;                   // Das funktioniert so nicht, grad mit Fibu besprochen - Alternativmöglichkeiten?
}PLAYERLIST,playerls;


/*
 * Type:4 CatalogRequest
 * Client --> Server
 */
typedef struct catalog_request{
        const uint8_t type;
        uint16_t length;
}CATALOGREQUEST,catareq;


/*
 * Type:5 CatalogResponse
 * Server --> Client
 */
typedef struct catalog_response{
        const uint8_t type;
        const uint16_t length;
        char *filename;
}CATALOGRESPONSE,catares;


/*
 * Type:6 CatalogChange
 * Server --> Client
 * Client --> Server
 */

typedef struct catalog_change{
        const uint8_t type;
        uint16_t length;
        char *filename;
}CATALOGCHANGE,catach;


/*
 * Type:7 StartGame
 * Client --> Server
 */
typedef struct start_game{
        const uint8_t type;
        uint16_t length;
        char* filename;
}STARTGAME,startgm;


/*
 * Type:8 QuestionRequest
 * Client --> Server
 */
typedef struct question_request{
        const uint8_t type;
        uint16_t length;
}QUESTIONREQUEST,questionreq;


/*
 * Type:9 Question
 * Server --> Client
 */
typedef struct question{
        const uint8_t type;
        uint16_t length;
        QuestionMessage data;
}QUESTION;


/*
 * Type:10 QuestionAnswered
 * Client --> Server
 */
typedef struct question_answered{
        const uint8_t type;
        uint16_t length;
        uint8_t selection;
}QUESTIONANSWERED;


/*
 * Type:11 QuestionResult
 * Server --> Client
 */
typedef struct question_result{
        const uint8_t type;
        uint16_t length;
        uint8_t selection;
        uint8_t correct;
}QUESTIONRESULT;


/*
 * Type:12 GameOver
 * Server --> Client
 */
typedef struct game_over{
        const uint8_t type;
        uint16_t length;
        uint8_t rank;
}GAMEOVER;


/*
 * Type:255 ErrorWarning
 * Server --> Client
 */
typedef struct error_warning{
        const uint8_t type;
        uint16_t length;
        uint8_t subtype;
        char* message;
}ERRORWARNING;


#endif
