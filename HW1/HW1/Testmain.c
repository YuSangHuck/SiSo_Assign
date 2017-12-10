#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

#include "TestCase1.h"
#include "TestCase2.h"
#include "TestCase3.h"

#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[]) {
    perror("[FileName]\t state");
    int TcNum;
	thread_t tid1, tid2, tid3, tid4;

	if (argc != 2) {
		perror("Input TestCase Number!");
		exit(0);
	}
    printf("[main.c:23]\t Init()\n");
	Init();

	TcNum = atoi(argv[1]);

    switch (TcNum)
	{
	case 1:
        printf("[main:31]\t thread_crate(Testcase1)\n");
		thread_create(&tid1, NULL, (void*)TestCase1, 0);
		break;
	case 2:
        printf("[main:35]\t thread_crate(Testcase2)\n");
		thread_create(&tid2, NULL, (void*)TestCase2, 0);
		break;
	case 3:
        printf("[main:39]\t thread_crate(Testcase3)\n");
		thread_create(&tid3, NULL, (void*)TestCase3, 0);
		break;

	}
    printf("[main:41]\t RunScheduler()\n");
	RunScheduler();
	while (1) {}
}

