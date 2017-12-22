#define _GNU_SOURCE
#include "MsgQueue.h"
#include "Thread.h"
#include <sched.h>
#include <signal.h>
#include <memory.h>
#include <malloc.h>

#define FLAGS (CLONE_VM|CLONE_SIGHAND|CLONE_FS|CLONE_FILES|SIGCHLD)
#define STACK_SIZE (1024*64)
#define PATH_SIZE (10)
#define STR_SIZE (20)

Thread* current_thread;
// linked list for msg list
// need to make function that target node can be removed.
// need to make function that insert the target
void display_list(Message* head);
void delete_target(long type);
/* head and tail pointers for ready queue */ 
typedef struct ready_queue{
    int ready_current_count;
    Thread* ready_head_ptr;
    Thread* ready_tail_ptr;
} ready_queue;

/* head and tail pointers for waiting queue */
typedef struct waiting_queue{
    int waiting_current_count;
    Thread* waiting_head_ptr;
    Thread* waiting_tail_ptr;
} waiting_queue;

ready_queue* rq;
waiting_queue* wq;


/* linked list function(remove) declared */

Thread *remove_thread_rq(thread_t tid, ready_queue* rq_ptr);

/* queue function declared */


int is_empty_rq(ready_queue* r_queue);
ready_queue* create_rq();
void r_enqueue(ready_queue* rq_ptr, Thread* new_thread_ptr);
Thread* r_dequeue(ready_queue* r_queue);
void display_rq(ready_queue* r_queue);

int is_empty_wq(waiting_queue* w_queue);
waiting_queue* create_wq();
void w_enqueue(waiting_queue* w_queue, Thread* new_thread_ptr);
Thread* w_dequeue(waiting_queue* w_queue);
void display_wq(waiting_queue* w_queue);
