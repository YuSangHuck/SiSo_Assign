#ifndef __Queue__
#define __Queue__
#include "Thread.h"

//typedef struct _Queue {
//	Thread* head;
//	Thread* tail;
//	int count;
//} Queue;

void InitReadyQ();
void InitWaitQ();
BOOL IsReadyQEmpty();
BOOL IsWaitQEmpty();
void ReadyQEnqueue(Thread new);
void WaitQEnqueue(Thread new);
Thread ReadyQDequeue();
Thread WaitQDequeue();
Thread* SearchReadyTCB(thread_t tid);
Thread* SearchWaitTCB(thread_t tid);

#endif
