#include "_Thread.h"
#include "Init.h"
#include "Scheduler.h"

void context_switch(int signum);
void config_timer();

void Init(void){
    _InitMsgQueue();
    rq = create_rq();
    wq = create_wq();
    WaitQHead = wq->waiting_head_ptr;
    ReadyQHead = rq->ready_head_ptr;
    config_timer();
}

void config_timer()
{
	struct sigaction act;
	act.sa_handler = context_switch;
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, NULL);

}

