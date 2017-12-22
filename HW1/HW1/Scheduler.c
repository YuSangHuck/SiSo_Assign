#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"
#include "signal.h"

int	RunScheduler(void) {
    printf("[Scheduler:9]\t Start\n");
    while (1) {
        printf("[Scheduler:15]\t # of ReadyQ is (%d)\n", ReadyQ->count);
		if (IsQEmpty(ReadyQ)){
        }
		else {
            __ContextSwitch(runningThread, ReadyQHead);
//            ShowQueueBRunnable(ReadyQ);
		}
        sleep(TIMESLICE);   
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread) {
    printf("start\n");
	// 1.stop current running thread
    //pthread_kill(runningThread->tid,)
    if(pCurThread->tid != 0){
        printf("check\n");
    	//_thread_wait_handler(pCurThread);
        pthread_kill(pCurThread->tid, SIGUSR2);
	// 2.enqueue
//        printf("enqueue TCB check bRunnable(%d)\n", pCurThread->bRunnable);
		Enqueue(ReadyQ, pCurThread);
    }
	
	// 3.dequeue target thread & execute target thread
    printf("check2\n");
    Dequeue(ReadyQ);
    runningThread = pNewThread;
    __thread_wakeup(pNewThread);
}

