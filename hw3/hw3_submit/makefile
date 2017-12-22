test: Init.o Thread.o TestCase1.o TestMain.o Scheduler.o MsgQueue.o TestCase2.o TestCase3.o TestCase4.o
	gcc -o test Init.o Thread.o TestCase1.o TestMain.o Scheduler.o MsgQueue.o TestCase2.o TestCase3.o TestCase4.o

Init.o: Init.c Init.h _Thread.h Scheduler.h
	gcc -c Init.c _Thread.h Scheduler.h

Thread.o: Thread.c _Thread.h Init.h Scheduler.h
	gcc -c Thread.c _Thread.h Init.h Scheduler.h

TestCase1.o: TestCase1.h TestCase1.c
	gcc -c TestCase1.c
TestCase2.o: TestCase2.c TestCase2.h
	gcc -c TestCase2.c
TestCase3.o: TestCase3.c TestCase3.h
	gcc -c TestCase3.c
TestCase4.o: TestCase4.c TestCase4.h
	gcc -c TestCase4.c
TestMain.o: TestMain.c TestCase1.h Init.h Scheduler.h Thread.h
	gcc -c TestMain.c TestCase1.h Init.h Scheduler.h Thread.h

Scheduler.o: Scheduler.c Init.h _Thread.h Scheduler.h
	gcc -c Scheduler.c Init.h _Thread.h Scheduler.h

MsgQueue.o: MsgQueue.c Thread.h MsgQueue.h _Thread.h
	gcc -c MsgQueue.c Thread.h MsgQueue.h _Thread.h

clean:
	rm test Init.o Thread.o TestCase1.o TestMain.o Scheduler.o MsgQueue.o TestCase2.o TestCase3.o TestCase4.o



