#ifndef MESSAGES_H_
#define MESSAGES_H_

/*
 * keymanager.h
 *
 *  Created on: May 12, 2011
 *      Author: kathrin
 */

#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>

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


#endif