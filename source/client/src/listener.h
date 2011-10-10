#ifndef LISTENER_H
#define LISTENER_H

/**
 * ============================================================================
 * @file        : client.c
 * @author      : Kathrin Holzmann
 * @date        : May 12, 2011
 * ============================================================================
 */

#include "../../common/src/messages.h"
#include "../../common/src/question.h"

void *listener_thread(void *data);
int parse_msg(t_msg_header *hdr);
void preparation_refresh_playerlist(t_msg_header *hdr);

typedef struct{
	uint8_t selection;
	uint8_t correct;
}questionResult;

#endif /* LISTENER_H */