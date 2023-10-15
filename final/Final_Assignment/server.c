#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket.
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define LISTEN_BACKLOG 50
#define BUFF_SIZE 255
#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define FIFO_FILE   "logFifo"


pthread_mutex_t lock_connect = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_data = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_manager = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_connect = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_data = PTHREAD_COND_INITIALIZER;

int counter = 2; // shared variable/shared resources/global variable
int port_no;
int len, opt;
int server_fd, new_socket_fd; 
struct sockaddr_in serv_addr, client_addr;

typedef struct _log_event {
    int  SquenceNumber;
    int  NodeID;
    float Temperature;
    unsigned long int Timestamp;
} log_event;

typedef struct _thread_args_t{
        int IsChild;
}thread_args_t;

//share memory
log_event dataSensor;
/* Chức năng chat */
void ServerReadDataAndWriteDataToShareStruct(int new_socket_fd)
{
    char recvbuff[BUFF_SIZE];
    int numb_read, numb_write;
    memset(&recvbuff, '0', sizeof(log_event));
    numb_read = recv(new_socket_fd, &recvbuff, BUFF_SIZE, 0);
    if(numb_read == -1){
        handle_error("recv() form socket");
    }
    dataSensor.Temperature = atof(recvbuff);
    printf("line : %d temperature : %f \n",__LINE__, dataSensor.Temperature );

}


//Server TCP
//Listen on a TCP socket for incoming connect request from new sensor nodes
static void *Connect_handle(void *args)
{
    thread_args_t *thr = (thread_args_t *)args;
    pthread_mutex_lock(&lock_connect);
    if(thr->IsChild == 0){
    // log process thread
        char buff[BUFF_SIZE];
        int fd; 
        unsigned char size_buff = BUFF_SIZE;
        mkfifo(FIFO_FILE, 0666);
        while (1) {
            // Write first
            // printf("Message to comsumer : "); fflush(stdin);
            // fgets(buff, BUFF_SIZE, stdin);
            sprintf(buff, "%f", dataSensor.Temperature );
            fd = open(FIFO_FILE, O_WRONLY);
            write(fd, buff, strlen(buff) +1);
            close(fd);
        }
        pthread_cond_signal(&cond_connect);
        pthread_mutex_unlock(&lock_connect);
        pthread_exit(NULL); // exit

    } else {
    // Main process thread

        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        memset(&client_addr, 0, sizeof(struct sockaddr_in));

        /* Tạo socket */
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1){
            handle_error("socket()");
        }
        // fprintf(stderr, "ERROR on socket() : %s\n", strerror(errno));

        /* Ngăn lỗi : “address already in use” */
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            handle_error("setsockopt()");
        }

        /* Khởi tạo địa chỉ cho server */
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port_no);
        serv_addr.sin_addr.s_addr =  INADDR_ANY; //inet_addr("192.168.5.128"); //INADDR_ANY

        /* Gắn socket với địa chỉ server */
        if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
            handle_error("bind()");
        }

        /* Nghe tối đa 5 kết nối trong hàng đợi */
        if (listen(server_fd, LISTEN_BACKLOG) == -1){
            handle_error("listen()");
        }

        /* Dùng để lấy thông tin client */
        len = sizeof(client_addr);
        
        while (1) {
            printf("Server is listening at port : %d \n....\n",port_no);
            new_socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t *)&len);
            if (new_socket_fd == -1)
            {
                handle_error("accept() no connect");
            }
            system("clear");
            char temp[BUFF_SIZE];
            inet_ntop(client_addr.sin_family, (struct sockaddr*)&client_addr, temp, sizeof(temp));
            printf("Server : got connection \n");
            printf("socket_fd : %d \n", new_socket_fd);
            ServerReadDataAndWriteDataToShareStruct(new_socket_fd);
            // close(server_fd);
        }
    }
    close(server_fd);
    pthread_cond_signal(&cond_connect);
    pthread_mutex_unlock(&lock_connect);
    pthread_exit(NULL); // exit
}
//



static void *Data_handle(void *args)
{  
    thread_args_t *thr = (thread_args_t *)args;
    float sum = 0;
    float avrg = 0;
    pthread_mutex_lock(&lock_connect);
    if(thr->IsChild == 0){
        // log process thread
    }else{
        //Main process thread
        //read data from share data, calculate a running average on the temperature  use the result to decide  on 'too/hot/cold'
        //this function only read data and calculate 
        sum += dataSensor.Temperature;
    }
    pthread_mutex_unlock(&lock_connect);
    pthread_exit(NULL); // exit

}
//read sensor measurements and insert them to SQL
static void *Storage_manager_handle(void *args)
{  

    thread_args_t *thr = (thread_args_t *)args;
    pthread_mutex_lock(&lock_connect);
    if(thr->IsChild == 0){
        // log process thread
    }else{
        //Main process thread
    }
    pthread_mutex_unlock(&lock_connect);
    pthread_exit(NULL); // exit
}


int main(int argc, char *argv[]){
    int ret;
    thread_args_t thr;
    pthread_t thread_id1, thread_id2, thread_id3;

    pid_t log_process = fork();
    if( log_process == 0){
        thr.IsChild = log_process;
        // log_process is child process
        if (ret = pthread_create(&thread_id1, NULL, &Connect_handle, &thr)) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }

        if (ret = pthread_create(&thread_id2, NULL, &Data_handle, NULL)) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }

        if (ret = pthread_create(&thread_id3, NULL, &Storage_manager_handle, NULL)) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }

        pthread_join(thread_id1,NULL);
        pthread_cond_wait(&cond_connect, &lock_connect);
        pthread_join(thread_id2,NULL);
        pthread_cond_wait(&cond_connect, &lock_connect);
        pthread_join(thread_id3,NULL);
    }else{
    //main process TBD
        thr.IsChild = log_process;

        /* Đọc portnumber trên command line */
        if (argc < 2) {
            printf("No port provided\ncommand: ./server <port number>\n");
            exit(EXIT_FAILURE);
        } else{
                port_no = atoi(argv[1]);
        }

        if (ret = pthread_create(&thread_id1, NULL, &Connect_handle, &thr)) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }

        if (ret = pthread_create(&thread_id2, NULL, &Data_handle, NULL)) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }

        if (ret = pthread_create(&thread_id3, NULL, &Storage_manager_handle, NULL)) {
            printf("pthread_create() error number=%d\n", ret);
            return -1;
        }
        pthread_join(thread_id1,NULL);
        pthread_cond_wait(&cond_connect, &lock_connect);
        pthread_join(thread_id2,NULL);
        pthread_cond_wait(&cond_connect, &lock_connect);
        pthread_join(thread_id3,NULL);
    }
    return 0;
}