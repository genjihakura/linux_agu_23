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


#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct _log_event {
    int  SquenceNumber;
    int  NodeID;
    char Msg[255];
    unsigned long int Timestamp;
} log_event;
void sendDatatoServer(int server_fd,float temperature );

int main(int argc, char *argv[])
{
    int portno;
    int server_fd;
    struct sockaddr_in serv_addr;
    float temperature;
    memset(&serv_addr, '0',sizeof(serv_addr));
    /* Đọc portnumber từ command line */
    if (argc < 4) {
        printf("command : ./client <server address> <port number>\n");
        exit(1);
    }
    portno = atoi(argv[2]);
    temperature = atof(argv[3]);
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
    printf("line : %d connect() success !!!\n",__LINE__ );
    sendDatatoServer(server_fd,temperature);
    printf("line : %d close() server_fd !!!\n",__LINE__ );      
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
    printf("line : %d connect() success !!!\n",__LINE__ );
    sendDatatoServer(server_fd,temperature+1.f); 
    printf("line : %d close() server_fd !!!\n",__LINE__ );      
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
    printf("line : %d connect() success !!!\n",__LINE__ );
    sendDatatoServer(server_fd,temperature+0.5f); 
    printf("line : %d close() server_fd !!!\n",__LINE__ );      
    close(server_fd); /*close*/

    return 0;
}


/* Chức năng chat */
void sendDatatoServer(int server_fd, float temperature)
{
    int numb_write, numb_read;
    char Msg[256];
    log_event sendbuff;
    memset(Msg, '0', sizeof(Msg));
    snprintf(Msg,sizeof(Msg),"%f",temperature);
    printf("line : %d sendDatatoServer() !!!\n",__LINE__ );
    numb_write = send(server_fd, &Msg, sizeof(Msg), 0);
    if (numb_write == -1){
        handle_error("send()");
    }

}