#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

#include <signal.h>
#define NULL 0


int thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
	// pthread_create
	pthread_t tid;
	WrapperArg wrapperArg;
	wrapperArg.funcPtr = start_routine;
	wrapperArg.funcArg = arg;
	pthread_create(&tid, NULL, __wrapperFunc, &wrapperArg);

	// Allocate & Init TCB
	Thread* threadPtr = (Thread*)malloc(sizeof(Thread));
	TCB_Init(threadPtr);
	threadPtr->parentTid = thread_self();
	threadPtr->tid = tid;

	// Move TCB to ReadyQ
	ReadyQEnqueue(*threadPtr);

	// send SIGUSR1 to child
	pthread_kill(tid, SIGUSR1);

	// Return
}

void* __wrapperFunc(void* arg) {
	void* ret;
	WrapperArg* pArg = (WrapperArg*)arg;
	sigset_t set;
	int retSig;

	// wait until receiving singal(SIGUSR1)
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigwait(&set, &retSig);

	__thread_wait_handler(0);
	void* funcPtr = pArg->funcPtr;
	void* funcArg = pArg->funcArg;
	//ret = (*(void*)(void*)(funcPtr))(funcArg);
	
	ret = (*(void*(*)(void*))funcPtr)(funcArg);
	return ret;
}

void __thread_wait_handler(int signo) {
	Thread pTh;
	pTh = SearchReadyTCB(thread_self());

	pthread_mutex_lock(&(pTh.readyMutex));
	while (pTh.bRunnable == 0)
		pthread_cond_wait(&(pTh.readyCond), &(pTh.readyMutex));
	pthread_mutex_unlock(&(pTh.readyMutex));

}
void __thread_wait_handler(Thread* TCB) {
	//Thread* pTh=TCB;
	
	pthread_mutex_lock(&(TCB->readyMutex));
	while (TCB->bRunnable == 0)
		pthread_cond_wait(&(TCB->readyCond), &(TCB->readyMutex));
	pthread_mutex_unlock(&(TCB->readyMutex));

}
void __thread_wakeup(Thread* thread) {
	pthread_mutex_lock(&(thread->readyMutex));
	thread->bRunnable = 1;
	pthread_cond_signal(&(thread->readyCond));
	pthread_mutex_unlock(&(thread->readyMutex));
}

void TCB_Init(Thread* TCB) {
	TCB->status = THREAD_STATUS_READY;
	TCB->pExitCode = 0;
	TCB->readyCond = PTHREAD_COND_INITIALIZER;
	TCB->bRunnable = 0;
	TCB->readyMutex = PTHREAD_MUTEX_INITIALIZER;
	TCB->parentTid = 0;
	TCB->pPrev = NULL;
	TCB->pNext = NULL;
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

