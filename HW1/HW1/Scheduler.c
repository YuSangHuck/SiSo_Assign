#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

int	RunScheduler(void) {
    printf("[Scheduler:8]\t Start\n");
    sleep(5);
    Thread thread;
    while (1) {
		if (IsReadyQEmpty()){
            printf("[Scheduler:12]\t ReadyQ is Empty\n");
			sleep(TIMESLICE);
        }
		else {
            thread = ReadyQDequeue();
            printf("[scheduler:17]\t dequeue tid %d\n",thread.tid);
            __ContextSwitch(runningThread, &thread);
            runningThread = &thread;
			sleep(TIMESLICE);   
		}
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread) {
	// 1.stop current running thread
	//pthread_kill(runningThread->tid,)
    usleep(1);
    if(pCurThread->tid != 0){
        printf("[ContestSwitch]\t thread_wait_handler()\n");
//    	__thread_wait_handler(pCurThread);
    	_thread_wait_handler(pCurThread);
    }
	
	// 2.execute target thread
    printf("[ContestSwitch]\t before thread_wakeup\n");
	__thread_wakeup(pNewThread);
    printf("[ContestSwitch]\t after thread_wakeup\n");
    
    // 3.enqueue
//    ReadyQEnqueue(*pCurThread);
}

