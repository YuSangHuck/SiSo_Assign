#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

#include <signal.h>
//#define NULL 0


void* __wrapperFunc(void* arg) {
    WrapperArg* pArg = (WrapperArg*)arg;
    WrapperArg myArg = *pArg;
    void* ret;
    int retSig;
    void* funcPtr = myArg.funcPtr;
    void* funcArg = myArg.funcArg;

    
//    printf("tid : (%d)\t funcArg : (%d)\n\
\t\t\t *funcArg : (%d)\n", thread_self(), funcArg, *(int*)funcArg);
    sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
//    printf(" pthread(%d) wait signal\n",pthread_self());
	sigwait(&set, &retSig);

//    printf(" pthread(%d) pass sigwait\n",pthread_self());

	__thread_wait_handler(0);
//	void* funcPtr = pArg->funcPtr;
//	void* funcArg = pArg->funcArg;
//    printf(" wrapperFunc start\n");
    ret = (*(void*(*)(void*))funcPtr)(funcArg);
//    printf(" wrapperFunc finish\n");
	return ret;
}


int thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
//    printf("thread_create start at (%d)\n", thread_self());
	// pthread_create
    WrapperArg wrapperArg;
//    printf("&wrapperArg : (%d)\n", &(wrapperArg));
	wrapperArg.funcPtr = start_routine;
	wrapperArg.funcArg = arg;
    if(arg == 0){
        pthread_create(thread, NULL, wrapperArg.funcPtr, wrapperArg.funcArg);
        return 0;
    }
//    printf("(%d)\n",*(int*)arg);
//    printf("(%d)\n",*(int*)wrapperArg.funcArg);
    pthread_create(thread, NULL, __wrapperFunc, &wrapperArg);
	// Allocate & Init TCB
	Thread* threadPtr = (Thread*)malloc(sizeof(Thread));
	pthread_cond_t readyCond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t readyMutex = PTHREAD_MUTEX_INITIALIZER;
	//threadPtr->status = THREAD_STATUS_BLOCKED;
	threadPtr->status = THREAD_STATUS_READY;
	threadPtr->pExitCode = 0;
	threadPtr->bRunnable = 0;
	threadPtr->pPrev = NULL;
	threadPtr->pNext = NULL;
	threadPtr->parentTid = thread_self();
	threadPtr->tid = *thread;
    threadPtr->readyCond = readyCond;
    threadPtr->readyMutex = readyMutex;

	// Move TCB to ReadyQ
	Enqueue(ReadyQ,threadPtr);

	// send SIGUSR1 to child
    usleep(1);
    pthread_kill(*thread, SIGUSR1);
//    printf(" signal to pthread(%d)\n",*thread);
	// Return
//	printf("thread_create finish at (%d)\n", thread_self());
}

int thread_join(thread_t thread, void **retval) {

}

int thread_suspend(thread_t tid) {

}

int	thread_resume(thread_t tid) {

}

thread_t thread_self() {
	return pthread_self();
}

int thread_exit(void * retval) {
	//Thread* TCB;
	//TCB = getRunningThread();
	//pthread_mutex_lock(&(TCB->readyMutex), &(TCB->readyCond));
	//pthread_cond_signal(&(TCB->readyCond));
	//pthread_mutex_unlock(&(TCB->readyMutex));

	// thread_exit을 호출한 TCB.pExitCode를 1로설정
	Thread* TCB;
	TCB = runningThread;
	TCB->pExitCode = retval;
	return 0;
}

