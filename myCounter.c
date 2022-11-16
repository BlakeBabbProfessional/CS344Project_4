#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "myCounter.h"

#define MAX 10

int myCount;
pthread_mutex_t myMutex;
// Less than max
pthread_cond_t myCond1;
// Greater than 0
pthread_cond_t myCond2;

int main() {
	printf("PROGRAM START\n"); fflush(stdout);

	myCount = 0;

	pthread_mutex_init(&myMutex, NULL);

	pthread_t producer;
	pthread_create(&producer, NULL, produce, NULL);

	pthread_t consumer;
	pthread_create(&consumer, NULL, consume, NULL);

	usleep(10000);

	printf("PROGRAM END\n"); fflush(stdout);
	return 0;
}

/*
 * Start routine for a thread to increment count to MAX
 */
void* produce(void* argument) {
	printf("PRODUCER THREAD CREATED\n"); fflush(stdout);

	while (myCount < MAX) {
		pthread_mutex_lock(&myMutex);
		printf("PRODUCER: myMutex locked\n"); fflush(stdout);

		printf("PRODUCER: waiting on myCond1\n"); fflush(stdout);
		pthread_cond_wait(&myCond1, &myMutex);

		myCount++;

		pthread_mutex_unlock(&myMutex);
		printf("PRODUCER: myMutex unlocked\n"); fflush(stdout);

		pthread_cond_signal(&myCond2);
		printf("PRODUCER: signaled myCond2\n"); fflush(stdout);
	}

	return NULL;
}

/*
 * Start routine for a thread to print out count
 */
void* consume(void* argument) {
	printf("CONSUMER THREAD CREATED\n"); fflush(stdout);

	while (myCount < MAX) {
		pthread_mutex_lock(&myMutex);
		printf("CONSUMER: myMutex locked\n"); fflush(stdout);

		printf("CONSUMER: waiting on myCond2\n"); fflush(stdout);
		pthread_cond_wait(&myCond2, &myMutex);

		printf("CONSUMER: %d -> %d\n", myCount-1, myCount); fflush(stdout);

		pthread_mutex_unlock(&myMutex);
		printf("CONSUMER: myMutex unlocked\n"); fflush(stdout);

		pthread_cond_signal(&myCond1);
		printf("CONSUMER: signaled myCond1\n"); fflush(stdout);
	}

	return NULL;
}
