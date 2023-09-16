#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>

int main(char argc, char * argv[]){

    int pid = fork();
    if(pid ==0){
        printf("I am child my PID =%d. My parent have PID = %d\n",getpid(),getppid());
        exit(EXIT_SUCCESS);
    }
    else{
        printf("I am parent my PID =%d \n ",getpid());
        exit(EXIT_SUCCESS);
    }
    return 0;
}