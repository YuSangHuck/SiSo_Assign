Testmain : Common.o Init.o Queue.o Scheduler.o TestCase1.o TestCase2.o TestCase3.o Thread.o
	gcc -o Testmain Common.o Init.o Queue.o Scheduler.o TestCase1.o TestCase2.o TestCase3.o Thread.o Testmain.o
Common.o : Common.c Common.h
	gcc -c Common.c
Init.o : Init.c Init.h
	gcc -c Init.c
Queue.o : Queue.c Queue.h
	gcc -c Queue.c
Scheduler.o : Scheduler.c Scheduler.h
	gcc -c Scheduler.c
TestCase1.o : TestCase1.c TestCase1.h
	gcc -c TestCase1.c
TestCase2.o : TestCase2.c TestCase2.h
	gcc -c TestCase2.c
TestCase3.o : TestCase3.c TestCase3.h
	gcc -c TestCase3.c
Thread.o : Thread.c Thread.o
	gcc -c Thread.c
Testmain.o : Testmain.c
	gcc -c Testmain.c
clean :
	rm -rf *.o
