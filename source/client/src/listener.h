#ifndef LISTENER_H
#define LISTENER_H

/*============================================================================
 Name        : client.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 12, 2011 10:18:15 AM
 Project     : client
 =============================================================================*/

#include "../../common/src/messages.h"

void *listener_thread(void *data);
int parse_msg(t_msg_header *hdr);

#endif /* LISTENER_H */