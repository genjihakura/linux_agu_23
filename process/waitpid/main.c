#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(char argc, char *argv[]){
    int pid;
    int statusChild;
    int w;
    int fd = 0;
    pid = fork();
    if(pid == 0){
        printf("I am child my PID = %d , my parrent have PID = %d \n", getpid(),getppid());
        printf("child doing \n");
        if (argc < 2 ){
            fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	        exit(EXIT_FAILURE);
        }

        fd = open(argv[1], O_RDWR, 0666);
        if(fd < 0){
            perror("open");
            exit(EXIT_FAILURE);
        }
        char buf[20];
        int read_stat = read(fd,buf,sizeof(buf));
        if(read_stat < 0){
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd);
        printf("child reading the book: %s \n", buf);
        exit(EXIT_SUCCESS);
    }
    else{
        printf("I am parrent, my PID = %d \n",getpid());
        
        if (argc < 2 ){
            fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	        exit(EXIT_FAILURE);
        }

        fd = open(argv[1], O_CREAT| O_RDWR, 0666);
        if(fd < 0){
            perror("open");
            exit(EXIT_FAILURE);
        }
        char buf[]="AAAAA";
        int write_stat = write(fd,buf,sizeof(buf));
        if(write_stat < 0){
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd);


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