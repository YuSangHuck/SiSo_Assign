#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"
#include "signal.h"

int	RunScheduler(void) {
    printf("[Scheduler:8]\t Start\n");
    while (1) {
		if (IsQEmpty(ReadyQ)){
            printf("[Scheduler:12]\t # of ReadyQ is (%d)\n", ReadyQ->count);
			sleep(TIMESLICE);
        }
		else {
            printf("[Scheduler:15]\t # of ReadyQ is (%d)\n", ReadyQ->count);
            __ContextSwitch(runningThread, ReadyQHead);
			sleep(TIMESLICE);   
		}
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread) {
	// 1.stop current running thread
	//pthread_kill(runningThread->tid,)
    if(pCurThread->tid != 0){
    	//_thread_wait_handler(pCurThread);
        pthread_kill(runningThread, SIGUSR2);
	// 2.enqueue
		Enqueue(ReadyQ, pCurThread);
    }
	
	// 3.dequeue target thread & execute target thread
    Dequeue(ReadyQ);
    runningThread = pNewThread;
    __thread_wakeup(pNewThread);
}

