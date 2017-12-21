t : Common.o Init.o Queue.o Scheduler.o TestCase1.o TestCase2.o TestCase3.o Thread.o Testmain.o
	gcc -g -o t Common.o Init.o Queue.o Scheduler.o TestCase1.o TestCase2.o TestCase3.o Thread.o Testmain.o -lpthread
Testmain.o : Testmain.c
	gcc -g -c Testmain.c
Thread.o : Thread.c Thread.o
	gcc -g -c Thread.c
TestCase1.o : TestCase1.c TestCase1.h
	gcc -g -c TestCase1.c
TestCase2.o : TestCase2.c TestCase2.h
	gcc -g -c TestCase2.c
TestCase3.o : TestCase3.c TestCase3.h
	gcc -g -c TestCase3.c
Scheduler.o : Scheduler.c Scheduler.h
	gcc -g -c Scheduler.c
Queue.o : Queue.c Queue.h
	gcc -g -c Queue.c
Init.o : Init.c Init.h Thread.c
	gcc -g -c Init.c Thread.c 
Common.o : Common.c Common.h
	gcc -g -c Common.c
clean :
	rm -rf *.o
