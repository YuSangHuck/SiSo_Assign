#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"
#include <stdlib.h>
#include <string.h>

void InitQ(Queue* queue, int type) {
	queue->front = NULL;
	queue->rear = NULL;
	queue->count = 0;
	queue->type = type;
}

BOOL IsQEmpty(Queue* queue) {
	return !(queue->count);
}

void Enqueue(Queue* queue, Thread* new) {
	if (queue->type == 1) {	// ReadyQ
		if (IsQEmpty(queue)) {
			queue->front = new;
			queue->front->pPrev = NULL;
			queue->front->pNext = NULL;
			queue->rear = new;
			queue->rear->pPrev = NULL;
			queue->rear->pNext = NULL;
			ReadyQHead = queue->front;
			ReadyQTail = queue->rear;
		}
		else {
			queue->rear->pNext = new;
			new->pPrev = queue->rear;
			new->pNext = NULL;
			queue->rear = new;
			ReadyQTail = new;
		}
	}
	else if (queue->type == 2) {	// WaitQ
		if (IsQEmpty(queue)) {
			queue->front = new;
			queue->front->pPrev = NULL;
			queue->front->pNext = NULL;
			queue->rear = new;
			queue->rear->pPrev = NULL;
			queue->rear->pNext = NULL;
			WaitQHead = queue->front;
			WaitQTail = queue->rear;
		}
		else {
			queue->rear->pNext = new;
			new->pPrev = queue->rear;
			new->pNext = NULL;
			queue->rear = new;
			WaitQTail = new;
		}
	}
	else
		return;
	queue->count += 1;
	return;
}

Thread* Dequeue(Queue* queue) {
	Thread* temp;
	if (queue->type == 1) {	// ReadyQ
		if (IsQEmpty(queue)) {
			temp = (Thread*)malloc(sizeof(Thread));
			temp->tid = -1;
		}
		else if (queue->count == 1) {
			temp = queue->front;
			queue->front = NULL;
			queue->rear = NULL;
		}
		else {
			temp = queue->front;
			queue->front = queue->front->pNext;
			queue->front->pPrev = NULL;
		}
		ReadyQHead = queue->front;
		ReadyQTail = queue->rear;
	}
	else if (queue->type == 2) {	// WaitQ
		if (IsQEmpty(queue)) {
			temp = (Thread*)malloc(sizeof(Thread));
			temp->tid = -1;
		}
		else if (queue->count == 1) {
			temp = queue->front;
			queue->front = NULL;
			queue->rear = NULL;
		}
		else {
			temp = queue->front;
			queue->front = queue->front->pNext;
			queue->front->pPrev = NULL;
		}
		WaitQHead = queue->front;
		WaitQTail = queue->rear;
	}
	else {
		temp = (Thread*)malloc(sizeof(Thread));
		temp->tid = -2;
	}
	queue->count -= 1;
	return temp;
}

Thread* SearchTCB(Queue* queue, thread_t tid) {
	Thread* cursor = queue->front;
	while (cursor != NULL) {
		if (cursor->tid == tid)
			return cursor;
		cursor = cursor->pNext;
	}
//	cursor = (Thread*)malloc(sizeof(Thread));
//	cursor->tid = -1;
    cursor = NULL;
	return cursor;
}

void ShowQueue(Queue* queue){
    printf("@Queue status\n");
    printf("front\t\trear\t\tcount\t\ttype\n");
    if(IsQEmpty(queue)){
        printf("%-9d\t%-9d\t%-9d\t%-9d\n\n", queue->front, queue->rear, queue->count, queue->type);
        return;
    }
	Thread* cursor = queue->front;
    printf("%-9d\t%-9d\t%-9d\t%-9d\n", queue->front, queue->rear, queue->count, queue->type);

    printf("@Queue Links\n");
    printf("prev\t\tnow\t\tnext\t\ttid\t\tfront\t\trear\n");
	while (cursor != NULL) {
        printf("%-9d\t%-9d\t%-9d\t%-9d\t", cursor->pPrev, cursor, cursor->pNext, cursor->tid);
        if(queue->type==1 && cursor==ReadyQHead)
            printf("%-9d\n", cursor);
        else if(queue->type==1 && cursor==ReadyQTail)
            printf("\t\t%-9d\n", cursor);
        else
            printf("\n");
		cursor = cursor->pNext;
	}
    printf("\n");
	return ;
}

void ShowQueueBRunnable(Queue* queue){
	Thread* cursor = queue->front;

    printf("@Queue bRunnable\n");
    printf("tid\t\tbRunnable\n");
	while (cursor != NULL) {
        printf("%-9d\t%-9d\n", cursor->tid, cursor->bRunnable);
		cursor = cursor->pNext;
	}
    printf("\n");
	return ;
}

