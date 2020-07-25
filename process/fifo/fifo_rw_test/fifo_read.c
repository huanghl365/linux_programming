#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define FIFO_NAME "/home/mxc/nfs/my_fifo"
#define BUFFER_SIZE PIPE_BUF/16   //PIPE_BUF为命名管道的buffer大小，默认为4096字节
//#define BUFFER_SIZE PIPE_BUF
#define MSG_SIZE	1024*1024*10

int main()
{
	int fifo_fd;
	int ret;
	int i;
	char read_buffer[BUFFER_SIZE];
	int read_cnt = 0;
		
	if (access(FIFO_NAME, F_OK))
	{
		ret = mkfifo(FIFO_NAME, 777);
		if (0 != ret)
		{
			perror("mkfifo");
			exit(1);
		}
		else
		{
			printf("the process %d has mkfifo the %s\n", getpid(), FIFO_NAME);
		}
	}
	else
	{
			printf("%s is exist\n", FIFO_NAME);
	}

	fifo_fd = open(FIFO_NAME, O_RDONLY);
	if (-1 == fifo_fd)
	{
		perror("open");
		exit(1);
	}
	else
	{
		printf("the process %d has open %s\n", getpid(), FIFO_NAME);
	}

	memset(read_buffer, 0, BUFFER_SIZE);
	while(1)
	{
		
		ret = read(fifo_fd, read_buffer, BUFFER_SIZE);
		if (ret > 0)
		{
			read_cnt += ret;
			printf("read %d bytes read_cnt = %d\n", ret, read_cnt);
			//printf("read content:%s\n", read_buffer); //这里打印会影响读写效率
			memset(read_buffer, 0, BUFFER_SIZE);
			usleep(200*1000);
		}
		else
			break;
	}
	close(fifo_fd);	
	printf("the process %d has close fifo_fd\n", getpid());
	
	return 0;
}

