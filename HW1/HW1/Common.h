#ifndef _COMMON_H_
#define _COMMON_H_
#include "Init.h"
#include "Queue.h"
#include "Scheduler.h"
#include "Thread.h"

int myIndex;
Thread* runningThread;
Queue* ReadyQ;
Queue* WaitQ;
void __sigusr1_handler();
void __thread_wait_handler(int signo);
void __thread_wakeup(Thread* thread);
#endif
