#ifndef SEM_H
#define	SEM_H

#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "keymanager.h"

int 	sem_open(key_t key);
int 	sem_remove(key_t key);
int 	sem_V(key_t key);
int 	sem_P(key_t key);

#endif
