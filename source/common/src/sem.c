/* binary semaphor */

#include <stdio.h>
#include <signal.h>
#include "sem.h"

/**
 * ============================================================================
 * @file        : sem.c
 * @author      : Kathrin Holzmann
 * @date        : June 02, 2011
 * ============================================================================
 */

int sem_open(key_t key) {
	int id = semget(key, 0, 0); /* Semaphor benutzen */
	if (id == -1) { /* Semaphor existiert noch nicht */
		id = semget(key, 1, IPC_CREAT|IPC_EXCL|0600); /* also erzeugen */

		if (id == -1) {
			printf("Fehler bei Semaphorerzeugung!\n");
			raise(SIGINT);
		}
		semctl(id, 0, SETVAL, 0);
	}
	return id;
}

int sem_remove(key_t key) {
	int semid = semget(key, 0, 0);
	if (semctl(semid, 0, IPC_RMID, 0) == -1) {
		printf("Fehler beim Löschen des Semaphors\n");
		return -1;
	}
	return 0;
}

/*  Ressourcen freigeben */
int sem_V(key_t key) {
	int semid = sem_open(key);
	struct sembuf sem_array[1];

	sem_array[0].sem_num 	= 0;
	sem_array[0].sem_op 	= 1;
	sem_array[0].sem_flg 	= SEM_UNDO;

	return semop(semid, sem_array, 1);
}

/* Ressourcen anfordern */
int sem_P(key_t key) {
	int semid = sem_open(key);
	struct sembuf sem_array[1];

	sem_array[0].sem_num 	= 0;
	sem_array[0].sem_op 	= -1;
	sem_array[0].sem_flg 	= SEM_UNDO;

	return semop(semid, sem_array, 1);
}

