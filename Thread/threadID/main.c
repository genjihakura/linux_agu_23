#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

// struct human{
//     char name[30];
//     char yearbirth[4];
//     char numberphone[12];
//     char placeBirth[20];
// };
typedef struct {
    char name[30];
    char yearbirth[5];
    char numberphone[12];
    char placeBirth[20];
}human;

pthread_t pthread1, pthread2;

static void *handle_thread(void * data){

    pthread_t phtread_current = pthread_self();

    if(pthread_equal(phtread_current, pthread1)){
        printf("Hello I am thread in 1 \n");
    }
    else if (pthread_equal(phtread_current, pthread2)){
        printf("Hello I am thread in 2 the human is \n");
        printf("Name %s \n",((human*)data)->name);
        printf("Year Birth %s \n",((human*)data)->yearbirth);
        printf("Number phone %s \n",((human*)data)->numberphone);
        printf("Place of birth %s \n",((human*)data)->placeBirth);
    }
}

int main(char argc, char *argv[]){
    printf("I am thread ID task\n");
    human data = {"phat\0","1994\0","098888888\0","soc trang\0"};

    if(pthread_create(&pthread1, NULL, &handle_thread, NULL))
    {
        printf("thread1 fail to create \n");
        return -1;
    }
    sleep(5);
    if(pthread_create(&pthread2,NULL, &handle_thread, &data))
    {
        printf("thread2 fail to create \n");
        return -1;
    }

    sleep(5);
    return 0;
}