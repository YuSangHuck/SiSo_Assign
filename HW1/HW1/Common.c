#include "Common.h"

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

