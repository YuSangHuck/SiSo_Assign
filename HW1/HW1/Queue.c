#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include <stdlib.h>

void InitReadyQ(){
	Thread* ReadyQHead = NULL;
	Thread*	ReadyQTail = NULL;
}

void InitWaitQ(){
	Thread* WaitQHead = NULL;
	Thread*	WaitQTail = NULL;
}

BOOL IsReadyQEmpty(){
	return (ReadyQHead == NULL) && (ReadyQTail == NULL);
}

BOOL IsWaitQEmpty(){
	return (WaitQHead == NULL) && (WaitQTail == NULL);
}

void ReadyQEnqueue(Thread new) {
	Thread* now = (Thread*)malloc(sizeof(Thread));
	if (IsReadyQEmpty()) {
		ReadyQHead = now;
		ReadyQTail = now;
	}
	else {
		ReadyQTail->pNext = now;
		now->pPrev = ReadyQTail;
		now->pNext = NULL;
		ReadyQTail = now;
	}
}

void WaitQEnqueue(Thread new) {
	Thread* now = (Thread*)malloc(sizeof(Thread));
	if (IsWaitQEmpty()) {
		WaitQHead = now;
		WaitQTail = now;
	}
	else {
		WaitQTail->pNext = now;
		now->pPrev = WaitQTail;
		now->pNext = NULL;
		WaitQTail = now;
	}
}

Thread ReadyQDequeue(){
	if (IsReadyQEmpty())
		return -1; // 될까?
	else {
		Thread temp = *ReadyQHead;
		Thread* tempPtr = ReadyQHead;
		ReadyQHead = ReadyQHead->pNext;
		if (ReadyQHead == NULL)
			ReadyQTail = NULL;
		ReadyQHead->pPrev = NULL;
		free(tempPtr);
		return temp;
	}
}

Thread WaitQDequeue(){
	if (IsWaitQEmpty())
		return -1; // 될까?
	else {
		Thread temp = *WaitQHead;
		Thread* tempPtr = WaitQHead;
		WaitQHead = WaitQHead->pNext;
		if (WaitQHead == NULL)
			WaitQTail = NULL;
		WaitQHead->pPrev = NULL;
		free(tempPtr);
		return temp;
	}
}