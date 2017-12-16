#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"
#include <signal.h>

void Init(void) {
	// Create ready Queue and Waiting Queue
//    printf("[Init.c:10]\t Init start\n");
    runningThread = (Thread*)malloc(sizeof(Thread));
    runningThread->tid = 0;
    
    ReadyQ = (Queue*)malloc(sizeof(Queue));
    WaitQ = (Queue*)malloc(sizeof(Queue));
	InitQ(ReadyQ, 1);
	InitQ(WaitQ, 2);
	Thread* ReadyQHead = NULL;
	Thread*	ReadyQTail = NULL;
	Thread* WaitQHead = NULL;
	Thread*	WaitQTail = NULL;
    
    signal(SIGUSR2, __thread_wait_handler);
    signal(SIGUSR1, __sigusr1_handler);

}
