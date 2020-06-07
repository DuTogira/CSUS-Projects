/* buffer.h */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty, full;

void print_buffer() {
	printf("\nCurrent Buffer:\n");
	for (int i = 0; i < BUFFER_SIZE; i++)
		printf("%d %d\n", i, buffer[i]);
	printf("\n");
}

int insert_item(buffer_item item){
	/* insert item into buffer
	return 0 if successful, otherwise
	return -1 indicating an error condition */
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (buffer[i] == 0) {
			buffer[i] = item;
			//print_buffer();
			return 0;
		}
	}
	//print_buffer();
	return -1;

}

int remove_item(buffer_item* item) {
	/* remove an object from buffer
	placing it in item
	return 0 if successful, otherwise
	return -1 indicating an error condition. */
	if (buffer[0] > 0) {
		*item = buffer[0];
		for (int i = 0; i < BUFFER_SIZE - 1; i++) 
			buffer[i] = buffer[i + 1];
		buffer[BUFFER_SIZE - 1] = 0;
		//print_buffer();
		return 0;
	}
	return -1;
}

void init() {
	/* Initalize the mutex object
	along with empty and full semaphores */
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	for (int i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = 0;
}

