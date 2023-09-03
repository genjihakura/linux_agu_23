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
    printf("sendSIGINTSbyUser1 is calling, I unblock SIGINT for this process after 4 second\n");
    sigset_t newset, oldset;    
    sigemptyset(&newset);
    sigemptyset(&oldset);
    sigaddset(&newset,SIGINT);
    if (sigprocmask(SIG_UNBLOCK,&newset,&oldset)==0){
         printf("PID is unblocking SIGINT!!!\n");
    }
}

int main(char argc, char *argv[]){
    printf("I am signal_ub process my PID %d \n",getpid());
    sigset_t newset, oldset;
    if(signal(SIGINT,countSIGINT) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&newset);
    sigemptyset(&oldset);
    printf("newset = 0x%x\n",newset);
    printf("oldset = 0x%x\n",oldset);
    sigaddset(&newset,SIGINT);
    printf("newset = 0x%x\n",newset);
    //kiem tra newset duoc thiet lap la SIGNINT 
    if (sigismember(&newset,SIGINT) == 1){
        printf("newset is SIGINT\n");
    }
    else{
        printf("newset is no SIGINT\n");
    }
    // sigprocmask
    if (sigprocmask(SIG_SETMASK,&newset,&oldset)==1){
         printf("PID is blocking SIGINT!!!");
    }
    //kiem tra lai oldset - signal hien tai
    printf("oldset = %x\n",oldset);

    if(signal(SIGUSR1,sendSIGINTSbyUser1) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }

    while(1);
    return 0;
}