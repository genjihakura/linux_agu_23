#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

void func(){
    static int count = 0;
    printf("SIGINT is called, overwrite by count++ count = %d \n", count++);

}

int main(char argc, char *argv[]){
    
    int status_signal;
    

    status_signal = signal(SIGCHLD,func);
    if(status_signal == SIG_ERR){
        fprintf(stderr, "Cannot handle SIGINT \n");
        exit(EXIT_FAILURE);
    }
    while(1);
    return 0;
}