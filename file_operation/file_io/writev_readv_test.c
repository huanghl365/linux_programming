
#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(void){
	int fd ;
	int rsize,wsize; 
    char buf1[5+1] = "";
	char buf2[10+1] = "";
    struct iovec iov[2];

    iov[0].iov_base = buf1;
    iov[0].iov_len = 5;
    iov[1].iov_base = buf2;
    iov[1].iov_len = 10;

    fd = open("test.txt",O_RDWR);
    if(fd < 0){
        perror("open");
        return -1;
    }
    rsize = readv(fd, iov, 2);
    printf("rsize = %d\n",rsize);
	//printf("%d\n", iov[0].iov_len);
	//printf("%d\n", iov[1].iov_len);
	

    close(fd);

    fd = open("test2.txt", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd < 0){
        perror("open");
        return -1;
    }

	iov[0].iov_len = strlen(iov[0].iov_base);
	iov[1].iov_len = strlen(iov[1].iov_base);
	//printf("%d\n", iov[0].iov_len);
	//printf("%d\n", iov[1].iov_len);

    wsize = writev(fd,iov,2);
    printf("wsize = %d\n",wsize);

    close(fd);
    return 0;
}
