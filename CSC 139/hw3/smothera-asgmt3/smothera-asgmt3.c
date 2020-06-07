/*
Austin Smothers
CSC 139
smothera-asgmt3.c
A program to created a bounded buffer of
multithreaded producer(s)/consumer(s)
*/


/*======================================================
		 IMPORTS AND GLOBAL VARIABLE DECLARATION
======================================================*/

#include <pthread.h>
#include <stdlib.h>		/* required for rand() */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "buffer.h"


/*======================================================
		  AUXILIARY FUNCTION DECLARATION BLOCK
======================================================*/

void* producer(void* param);
void* consumer(void* param);


/*======================================================
				   MAIN FUNCTION BLOCK
======================================================*/

int main (int argc, char *argv[]) {
	int sleepTime, numProds, numCons, i, exit_code = 0;
	pthread_t tid;
	pthread_attr_t attr;

	/* 1. Get command line arguments argv[1], argv[2], argv[3] */
	if (argc == 4) {
		sleepTime = atoi(argv[1]);
		numProds = atoi(argv[2]);
		numCons = atoi(argv[3]);
	}
	else {
		fprintf(stderr, "usage: proj3 [Sleep time before terminating] [Number of Producer threads] [Number of Consumer threads]\n");
		return(1);
	}
	// Create arrays to store consumer and producer threads, so we can clean them up later
	pthread_t prodThreads[numProds], conThreads[numCons];
	/* 2. Initialize buffer */
	init();
	/* 3. Create producer thread(s) */
	for (i = 0; i < numProds; i++) {
		pthread_attr_init(&attr);
		exit_code = pthread_create(&tid, &attr, producer, NULL);
		if (exit_code != 0)
			printf("Failed to create producer thread\n");
		prodThreads[i] = tid;
	}
	/* 4. Create consumer thread(s) */
	for (i = 0; i < numCons; i++) {
		pthread_attr_init(&attr);
		exit_code = pthread_create(&tid, &attr, consumer, NULL);
		if (exit_code != 0)
			printf("Failed to create consumer thread\n");
		conThreads[i] = tid;
	}
	/* 5. Sleep */
	sleep(sleepTime);
	/* 6. Exit */
	/*for (i = 0; i < numProds; i++) {
		exit_code = pthread_cancel(prodThreads[i]);
		if (exit_code != 0)
			printf("Failed to cancel producer thread\n");
	}
	for (i = 0; i < numCons; i++) {
		exit_code = pthread_cancel(conThreads[i]);
		if (exit_code != 0)
			printf("Failed to cancel consumer thread\n");
	}*/
	exit(0);
}


/*========================================================
		   AUXILIARY FUNCTION DEFINITION BLOCK
========================================================*/

// Producer thread entry point
void* producer(void* param) {
	buffer_item item;
	pthread_t tid = pthread_self();
	while (1) {
		/* sleep for a random period of time */
		sleep(rand()%7);
		/* generate a random number */
		item = rand() + 1;
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);		
		if (insert_item(item))
			fprintf(stderr, "Producer %d failed to insert item\n", tid);
		else
			printf("producer %d produced %d\n", tid, item);
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void* consumer(void* param) {
	buffer_item item;
	pthread_t tid = pthread_self();
	while (1) {
		/* sleep for a random period of time */
		sleep(rand()%7);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		if (remove_item(&item))
			fprintf(stderr, "Consumer %d failed to remove item\n", tid);
		else
			printf("consumer %d consumed %d\n", tid, item);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}
