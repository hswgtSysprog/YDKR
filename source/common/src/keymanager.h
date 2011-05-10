/*
 * keymanager.h
 *
 *  Created on: May 10, 2011
 *      Author: kathrin
 */

#ifndef KEYMANAGER_H_
#define KEYMANAGER_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

/* server */


/* client */
#define KEY_COMMAND_MQ      0x41 // command message queue

key_t keymng(int offset);
key_t keymng_local(int offset);

#endif
