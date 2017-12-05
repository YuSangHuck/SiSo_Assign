#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "Queue.h"
#include "Common.h"

void Init(void) {
	// Create ready Queue and Waiting Queue
	InitReadyQ();
	InitWaitQ();
}
