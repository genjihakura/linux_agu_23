#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


void countSIGINT(){
    static int count = 0;
    printf("SIGINT is called, overwrite by count++ count = %d \n", count++);

}

void sendSIGINTSbyUser1(){
    printf("sendSIGINTSbyUser1 is calling, I send SIGINT in 4 times to this process after 4 second\n");
    for(int i = 0; i < 4; i++){
        sleep(1);
        kill(getpid(),SIGINT);
    }

}

void sendSIGKILLbyUser2(){
    printf("sendSIGINTSbyUser2 is calling, I send SIGKILL to this process after 3 second\n");
    sleep(3);
    kill(getpid(),SIGKILL);

}

int main(char argc, char *argv[]){
    printf("I am signal_int process my PID %d \n",getpid());

    if(signal(SIGINT,countSIGINT) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }
    if(signal(SIGUSR1,sendSIGINTSbyUser1) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }
    if(signal(SIGUSR2,sendSIGKILLbyUser2) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }
    while(1);
    return 0;
}