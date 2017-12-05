#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

int	RunScheduler(void) {
	while (1) {
		if (IsReadyQEmpty())
			sleep(TIMESLICE);
		else {
			Thread dequeue = ReadyQDequeue();
			Thread curThread = thread_self();
			__ContextSwitch(&dequeue, &curThread);
			sleep(TIMESLICE);
		}
	}
}

void __ContextSwitch(Thread* pCurThread, Thread* pNewThread) {
	// 1.stop current running thread
	//pthread_kill(runningThread->tid,)
	__thread_wait_handler(pCurThread);
	
	// 2.execute target thread
	__thread_wakeup(pNewThread);
}

