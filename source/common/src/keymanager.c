/**
 * ============================================================================
 * @file        : keymanager.c
 * @author      : Kathrin Holzmann
 * @date        : May 12, 2011
 * ============================================================================
 */
#include <unistd.h>
#include "keymanager.h"

/* generic keymanager */
key_t keymng(int offset) {
	key_t key;
	key = ftok(".", offset);
	if(key == -1) {
		printf("Error to create key!\n");
		exit(0);
	}
	return key;
}


key_t keymng_local(int offset)  {
	 /* shift the pid 4 bits, so there is room for our offsets without colliding keys */
	return keymng((getpid() << 4) + offset);
}
