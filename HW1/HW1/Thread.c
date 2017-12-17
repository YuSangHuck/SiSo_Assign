#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

#include <signal.h>
//#define NULL 0


void* __wrapperFunc(void* arg) {
    void* ret;
    WrapperArg* pArg = (WrapperArg*)arg;
    sigset_t set;
    int retSig;
    printf("tid : (%d), &arg : (%d), arg: (%d)\n", (int)thread_self(), &(pArg->funcArg), pArg->funcArg);
//    printf("@@@@@@@@@@@@@@@@ check arg %d @@@@@@@@@@@@@@@@\n",pArg->funcArg);

//    printf("[wrapper:18]\t Setting the sigset\n");
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
//    printf("[wrapper:21]\t sigwait SIGUSR1 : (%d)\n",(int)thread_self());
//    printf("[before]\t sigwait thread id : (%d)\n\
\t\t sigwait thread &arg : %d\n\
\t\t sigwait thread arg : %d\n", (int)thread_self(), &(pArg->funcArg), pArg->funcArg);
	sigwait(&set, &retSig);
//    printf("[after]\t\t ready thread id : (%d)\n\
\t\t ready thread &arg : %d\n\
\t\t ready thread arg : %d\n", (int)thread_self(), &(pArg->funcArg), pArg->funcArg);

//    printf("[wrapper:24]\t wait_handler called\n");
	__thread_wait_handler(0);
//    printf("[wrapper:25]\t ready thread arg : %d\n", pArg->funcArg);
	void* funcPtr = pArg->funcPtr;
	void* funcArg = pArg->funcArg;
//    printf("@@@@@@@@@@@@@@@@ recheck arg %d @@@@@@@@@@@@@@@@\n",funcArg);
    printf("[wrapper:25]\t start thread id : (%d)\n\t\t start thread arg : %d\n", (int)thread_self(), pArg->funcArg);
    
	ret = (*(void*(*)(void*))funcPtr)(funcArg);
	return ret;
}


int thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
	// pthread_create
//    printf("[th_create:38]\t pthread_create()\n");
    WrapperArg wrapperArg;
    printf("&wrapperArg : (%d)\n", &(wrapperArg.funcArg));
	wrapperArg.funcPtr = start_routine;
	wrapperArg.funcArg = arg;
//    printf("@@@@@@@@@@@@@@@@ input arg %d @@@@@@@@@@@@@@@@\n",wrapperArg.funcArg);
    if(arg == 0){
        pthread_create(thread, NULL, wrapperArg.funcPtr, arg);
        return 0;
    }
	pthread_create(thread, NULL, __wrapperFunc, &wrapperArg);
//    printf("[th_create:43]\t pthread_create tid : %d\n\t\t pthread_self : %d\n", *thread, pthread_self());
    usleep(1);
//    sleep(1);
	// Allocate & Init TCB
//    printf("[th_create:46]\t Allocate & Init TCB\n");
	Thread* threadPtr = (Thread*)malloc(sizeof(Thread));
//    printf("%d",*thread);
//    Thread tempThread = SearchReadyTCB(*thread);
//    memcpy(threadPtr, &tempThread, sizeof(Thread));
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
//    if(IsReadyQEmpty())
//        printf("[th_create:58]\t ReadyQ is empty\n");
//    printf("[th_create:59]\t Thread : %d\n\t\t ENQUEUE, tid : %d\n", threadPtr, threadPtr->tid);
	Enqueue(ReadyQ,threadPtr);
//    if(!IsReadyQEmpty())
//        printf("[th_create:57]\t ReadyQ is not empty\n");

	// send SIGUSR1 to child
    printf("[th_create:79]\t SIGNAL SIGUSR1 to (%d)\n",(int)*thread);
	pthread_kill(*thread, SIGUSR1);
	// Return
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

