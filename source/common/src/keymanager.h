/**
 * ============================================================================
 * @file        : client.h
 * @author      : Kathrin Holzmann
 * @date        : May 10, 2011
 * ============================================================================
 */
#ifndef KEYMANAGER_H_
#define KEYMANAGER_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

/* server */


/* client */
#define KEY_GCI_SEM     0x42 // key for global struct
#define KEY_GCISOCK_SEM 0x23 // key for client socket

#define KEY_COMMAND_MQ      0x41 // command message queue
#define KEY_RANKING	    0x44 // triggers update of ranking list
#define KEY_QUESTION	    0x43 // next question

key_t keymng(int offset);
key_t keymng_local(int offset);

#endif
