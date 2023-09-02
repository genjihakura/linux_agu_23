#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

int main(int argc, char * argv[]){

	if(argc != 2){
		fprintf(stderr,"Usage %s <pathname> \n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int fd = open(argv[1], O_CREAT|O_RDWR,0666);

	if(fd < 0){
		perror("open");
		exit(EXIT_FAILURE);
	}
	int cur = lseek(fd,2,SEEK_SET);
	printf("vi tri dau tien trong file %d \n",cur);
	char buf[20]="kernel,master\n";
	int write_index = write(fd,buf,sizeof(buf));
	if (write_index<0){
		perror("write");
		exit(EXIT_FAILURE);
	}

	close(fd);
	exit(EXIT_SUCCESS);
	return 0;
}