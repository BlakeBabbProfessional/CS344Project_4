#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "myCounter.h"

#define MAX 10

int myCount;
int canPrint = 0;
pthread_mutex_t myMutex;
// Can increment myCount
pthread_cond_t myCond1 = PTHREAD_COND_INITIALIZER;
// Can print myCount
pthread_cond_t myCond2 = PTHREAD_COND_INITIALIZER;

int main() {
	printf("PROGRAM START\n"); fflush(stdout);

	myCount = 0;

	pthread_mutex_init(&myMutex, NULL);

	pthread_t producer, consumer;
	pthread_create(&producer, NULL, produce, NULL);
	pthread_create(&consumer, NULL, consume, NULL);

	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	printf("PROGRAM END\n"); fflush(stdout);
	return 0;
}

/*
 * Start routine for a thread to increment myCount to MAX
 */
void* produce(void* argument) {
	printf("PRODUCER THREAD CREATED\n"); fflush(stdout);

	while (myCount < MAX) {
		pthread_mutex_lock(&myMutex);
		printf("PRODUCER: myMutex locked\n"); fflush(stdout);

		while (canPrint) {
			printf("PRODUCER: waiting on myCond1\n"); fflush(stdout);
			pthread_cond_wait(&myCond1, &myMutex);
		}

		myCount++;
		canPrint = 1;

		pthread_cond_signal(&myCond2);
		printf("PRODUCER: signaled myCond2\n"); fflush(stdout);

		pthread_mutex_unlock(&myMutex);
		printf("PRODUCER: myMutex unlocked\n"); fflush(stdout);
	}

	printf("PRODUCER THREAD ENDED\n"); fflush(stdout);
	return NULL;
}

/*
 * Start routine for a thread to print out myCount
 */
void* consume(void* argument) {
	printf("CONSUMER THREAD CREATED\n"); fflush(stdout);

	while (myCount < MAX) {
		pthread_mutex_lock(&myMutex);
		printf("CONSUMER: myMutex locked\n"); fflush(stdout);

		while (!canPrint) {
			printf("CONSUMER: waiting on myCond2\n"); fflush(stdout);
			pthread_cond_wait(&myCond2, &myMutex);
		}

		printf("CONSUMER: %d -> %d\n", myCount-1, myCount); fflush(stdout);
		canPrint = 0;

		pthread_cond_signal(&myCond1);
		printf("CONSUMER: signaled myCond1\n"); fflush(stdout);

		pthread_mutex_unlock(&myMutex);
		printf("CONSUMER: myMutex unlocked\n"); fflush(stdout);
	}

	printf("CONSUMER THREAD ENDED\n"); fflush(stdout);
	return NULL;
}
