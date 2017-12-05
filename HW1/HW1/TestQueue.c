#include "Thread.h"
#include "Queue.h"
#include <stdio.h>

int main() {
	Thread th1, th2, th3;
	int i = 0;
	th1.tid = 1; th2.tid = 2; th3.tid = 3;

	if (IsReadyQEmpty() == 0)
		printf("Q is not empty\n");
	else
		printf("Q is empty\n");

	for (i; i < 3; i++) {
		if (i == 0)
			ReadyQEnqueue(th1); printf("Enqueue th1\n");
		if (i == 1)
			ReadyQEnqueue(th2); printf("Enqueue th2\n");
		if (i == 2)
			ReadyQEnqueue(th3); printf("Enqueue th3\n");

		if (IsReadyQEmpty() == 0)
			printf("Q is not empty\n");
		else
			printf("Q is empty\n");
	}
	for (i = 0; i < 3; i++) {
		if (i == 0)
			printf("tid : %d",ReadyQDequeue(th1).tid);
		if (i == 1)
			printf("tid : %d", ReadyQDequeue(th2).tid);
		if (i == 2)
			printf("tid : %d", ReadyQDequeue(th3).tid);
	}	
}