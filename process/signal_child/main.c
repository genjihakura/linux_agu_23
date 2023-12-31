#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>

void func(){
    printf("child is killing. \n");
    wait(NULL);
    printf("child is killed. \n");
}

int main(char argc, char *argv[]){
    pid_t pid;
    int statusChild;
    int w;
    pid = fork();
    if(pid == 0){
        printf("I am child my PID = %d , my parrent have PID = %d \n", getpid(),getppid());
        printf("Done!waiting for you!\n");
        // while(1);
        exit(EXIT_SUCCESS);
        // program child is done and become to zombie,
    }
    else{
        printf("I am parent my PID = %d \n", getpid());
        signal(SIGCHLD,func);
        // using signal to clean child process by wait function
        while(1);
        exit(EXIT_SUCCESS);
    }
    return 0;
}