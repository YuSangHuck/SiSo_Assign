#include "Thread.h"
#include "Init.h"
#include "Queue.h"
#include "Common.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// getch
#include <term.h>
#include <termios.h>
#include <unistd.h>

void* test(){
    int i = 1;
    while(i){
        printf("(%d)secondes!\n", i++);
        sleep(1);
    }
}

int getch(){
    int ch;
    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

int main(int argc, char* argv[]){
    thread_t tid[1];
    int arg[2] = {1, 2};
    int i = 0;
    char ch;
    printf("test\n");
    Init();
    printf("test\n");

    for(i=0; i<1; i++)
        thread_create(&tid[i], NULL, (void*)test, (void*)&arg[i]);
    printf("child(%d) thread created\n",tid[0]);
    while((ch=getch())!='\n'){
        if(ch=='s'){ // sleep
            printf("sleep\n");
            //__thread_wait_handler()
            pthread_kill(tid[0], SIGUSR2);
        }
        else if(ch=='r'){ // wake up
            printf("wake up\n");
            //__thread_wakeup
            __thread_wakeup(SearchTCB(ReadyQ, tid[0]));
        }

    }
}

