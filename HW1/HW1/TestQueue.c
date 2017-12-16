#include "Init.h"
#include "Thread.h"
#include "Queue.h"
#include "Common.h"
#include <stdio.h>

int main() {
    Init();
	InitQ(ReadyQ, 1);
	InitQ(WaitQ, 2);
	Thread* th1 = (Thread*)malloc(sizeof(Thread));
	Thread* th2 = (Thread*)malloc(sizeof(Thread));
	Thread* th3 = (Thread*)malloc(sizeof(Thread));
	int i = 0;
	th1->tid = 1; th2->tid = 2; th3->tid = 3;
	printf("id of threads direct : %d %d %d\n", th1->tid, th2->tid, th3->tid);

    for (i; i < 3; i++) {
		if (i == 0) {
			printf("Enqueue th1\n");
			Enqueue(ReadyQ, th1);
		}
		else if (i == 1) {
			printf("Enqueue th2\n");
			Enqueue(ReadyQ, th2);
		}
		else if (i == 2) {
			printf("Enqueue th3\n");
			Enqueue(ReadyQ, th3);
		}
	}
	printf("\nid of threads indirect : %d %d %d\n", ReadyQHead->tid, ReadyQHead->pNext->tid, ReadyQHead->pNext->pNext->tid);
	printf("check link of ReadyQ\n");
	printf("%d %d %d\n", ReadyQHead->pPrev, ReadyQHead, ReadyQHead->pNext);
	printf("%d %d %d\n", ReadyQHead, ReadyQHead->pNext, ReadyQHead->pNext->pNext);
	printf("%d %d %d\n\n", ReadyQHead->pNext, ReadyQHead->pNext->pNext, ReadyQHead->pNext->pNext->pNext);
    printf("search(5) : %d\n", SearchTCB(ReadyQ, 5)->tid);
	for (i = 0; i < 3; i++) {
		if (i == 0)
			printf("tid : %d\n", Dequeue(ReadyQ)->tid);
		if (i == 1)
			printf("tid : %d\n", Dequeue(ReadyQ)->tid);
		if (i == 2)
			printf("tid : %d\n", Dequeue(ReadyQ)->tid);
	}
}
