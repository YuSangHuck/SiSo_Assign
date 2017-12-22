#include "_Thread.h"
#include <stdio.h>
#include "Init.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Scheduler.h" 

/* linked list function */

void display_list(Message* head)
{
    while(head != NULL){
	printf("%d, %s\n", head->type, head->data);
	head = head->pNext;
    }
}

//void delete_target(long type);
/* ready queue function body */

int is_empty_rq(ready_queue* r_queue)
{

    int empty = 0;

    if(r_queue != NULL){
	if(r_queue->ready_current_count == 0){
	    empty = 1;
	}

    }

    return empty;
}


ready_queue* create_rq()
{

    ready_queue *pReturn = (ready_queue *)malloc(sizeof(ready_queue));

    memset(pReturn, 0, sizeof(ready_queue));
    return pReturn;
}

void r_enqueue(ready_queue* rq_ptr, Thread* new_thread_ptr)
{
    Thread *r_node = NULL;

    r_node = new_thread_ptr;
    r_node->pNext = NULL;
    r_node->pPrev = NULL;

    if(is_empty_rq(rq_ptr) == 0)
    {
	//queue is not empty
	rq_ptr->ready_tail_ptr->pNext = r_node;
	r_node->pPrev = rq_ptr->ready_tail_ptr;
	rq_ptr->ready_tail_ptr = r_node;
    }
    else
    {

	rq_ptr->ready_head_ptr = r_node;
	rq_ptr->ready_tail_ptr = r_node;
    }
    rq_ptr->ready_current_count++;
    ReadyQHead = rq_ptr->ready_head_ptr;
    ReadyQTail = rq_ptr->ready_tail_ptr;

}

Thread* r_dequeue(ready_queue* r_queue)
{

    Thread* pReturn = NULL;
    if(is_empty_rq(r_queue) == 0)
    {
	r_queue->ready_current_count--;
	if(is_empty_rq(r_queue) == 1){
	    pReturn = r_queue->ready_head_ptr;
	    r_queue->ready_head_ptr = NULL;
	    r_queue->ready_tail_ptr = NULL;
	    return pReturn;
	}
	pReturn = r_queue->ready_head_ptr;
	r_queue->ready_head_ptr = r_queue->ready_head_ptr->pNext;

	r_queue->ready_head_ptr->pPrev = NULL;
	pReturn->pNext = NULL;
    }
    ReadyQHead = r_queue->ready_head_ptr;
    ReadyQTail = r_queue->ready_tail_ptr;
    return pReturn;
}

void display_rq(ready_queue* r_queue)
{ 
    Thread *r_node = NULL;
    if(r_queue != NULL){
	printf("ready current node : %d\n", r_queue->ready_current_count);
	r_node = r_queue->ready_head_ptr; 
	while(r_node != NULL){
	    printf("[%d] ", r_node->pid);
	    r_node =r_node->pNext;
	}
	printf("\n");
    }
}

int is_tid_in_rq(thread_t tid, ready_queue* rq_ptr)
{
    Thread* search_ptr = NULL;
    for(search_ptr = rq_ptr->ready_head_ptr; (search_ptr != rq_ptr->ready_tail_ptr || search_ptr == NULL); search_ptr=search_ptr->pNext){
	if(search_ptr->pid == tid)
	    return 1;
    }
    return 0;

}
Thread *remove_thread_rq(thread_t tid, ready_queue* rq_ptr)
{
    Thread *search_ptr = NULL;
    for(search_ptr = rq_ptr->ready_head_ptr; (search_ptr != rq_ptr->ready_tail_ptr) ; search_ptr=search_ptr->pNext){
	if(search_ptr->pid == tid)
	    break;
    }

    if(search_ptr->pid != tid)
	return NULL;



    if((rq_ptr->ready_current_count == 1) || (search_ptr->pPrev == NULL)){
	r_dequeue(rq_ptr);
    }
    else if(search_ptr->pNext == NULL)
    {
	rq_ptr->ready_tail_ptr = search_ptr->pPrev;
	search_ptr->pPrev = NULL;
	rq_ptr->ready_tail_ptr->pNext = NULL;
    }
    else
    {
	search_ptr->pPrev->pNext = search_ptr->pNext;
	search_ptr->pNext->pPrev = search_ptr->pPrev;
	search_ptr->pNext = NULL;
	search_ptr->pPrev = NULL;
    }

    ReadyQHead = rq_ptr->ready_head_ptr;
    ReadyQTail = rq_ptr->ready_tail_ptr;
    return search_ptr;
}


/* waiting queue function body */


int is_empty_wq(waiting_queue* w_queue)
{

    int empty = 0;

    if(w_queue != NULL){
	if(w_queue->waiting_current_count == 0){
	    empty = 1;
	}

    }

    return empty;
}

waiting_queue* create_wq()
{

    waiting_queue *pReturn = (waiting_queue *)malloc(sizeof(waiting_queue));

    memset(pReturn, 0, sizeof(waiting_queue));
    return pReturn;
}

void w_enqueue(waiting_queue* w_queue, Thread* new_thread_ptr)
{

    Thread *w_node = NULL;

    w_node = new_thread_ptr;
    w_node->pNext = NULL;
    w_node->pPrev = NULL;

    if(is_empty_wq(w_queue) == 0)
    {
	//queue is not empty
	w_queue->waiting_tail_ptr->pNext = w_node;
	w_node->pPrev = w_queue->waiting_tail_ptr;
	w_queue->waiting_tail_ptr = w_node;
    }
    else
    {
	w_queue->waiting_head_ptr = w_node;
	w_queue->waiting_tail_ptr = w_node;
    }
    w_queue->waiting_current_count++;
    WaitQHead = w_queue->waiting_head_ptr;
    WaitQTail = w_queue->waiting_tail_ptr;
}

Thread* w_dequeue(waiting_queue* w_queue)
{

    Thread* pReturn = NULL;
    if(is_empty_wq(w_queue) == 0)
    {
	w_queue->waiting_current_count--;
	if(is_empty_wq(w_queue) == 1){
	    pReturn = w_queue->waiting_head_ptr;
	    w_queue->waiting_head_ptr = NULL;
	    w_queue->waiting_tail_ptr = NULL;
	    return pReturn;
	}
	pReturn = w_queue->waiting_head_ptr;
	w_queue->waiting_head_ptr = w_queue->waiting_head_ptr->pNext;

	w_queue->waiting_head_ptr->pPrev = NULL;
	pReturn->pNext = NULL;
    }
    WaitQHead = w_queue->waiting_head_ptr;
    WaitQTail = w_queue->waiting_tail_ptr;
    return pReturn;
}

void display_wq(waiting_queue* w_queue)
{ 
    Thread *w_node = NULL;
    if(w_queue != NULL){
	printf("waiting current node : %d\n", w_queue->waiting_current_count);
	w_node = w_queue->waiting_head_ptr; 
	while(w_node != NULL){
	    printf("[%d] ", w_node->pid);
	    w_node =w_node->pNext;
	}
	printf("\n");
    }
}

Thread *remove_thread_wq(thread_t tid, waiting_queue* wq_ptr)
{
    Thread *search_ptr = NULL;
    for(search_ptr = wq_ptr->waiting_head_ptr; (search_ptr != wq_ptr->waiting_tail_ptr) || (search_ptr == NULL); search_ptr=search_ptr->pNext){
	if(search_ptr->pid == tid)
	    break;
    }

    if(search_ptr == NULL)
	return NULL;


    if((wq_ptr->waiting_current_count == 1) || (search_ptr->pPrev == NULL)){
	w_dequeue(wq_ptr);
    }
    else if(search_ptr->pNext == NULL)
    {
	wq_ptr->waiting_tail_ptr = search_ptr->pPrev;
	search_ptr->pPrev = NULL;
	wq_ptr->waiting_tail_ptr->pNext = NULL;
    }
    else
    {
	search_ptr->pPrev->pNext = search_ptr->pNext;
	search_ptr->pNext->pPrev = search_ptr->pPrev;
	search_ptr->pNext = NULL;
	search_ptr->pPrev = NULL;
    }

    WaitQHead = wq_ptr->waiting_head_ptr;
    WaitQTail = wq_ptr->waiting_tail_ptr;
    return search_ptr;
}


/* thread_function */

int thread_create(thread_t *thread_id, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    //clone -> pid allocate
    //ready queue enqueue
    void *stack = malloc(STACK_SIZE);
    if(stack == 0)
    {
	perror("malloc : could not allocate stack");
	exit(1);
    }

    *thread_id = clone((int (*)(void *))start_routine, (char *)stack+STACK_SIZE, FLAGS, arg);

    kill(*thread_id, SIGSTOP);

    Thread *m_thread = (Thread *)malloc(sizeof(Thread));

    m_thread->pid = *thread_id;
    m_thread->status = THREAD_STATUS_READY;

    r_enqueue(rq, m_thread);

}

/*
int thread_join(thread_t thread, void **retval)
{
    //readyqueue에서 parent 찾아서 빼낸다.
    //찾은 부모를 waiting queue에 넣는다.
    // /proc/[pid]/stat 파일에서 상태값을 계속 읽어온다.
    //z를 읽으면 부모 쓰레드를 웨이팅큐에서 디큐한다.
    //그후 레디큐에 인큐한다.
   
    printf("thread_join target : %d\n", thread);

    Thread* parent_thread;
    char address[40] = {0};
    char str[40]={0};
    sprintf(address, "/proc/%d/stat", thread);
    FILE* fp = fopen(address, "r");
    fgets(str, 40, fp);
    printf("parent pid finding...: %s \n", str);
    int i;
    for(i = 0;str[i]!=0;i++){
	if(str[i] == ')'){
	    break;
	}
    }
    fseek(fp, i+4, SEEK_SET);
    thread_t ppid;
    fscanf(fp, "%d", &ppid);
    printf("parent pid : %d\n", ppid);
    
    //부모 빼내기.
    if((parent_thread = remove_thread_rq(ppid, rq)) == NULL){
	*join_flag = 1;
	
    }
    else{
	w_enqueue(wq, parent_thread);
    }
    fclose(fp);
    int fd;
    memset(address, 0, 40);
    sprintf(address, "/proc/%d/stat", thread);

    while(1){
    fd = open(address, O_RDONLY);
    if(fd < 0) perror("open error\n");
    
    read(fd, str, 20);

    for(i=0;str[i]!=0;i++){
	if(str[i] == ')'){
	    break;
	}
    }
    
	if(str[i+2] != 'Z'){
	    lseek(fd, (off_t)0, SEEK_SET);
	    close(fd);
	}
	else{
	    // Z
	    close(fd);
	    Thread* temp_thread;
	    if(rq->ready_current_count == 2){
		rq->ready_current_count--;
		temp_thread = remove_thread_rq(thread, rq);
	    }
	    else if((temp_thread = remove_thread_rq(thread, rq)) == NULL){
		*join_flag = 2;	
	    }
	    else{
		rq->ready_current_count--;
		free(temp_thread);
	    }
	    printf("reap succeed\n");
	    memset(address, 0, 40);
	    memset(str, 0 , 40);
	    break;
	} 
    }
    

    parent_thread = w_dequeue(wq);
    r_enqueue(rq, parent_thread);
    display_rq(rq);

    free(join_flag);
}
*/

int thread_suspend(thread_t tid)
{
    //needed to search target thread using thread id
    //if found, move target thread to waiting queue
    //and update status 

    printf("thread_suspend : %d\n", thread_self());
    kill(tid, SIGSTOP); 
    Thread* target_thread = remove_thread_rq(tid, rq);
    target_thread->status = THREAD_STATUS_BLOCKED;
    w_enqueue(wq, target_thread);
    display_wq(wq);
    return 0;
}


int thread_resume(thread_t tid)
{
    //set thread status to ready
    //remove tcb from waiting queue
    //insert into ready queue

    Thread* target_thread;

    if((target_thread = remove_thread_wq(tid, wq)) == NULL ){
	return -1;
    }
    printf("thread_resume! waiting_current_count : %d\n", wq->waiting_current_count);
    if(target_thread == NULL)
    {
	printf("not exist target thread\n");
	return -1;
    }
    target_thread->status = THREAD_STATUS_READY;
    r_enqueue(rq, target_thread);

    printf("thread resume! ready_current_count : %d\n", rq->ready_current_count);
    return 0;
}


int thread_cancel(thread_t tid)
{
    //sigkill signal to target thread
    //remove target thread from ready or waiting queue
    //delocate TCB structure
    Thread *target_thread;
    kill(tid, SIGKILL);
    if((target_thread = remove_thread_rq(tid, rq)) != NULL)
    {
	printf("target thread %d canceled in ready queue\n", tid);
    }
    else if((target_thread = remove_thread_wq(tid, wq)) != NULL)
    {
	printf("target thread %d canceled in waiting queue\n", tid);
	wq->waiting_current_count--;
    }
    else{
	printf("not exist target thread!\n");
	return -1;
    }
    printf("cancel : %d\n", rq->ready_current_count);
    free(target_thread);
    return 0;
}


thread_t thread_self()
{
    return getpid();
}	

