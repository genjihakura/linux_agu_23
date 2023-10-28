#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket.
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct _log_event {
    int  SquenceNumber;
    int  NodeID;
    char Msg[255];
    unsigned long int Timestamp;
} log_event;
void sendDatatoServer(int server_fd,float temperature, int sensorID, time_t  curtime);

int main(int argc, char *argv[])
{
    int portno;
    int server_fd;
    struct sockaddr_in serv_addr;
    float temperature;
    int sensorID = 0;
    time_t curtime;
    
    memset(&serv_addr, '0',sizeof(serv_addr));
    /* Đọc portnumber từ command line */
    if (argc < 5) {
        printf("command : ./client <server address> <port number> <sensor ID> <temperature>\n");
        exit(1);
    }
    portno = atoi(argv[2]);
    sensorID = atoi(argv[3]);
    temperature = atof(argv[4]);
    printf("line : %d portno :%d \n",__LINE__ ,portno);
    /* Khởi tạo địa chỉ server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port   = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1){
        handle_error("inet_pton()");
    }
    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        handle_error("socket()");
    }
    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){

        handle_error("connect()");
    }
    time(&curtime);
    sendDatatoServer(server_fd,temperature,sensorID,curtime);     
    close(server_fd); /*close*/
    sleep(5);

        /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        handle_error("socket()");
    }
    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){

        handle_error("connect()");
    }
    time(&curtime);
    sendDatatoServer(server_fd,temperature+1,sensorID,curtime); 
    close(server_fd); /*close*/

    sleep(5);
        /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1){
        handle_error("socket()");
    }
    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){

        handle_error("connect()");
    }
    time(&curtime);
    sendDatatoServer(server_fd,temperature+0.5,sensorID,curtime); 
    close(server_fd); /*close*/

    return 0;
}


/* Chức năng chat */
void sendDatatoServer(int server_fd, float temperature,int sensorID, time_t  curtime)
{
    int numb_write, numb_read;
    char Msg[256];
    struct tm *info;
    info =  localtime( &curtime );
    log_event sendbuff;
    memset(Msg, '0', sizeof(Msg));


    snprintf(Msg,sizeof(Msg),"%d:%d:%d %d %f",info->tm_hour, info->tm_min, info->tm_sec, sensorID, temperature);
    printf("line : %d Msg  %s !!!\n",__LINE__,Msg);
    numb_write = send(server_fd, &Msg, sizeof(Msg), 0);
    if (numb_write == -1){
        handle_error("send()");
    }

}