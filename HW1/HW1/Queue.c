#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"
#include <stdlib.h>
#include <string.h>

//void InitReadyQ() {
//	Thread* ReadyQHead = NULL;
//	Thread*	ReadyQTail = NULL;
//}
//void InitWaitQ() {
//	Thread* WaitQHead = NULL;
//	Thread*	WaitQTail = NULL;
//}
void InitQ(Queue* queue, int type) {
	queue->front = NULL;
	queue->rear = NULL;
	//queue->next = NULL;
	//queue->prev = NULL;
	queue->count = 0;
	queue->type = type;
}

//BOOL IsReadyQEmpty() {
//	return (ReadyQHead == NULL) && (ReadyQTail == NULL);
//}
//BOOL IsWaitQEmpty() {
//	return (WaitQHead == NULL) && (WaitQTail == NULL);
//}
BOOL IsQEmpty(Queue* queue) {
	return !(queue->count);
	//return (queue->front == NULL) && (queue->rear == NULL);
}

//void ReadyQEnqueue(Thread new) {
//	Thread* now = (Thread*)malloc(sizeof(Thread));
//	memcpy(now, &new, sizeof(Thread));
//	if (IsReadyQEmpty()) {
//		ReadyQHead = now;
//		ReadyQTail = now;
//		ReadyQHead->pPrev = NULL;
//		ReadyQHead->pNext = NULL;
//		ReadyQTail->pPrev = NULL;
//		ReadyQTail->pNext = NULL;
//	}
//	else {
//		ReadyQTail->pNext = now;
//		now->pPrev = ReadyQTail;
//		now->pNext = NULL;
//		ReadyQTail = now;
//	}
////	printf("pPrev now pNext : %d %d %d\n", now->pPrev, now, now->pNext);
//}
//void WaitQEnqueue(Thread new) {
//	Thread* now = (Thread*)malloc(sizeof(Thread));
//	memcpy(now, &new, sizeof(Thread));
//	if (IsWaitQEmpty()) {
//		WaitQHead = now;
//		WaitQTail = now;
//		WaitQHead->pPrev = NULL;
//		WaitQHead->pNext = NULL;
//		WaitQTail->pPrev = NULL;
//		WaitQTail->pNext = NULL;
//	}
//	else {
//		WaitQTail->pNext = now;
//		now->pPrev = WaitQTail;
//		now->pNext = NULL;
//		WaitQTail = now;
//	}
//}
void Enqueue(Queue* queue, Thread* new) {
	if (queue->type == 1) {	// ReadyQ
		if (IsQEmpty(queue)) {
			queue->front = new;
			queue->front->pPrev = NULL;
			queue->front->pNext = NULL;
			queue->rear = new;
			queue->rear->pPrev = NULL;
			queue->rear->pNext = NULL;
			//ReadyQHead->pPrev = NULL;
			ReadyQHead = queue->front;
			//ReadyQHead->pNext = NULL;
			ReadyQTail = queue->rear;
			//ReadyQTail->pPrev = NULL;
			//ReadyQTail->pNext = NULL;
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
			//ReadyQHead->pPrev = NULL;
			WaitQHead = queue->front;
			//ReadyQHead->pNext = NULL;
			WaitQTail = queue->rear;
			//ReadyQTail->pPrev = NULL;
			//ReadyQTail->pNext = NULL;
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
	//	printf("pPrev new pNext : %d %d %d\n", new->pPrev, new, new->pNext);
	return;
}

//Thread ReadyQDequeue() {
//	if (IsReadyQEmpty()) {
//		Thread temp;
//		temp.tid = -1;
//		return temp;
//	}
//	else {
//		Thread temp = *ReadyQHead;
//        memcpy(&temp, ReadyQHead, sizeof(Thread));
//		Thread* tempPtr = ReadyQHead;
//		//printf("\ncheck link of ReadyQ\n");
//		//printf("%d %d %d\n", tempPtr->pPrev, tempPtr, tempPtr->pNext);
//		if (ReadyQHead->pNext == NULL) {
//			ReadyQHead = NULL;
//			ReadyQTail = NULL;
//		}
//		else {
//			ReadyQHead = ReadyQHead->pNext;
//			ReadyQHead->pPrev = NULL;
//		}
//		free(tempPtr);
//		return temp;
//	}
//}
//Thread WaitQDequeue() {
//	if (IsWaitQEmpty()) {
//		Thread temp;
//		temp.tid = -1;
//		return temp;
//	}
//	else {
//		Thread temp = *WaitQHead;
//		Thread* tempPtr = WaitQHead;
//		WaitQHead = WaitQHead->pNext;
//		if (WaitQHead == NULL) {
//			WaitQTail = NULL;
//			WaitQHead = NULL;
//		}
//		else {
//			WaitQHead = WaitQHead->pNext;
//			WaitQHead->pPrev = NULL;
//		}
//		free(tempPtr);
//		return temp;
//	}
//}
Thread* Dequeue(Queue* queue) {
	//printf("\ncheck link of ReadyQ\n");
	//printf("%d %d %d\n", tempPtr->pPrev, tempPtr, tempPtr->pNext);
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


//Thread* SearchReadyTCB(thread_t tid) {
//	Thread* cursor = ReadyQHead;
//	while (cursor != NULL) {
//		if (cursor->tid == tid)
//			return cursor;
//		cursor = cursor->pNext;
//	}
//	cursor->tid = -1;
//	return cursor;
//}
//Thread* SearchWaitTCB(thread_t tid) {
//	Thread* cursor = WaitQHead;
//	while (cursor != NULL) {
//		if (cursor->tid == tid)
//			return cursor;
//		cursor = cursor->pNext;
//	}
//	cursor->tid = -1;
//	return cursor;
//}

Thread* SearchTCB(Queue* queue, thread_t tid) {
	Thread* cursor = queue->front;
	while (cursor != NULL) {
		if (cursor->tid == tid)
			return cursor;
		cursor = cursor->pNext;
	}
	cursor = (Thread*)malloc(sizeof(Thread));
	cursor->tid = -1;
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

