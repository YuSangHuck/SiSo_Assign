#include "Common.h"

void __sigusr1_handler(){
}

void __thread_wait_handler(int signo) {
//    printf("__thread_wait_handler start\n");
	Thread* pTh;
	if((pTh=SearchTCB(ReadyQ,thread_self())) == NULL){
        return;
    }

    pthread_mutex_lock(&(pTh->readyMutex));
    pTh->bRunnable = 0; // dlatl qkdvus
//    printf("tid : (%d), pTh->bRunnable (%d)\n", pTh->tid, pTh->bRunnable);
	while (pTh->bRunnable == 0){
		pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
    }
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_wakeup(Thread* thread) {
//    printf("__thread_wakeup start, tid : (%d)\n", thread->tid);
	pthread_kill(thread->tid, SIGUSR1);
	pthread_mutex_lock(&(thread->readyMutex));
	thread->bRunnable = 1;
	pthread_cond_signal(&(thread->readyCond));
	pthread_mutex_unlock(&(thread->readyMutex));
    printf("wakeup called at %d\n", thread->tid);
}

