#include "Init.h"
#include "_Thread.h"
#include "Scheduler.h"

void context_switch(int signum)
{
	Thread* new_thread = r_dequeue(rq);

	if(current_thread == NULL){
		current_thread = new_thread;
		if(current_thread == NULL){
			printf("main_thread(%d) only running\n", thread_self());
			alarm(TIMESLICE);
			return;
		}
		new_thread = NULL;
		kill(current_thread->pid, SIGCONT);
		alarm(TIMESLICE);
		return;
	}
	if(new_thread == NULL) {
		printf("only one child thread(%d) running\n", current_thread->pid);
		alarm(TIMESLICE);
		return;
	}

	_ContextSwitch(current_thread->pid, new_thread->pid);
	current_thread = new_thread;
}

void run(Thread* parm_thread)
{
	if(is_empty_rq(rq) == 0){
		parm_thread  = r_dequeue(rq);
		parm_thread->status = THREAD_STATUS_RUN;
		current_thread = parm_thread;
		printf("current_thread pid : %d\n", current_thread->pid);
		kill(current_thread->pid, SIGCONT);
	}
}

int RunScheduler( void )
{
	Thread* init_thread;
	alarm(TIMESLICE);
	run(init_thread);
	while(1);
}


void _ContextSwitch(int curpid, int tpid)
{

	kill(curpid, SIGSTOP);
	current_thread->status = THREAD_STATUS_READY;
	r_enqueue(rq, current_thread);

	kill(tpid, SIGCONT);
	alarm(TIMESLICE);
	//printf("contextswitch! running thread tid : %d\n", tpid);
	display_rq(rq);
}

