#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

int	RunScheduler(void) {
    printf("[Scheduler:8]\t Start\n");
    while (1) {
		if (IsQEmpty(ReadyQ)){
            printf("[Scheduler:12]\t ReadyQ is Empty\n");
			sleep(TIMESLICE);
        }
		else {
            __ContextSwitch(runningThread, ReadyQHead);
			sleep(TIMESLICE);   
		}
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread) {
	// 1.stop current running thread
	//pthread_kill(runningThread->tid,)
    if(pCurThread->tid != 0){
    	_thread_wait_handler(pCurThread);
	// 2.enqueue
		Enqueue(ReadyQ, pCurThread);
    }
	
	// 3.dequeue target thread & execute target thread
    Dequeue(ReadyQ);
    runningThread = pNewThread;
    __thread_wakeup(pNewThread);
}

