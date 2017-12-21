#include "Common.h"

void __sigusr1_handler(){
}

void __thread_wait_handler(int signo) {
	Thread* pTh;
	pTh = SearchTCB(ReadyQ,thread_self());

    pthread_mutex_lock(&(pTh->readyMutex));
	while (pTh->bRunnable == 0){
		pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
    }
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_wakeup(Thread* thread) {
	pthread_mutex_lock(&(thread->readyMutex));
	thread->bRunnable = 1;
	pthread_cond_signal(&(thread->readyCond));
	pthread_mutex_unlock(&(thread->readyMutex));
}

