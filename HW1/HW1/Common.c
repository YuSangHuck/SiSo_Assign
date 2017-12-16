#include "Common.h"

void __sigusr1_handler(){
    //printf("[sigusr1_handler]\t SIGUSR1 catched\n");
    //testI++;
}

void _thread_wait_handler(Thread* TCB) {
	//Thread* pTh=TCB;
	
	pthread_mutex_lock(&(TCB->readyMutex));
    while (TCB->bRunnable == 0)
    	pthread_cond_wait(&(TCB->readyCond), &(TCB->readyMutex));
	pthread_mutex_unlock(&(TCB->readyMutex));

}

void __thread_wait_handler(int signo) {
//    printf("[wait_hand:18]\t start\n");
	Thread* pTh;
	pTh = SearchTCB(ReadyQ,thread_self());

    pthread_mutex_lock(&(pTh->readyMutex));
//    printf("[wait_hand:22]\t wait tid %d\n", pTh->tid);
	while (pTh->bRunnable == 0){
//        printf("[wait]\t\t while start\n");
//        printf("[wait]\t\t pth : %d\n\t\t Mutex : %d\n\t\t Cond : %d\n",pTh,pTh->readyMutex,pTh->readyCond);
		pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
//        printf("[wait]\t\t after while\n");
    }
	pthread_mutex_unlock(&(pTh->readyMutex));
}

void __thread_wakeup(Thread* thread) {
//    printf("[wakeup:34]\t tid : %d\n",thread->tid);
//    printf("[wakeup]\t pth : %d\n\t\t Mutex : %d\n\t\t Cond : %d\n",thread,thread->readyMutex,thread->readyCond);
	pthread_mutex_lock(&(thread->readyMutex));
	thread->bRunnable = 1;
	pthread_cond_signal(&(thread->readyCond));
	pthread_mutex_unlock(&(thread->readyMutex));
}

