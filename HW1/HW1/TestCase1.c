#include "TestCase1.h"
#include "Queue.h"
#include "Common.h"

void* Tc1ThreadProc(void* param)
{
	thread_t tid = 0;
	int count = 0;
	int* retVal;

	tid = thread_self();

	count = 5;
	while (count > 0)
	{
		/* sleep for 1 seconds */
		sleep(2);
		printf("Tc1ThreadProc: my thread id (%d), arg is (%d)\n", (int)tid, *((int*)param));
		count--;
	}
	retVal = (int*)param;
	thread_exit(retVal);
	return NULL;
}

/*
 * - TestCase1 tests a round-robin scheduling of thread
 * - Testing API scopes: thread_create
 */
void TestCase1(void)
{
    printf("TestCase1 start\n");
	thread_t tid[TOTAL_THREAD_NUM];

	thread_create(&tid[0], NULL, (void*)Tc1ThreadProc, (void*)1);
	thread_create(&tid[1], NULL, (void*)Tc1ThreadProc, (void*)2);
	thread_create(&tid[2], NULL, (void*)Tc1ThreadProc, (void*)3);
	thread_create(&tid[3], NULL, (void*)Tc1ThreadProc, (void*)4);
	thread_create(&tid[4], NULL, (void*)Tc1ThreadProc, (void*)5);
    
//    sleep(1);
    int i = 0;
    while(i<5){
        printf("tid[%d]:%d\n", i, tid[i]);
        i++;
    }
    while(!IsReadyQEmpty()){
        Thread thread;
        thread = ReadyQDequeue();
        printf("Queue : %d\n", thread.tid);
    }
//    printf("# of SIGUSR1 : %d\n",testI);
	while (1) {}

	return;
}

