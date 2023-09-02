#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(char argc, char *argv[]){
    int pid;
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
        printf("I am parrent, my PID = %d \n",getpid());
        // exit(EXIT_SUCCESS);
        do {
            w = waitpid(pid,&statusChild,WUNTRACED| WCONTINUED);
            if(w == -1){
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            
            if(WIFEXITED(statusChild)){
                printf("Exit easy, status = %d \n", WIFEXITED(statusChild));
            }
            else if(WIFSIGNALED(statusChild)){
                printf("Killed by signal %d \n", WTERMSIG(statusChild));
            }
            else if(WIFSTOPPED(statusChild)){
                printf("Stopped by signal %d \n",WSTOPSIG(statusChild));
            }
            else{
                printf("countinue \n");
            }

        }while(!(WIFEXITED(statusChild))&&!(WIFSIGNALED(statusChild)));
        // while(1);
        exit(EXIT_SUCCESS);
    }
    return 0;
}