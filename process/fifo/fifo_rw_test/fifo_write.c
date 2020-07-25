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

/*
程序功能描述：测试读写命名管道
*/

#define FIFO_NAME "/home/mxc/nfs/my_fifo"
#define BUFFER_SIZE PIPE_BUF/16   //PIPE_BUF为命名管道的buffer大小，默认为4096字节
//#define BUFFER_SIZE PIPE_BUF
#define MSG_SIZE	1024*1024*10

int main()
{
	int fifo_fd;
	int ret;
	int i;
	int write_cnt =0 ;
	char write_buffer[BUFFER_SIZE];
	
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

	fifo_fd = open(FIFO_NAME, O_WRONLY);
	if (-1 == fifo_fd)
	{
		perror("open");
		exit(1);
	}
	else
	{
		printf("the process %d has open %s\n", getpid(), FIFO_NAME);
	}

/*
测试：设置不同大小的BUFFER_SIZE大小，观察读写效率
读取时间测试方法：time ./xxx
测试结果：
当读写大小刚好为PIPE_BUF，效率是最高的，读取10MB只用real 0m0.019s
当读写大小为PIPE_BUF/16时，效率降低，读取10MB用时real	0m6.349s
*/

#if 1
	memset(write_buffer, 'A', BUFFER_SIZE);
	
	while(write_cnt < MSG_SIZE)
	{
		
		ret = write(fifo_fd, write_buffer, BUFFER_SIZE);
		if (-1 != ret)
			write_cnt += ret;
		else
		{
			perror("write");
			exit(1);
		}
		printf("write %d bytes write_cnt = %d\n", ret, write_cnt);
		usleep(200*1000);	
	}
#endif 
	
	close(fifo_fd); //当关闭命名管道写端时，读端会read到0
	printf("the process %d has close fifo_fd\n", getpid());
	
	return 0;
}



