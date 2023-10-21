#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define PATH_FILE "/dev/m_cdev"

int main( char argc, char *argv[]){
    int fd, fd_read, fd_write;
    char *buffer_in;
    char *buffer_out;
    const int size = 100;
    // open file
    buffer_in = malloc(100*sizeof(char));
    buffer_out = malloc(100*sizeof(char));
    if( argc != 2){
        printf("./getDataDriver <string>\n");
        return -1;
    }
    
    printf("argv[1] %s \n", argv[1]);
    sprintf(buffer_in,"%s\0",argv[1]);
    // memset(buffer, 0, sizeof(buffer));
    // memcpy(buffer, &argv[1],sizeof(argv[1]));

    printf("buffer %s \n", buffer_in);

    fd = open(PATH_FILE,O_CREAT|O_RDWR);
    if(fd < 0){
        printf("open file error \n");
        return -1;
    }
    fd_write = write(fd, buffer_in, sizeof(buffer_in));
    if(fd_write < 0 ){
        printf("write to file error \n");
        return -1;
    }
    fd_read = read(fd, buffer_out, size );
    if(fd_read < 0 ){
        printf("read to file error \n");
        return -1;
    }
    printf("Data from kernel is %s \n", buffer_out);
    close(fd);
    return 0;

}