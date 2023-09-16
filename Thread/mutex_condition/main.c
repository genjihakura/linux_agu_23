#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    char name[30];
    char yearbirth[5];
    char numberphone[12];
    char placeBirth[20];
} human;


pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 ;
pthread_cond_t cond2 ;

static void *getInfoHumanHandle(void * data){

    // data = (human *)malloc(sizeof(human));
    human *data_input = (human*)data; 
    char *buffer =malloc(30*sizeof(char));
    size_t  buffer_size =30;
    pthread_mutex_lock(&lock1);
    printf("Type name:\n");
    // getline(&buffer, &buffer_size,stdin);
    scanf("%s[^\n]",buffer);
    // fgets(buffer,buffer_size,stdin);
    // strtok(buffer, "\n");
    memcpy(&data_input->name,buffer,sizeof(buffer));
    memset(buffer,0,sizeof(buffer));

    printf("Type Year birth:\n");
    // getline(&buffer,&buffer_size,stdin);
    // scanf("%s",buffer);
    // fgets(buffer,buffer_size,stdin);
    // strtok(buffer, "\n");
    scanf("%s[^\n]",buffer);
    memcpy(&data_input->yearbirth,buffer,sizeof(buffer));
    memset(buffer,0,sizeof(buffer));

    printf("Type number phone:\n");
    // fgets(buffer,buffer_size,stdin);
    // strtok(buffer, "\n");
    // getline(&buffer,&buffer_size,stdin);
    // scanf("%s",buffer);
    scanf("%s[^\n]",buffer);
    memcpy(&data_input->numberphone,buffer,sizeof(buffer));
    memset(buffer,0,sizeof(buffer));
    printf("Type place of birth:\n");
    // fgets(buffer,buffer_size,stdin);
    // strtok(buffer, "\n");
    // getline(&buffer,&buffer_size,stdin);
    // scanf("%s",buffer);
    scanf("%s[^\n]",buffer);
    memcpy(&data_input->placeBirth,buffer,sizeof(buffer));
    memset(buffer,0,sizeof(buffer));
    // pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&lock1);

}

static void *writeInfoHumanToTXT(void * data){
    pthread_mutex_lock(&lock1);   
    // while(1){
    pthread_cond_wait(&cond1,&lock1);
    printf("xong dien ten nguoi\n");
    human *data_input = (human*)data; 
    printf("vao ham thread 2\n");
    int fd = open("infor.txt",O_RDWR,0666);
    if (fd < 0){
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buffer[80];
    int sizebuffer = sprintf(buffer,"%s,%s,%s,%s\n",data_input->name,data_input->yearbirth,data_input->numberphone,data_input->placeBirth); 
    int writefd = write(fd,buffer,sizebuffer);
    if (writefd < 0){
        perror("write to file fail");
        exit(EXIT_FAILURE);
    }
    close(fd);
    // }
    // pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&lock1);

}

static void *ShowInfoHumman(void * data){
    pthread_mutex_lock(&lock1);
    pthread_cond_wait(&cond2,&lock1);
    printf("xong dien file\n");
    human *data_input = (human*)data;
    printf("Info Human \n");
    printf("Name :%s \n" , ((human*)data_input)->name);
    printf("Year of birth :%s \n", ((human*)data_input)->yearbirth);
    printf("Number phone :%s \n", ((human*)data_input)->numberphone);
    printf("Place of birth :%s \n", ((human*)data_input)->placeBirth);
    pthread_mutex_unlock(&lock1);

}
void release2Fund(){
    pthread_cond_signal(&cond2);
}
void release1Fund(){
    pthread_cond_signal(&cond1);
}
int main(char argc, char *argv[]){
    printf("I work for update Humman info \n");
    pthread_t pthread1, pthread2, pthread3;
    human data;
    memcpy( &data.name, "phat",sizeof("phat"));
    if(pthread_create(&pthread1, NULL, &getInfoHumanHandle, &data))
    {
        printf("thread getInfoHumanHandle fail to create \n");
        return -1;
    }

    // pthread_cond_wait(&cond1,&lock1);  
    // printf("xong dien ten nguoi\n");
    if(pthread_create(&pthread2,NULL, &writeInfoHumanToTXT, &data))
    {
        printf("thread writeInfoHumanToTXT fail to create \n");
        return -1;
    }

    // pthread_cond_wait(&cond2,&lock1);
    // printf("xong dien file\n");
    if(pthread_create(&pthread3,NULL, &ShowInfoHumman, &data))
    {
        printf("thread ShowInfoHumman fail to create \n");
        return -1;
    }
    // sleep(5);
    // printf("xong \n");


    pthread_join(pthread1,NULL);
        release1Fund();
    pthread_join(pthread2,NULL);
        release2Fund();
    pthread_join(pthread3,NULL);
    // pthread_mutex_destroy(&lock1);
    return 0;
}