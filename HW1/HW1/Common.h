#ifndef _COMMON_H_
#define _COMMON_H_
#include "Init.h"
#include "Queue.h"
#include "Scheduler.h"
#include "Thread.h"

Thread* runningThread;
void __thread_wait_handler(Thread* thread);
void __thread_wakeup(Thread* thread);
#endif