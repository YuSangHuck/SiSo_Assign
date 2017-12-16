#ifndef __Queue__
#define __Queue__
#include "Thread.h"

typedef struct _Queue {
	Thread* front;
	Thread* rear;
	int count;
	int type;	// ReadyQ : 1, WaitQ : 2
} Queue;

void InitQ(Queue* queue, int type);
BOOL IsQEmpty(Queue* queue);
void Enqueue(Queue* queue, Thread* new);
Thread* Dequeue(Queue*);
Thread* SearchTCB(Queue*, thread_t tid);

#endif
