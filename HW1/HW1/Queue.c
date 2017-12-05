#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"
#include <stdlib.h>
#include <string.h>

void InitReadyQ() {
	Thread* ReadyQHead = NULL;
	Thread*	ReadyQTail = NULL;
}

void InitWaitQ() {
	Thread* WaitQHead = NULL;
	Thread*	WaitQTail = NULL;
}

BOOL IsReadyQEmpty() {
	return (ReadyQHead == NULL) && (ReadyQTail == NULL);
}

BOOL IsWaitQEmpty() {
	return (WaitQHead == NULL) && (WaitQTail == NULL);
}

void ReadyQEnqueue(Thread new) {
	Thread* now = (Thread*)malloc(sizeof(Thread));
	memcpy(now, &new, sizeof(Thread));
	if (IsReadyQEmpty()) {
		ReadyQHead = now;
		ReadyQTail = now;
		ReadyQHead->pPrev = NULL;
		ReadyQHead->pNext = NULL;
		ReadyQTail->pPrev = NULL;
		ReadyQTail->pNext = NULL;
	}
	else {
		ReadyQTail->pNext = now;
		now->pPrev = ReadyQTail;
		now->pNext = NULL;
		ReadyQTail = now;
	}
	printf("pPrev now pNext : %d %d %d\n", now->pPrev, now, now->pNext);
}

void WaitQEnqueue(Thread new) {
	Thread* now = (Thread*)malloc(sizeof(Thread));
	memcpy(now, &new, sizeof(Thread));
	if (IsWaitQEmpty()) {
		WaitQHead = now;
		WaitQTail = now;
		WaitQHead->pPrev = NULL;
		WaitQHead->pNext = NULL;
		WaitQTail->pPrev = NULL;
		WaitQTail->pNext = NULL;
	}
	else {
		WaitQTail->pNext = now;
		now->pPrev = WaitQTail;
		now->pNext = NULL;
		WaitQTail = now;
	}
}

Thread ReadyQDequeue() {
	if (IsReadyQEmpty()) {
		Thread temp;
		temp.tid = -1;
		return temp;
	}
	else {
		Thread temp = *ReadyQHead;
		Thread* tempPtr = ReadyQHead;
		//printf("\ncheck link of ReadyQ\n");
		//printf("%d %d %d\n", tempPtr->pPrev, tempPtr, tempPtr->pNext);
		if (ReadyQHead->pNext == NULL) {
			ReadyQHead = NULL;
			ReadyQTail = NULL;
		}
		else {
			ReadyQHead = ReadyQHead->pNext;
			ReadyQHead->pPrev = NULL;
		}
		free(tempPtr);
		return temp;
	}
}

Thread WaitQDequeue() {
	if (IsWaitQEmpty()) {
		Thread temp;
		temp.tid = -1;
		return temp;
	}
	else {
		Thread temp = *WaitQHead;
		Thread* tempPtr = WaitQHead;
		WaitQHead = WaitQHead->pNext;
		if (WaitQHead == NULL) {
			WaitQTail = NULL;
			WaitQHead = NULL;
		}
		else {
			WaitQHead = WaitQHead->pNext;
			WaitQHead->pPrev = NULL;
		}
		free(tempPtr);
		return temp;
	}
}

Thread SearchReadyTCB(thread_t tid) {
	Thread* cursor = ReadyQHead;
	while (cursor != NULL) {
		if (cursor->tid == tid)
			return *cursor;
		cursor = cursor->pNext;
	}
	cursor->tid = -1;
	return *cursor;
}

Thread SearchWaitTCB(thread_t tid) {
	Thread* cursor = WaitQHead;
	while (cursor != NULL) {
		if (cursor->tid == tid)
			return *cursor;
		cursor = cursor->pNext;
	}
	cursor->tid = -1;
	return *cursor;
}