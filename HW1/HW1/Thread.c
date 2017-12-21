#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

#include <signal.h>
//#define NULL 0


void* __wrapperFunc(void* arg) {
    void* ret;
    int retSig;
    WrapperArg* pArg = (WrapperArg*)arg;
    WrapperArg myArg = *pArg;

//    printf("&wrapperArg : (%d)\n", arg);
//    printf("tid : (%d)\t arg : (%d)\n\
\t\t\t (WrapperArg*)arg : (%d)\n\
\t\t\t ((WrapperArg*)arg)->funcArg : (%d)\n\
\t\t\t *((WrapperArg*)arg)->funcArg : (%d)\n", thread_self(), arg, pArg, pArg->funcArg, (pArg->funcArg));
    sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigwait(&set, &retSig);

	__thread_wait_handler(0);
    void* funcPtr = myArg.funcPtr;
    void* funcArg = myArg.funcArg;
//	void* funcPtr = pArg->funcPtr;
//	void* funcArg = pArg->funcArg;
    ret = (*(void*(*)(void*))funcPtr)(funcArg);
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
    usleep(100);
	// Allocate & Init TCB
	Thread* threadPtr = (Thread*)malloc(sizeof(Thread));
	pthread_cond_t readyCond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t readyMutex = PTHREAD_MUTEX_INITIALIZER;
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
//    printf("signal\n");
    pthread_kill(*thread, SIGUSR1);
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

