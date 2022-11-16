#ifndef CS344PROJECT_4_MYCOUNTER_H
#define CS344PROJECT_4_MYCOUNTER_H

#include <pthread.h>

void* produce(void * argument);

void* consume(void * argument);

#endif //CS344PROJECT_4_MYCOUNTER_H
