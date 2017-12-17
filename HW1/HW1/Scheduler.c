#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

int	RunScheduler(void) {
    printf("[Scheduler:8]\t Start\n");
//    sleep(5);
//    Thread* headPtr;
    while (1) {
		if (IsQEmpty(ReadyQ)){
            printf("[Scheduler:12]\t ReadyQ is Empty\n");
			sleep(TIMESLICE);
        }
		else {
//            headPtr = ReadyQHead;
//            Dequeue(ReadyQ);
//            __ContextSwitch(runningThread, headPtr);
            __ContextSwitch(runningThread, ReadyQHead);
//            runningThread = headPtr;
			sleep(TIMESLICE);   
		}
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread) {
	// 1.stop current running thread
	//pthread_kill(runningThread->tid,)
//    usleep(1);
    if(pCurThread->tid != 0){
//        printf("[ContestSwitch]\t stop current running thread\n");
//    	__thread_wait_handler(pCurThread);
    	_thread_wait_handler(pCurThread);
        Enqueue(ReadyQ, pCurThread);
    }
	
	// 2.dequeue target thread & execute target thread
    Dequeue(ReadyQ);
    runningThread = pNewThread;
//    printf("[ContestSwitch]\t dequeue & execute target thread\n");
//    printf("[ContestSwitch]\t dequeue tid %p\n", runningThread->tid);
    __thread_wakeup(pNewThread);
    
    // 3.enqueue
//    printf("[ContestSwitch]\t enqueue stop thread\n");
//    Enqueue(ReadyQ, pCurThread);
//    printf("[ContestSwitch]\t queue count check : %d\n",ReadyQ->count);
}

