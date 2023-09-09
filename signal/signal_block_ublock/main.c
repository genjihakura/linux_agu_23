#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


void countSIGINT(){
    static int count = 0;
    printf("SIGINT is called, overwrite by count++ count = %d \n", ++count);

}

void sendSIGINTSbyUser1(){
    printf("sendSIGINTSbyUser1 is calling, I unblock SIGINT for this process after 4 second\n");
    sigset_t newset, oldset;
    sigemptyset(&newset);
    sigemptyset(&oldset);
    sigaddset(&newset,SIGINT);

    if (sigismember(&oldset,SIGINT) == 1){
        printf("%s, %d oldset is SIGINT\n", __func__,__LINE__);
    }
    else{
        perror("signal");
        printf("%s, %d oldset isn't signal\n", __func__,__LINE__);
    }
    if (sigprocmask(SIG_UNBLOCK,&newset,&oldset)==0){
         printf("PID is unblocking SIGINT!!!\n");
    }

    if (sigismember(&oldset,SIGINT) == 1){
        printf("%s, %d oldset is SIGINT\n", __func__,__LINE__);
    }
    else{
        perror("signal");
        printf("%s, %d oldset isn't signal\n", __func__,__LINE__);
    }
}

int main(char argc, char *argv[]){
    sigset_t newset, oldset;

    printf("I am signal_ub process my PID %d \n",getpid());

    if(signal(SIGINT,countSIGINT) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&newset);
    sigemptyset(&oldset);
    printf("newset = %d\n",newset);
    printf("oldset = %d\n",oldset);
    sigaddset(&newset,SIGINT);
    printf("newset = %d\n",newset);
    //kiem tra newset duoc thiet lap la SIGNINT 
    if (sigismember(&newset,SIGINT) == 1){
        printf("newset is SIGINT\n");
    }
    else{
        printf("newset isn't signal\n");
    }
    // sigprocmask
    if (sigprocmask(SIG_SETMASK,&newset,&oldset)==1){
         printf("PID is blocking SIGINT!!!");
    }
    //kiem tra lai oldset - signal hien tai
    printf("oldset = %d\n",oldset);

    if(signal(SIGUSR1,sendSIGINTSbyUser1) == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }

    while(1);
    return 0;
}