#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>

void func(){
    printf("child is killing \n");
    wait(NULL);
}

int main(char argc, char *argv[]){
    pid_t pid;
    int statusChild;
    int w;
    pid = fork();
    if(pid == 0){
        printf("I am child my PID = %d , my parrent have PID = %d \n", getpid(),getppid());
        printf("Done \n");
        while(1);
        exit(EXIT_SUCCESS);
    }
    else{
        printf("I am parent my PID = %d \n", getpid());
        signal(SIGCHILD,func);
        while(1);
        exit(EXIT_SUCCESS);
    }
    return 0;
}