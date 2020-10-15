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
程序功能描述：观察分析open调用阻塞及非阻塞方式打开命名管道的现象
*/
#define FIFO_NAME "./my_fifo"
int main(int argc, char *argv[])
{
	int open_flags = 0;
	int fifo_fd;
	int ret;
	int i;
	if (argc < 2)
	{
		printf("usage: ./xxx (the mix of O_WRONLY/O_RDONLY/O_NONBLOCK)\n");
		exit(1);
	}
	for (i=0; i< argc-1; i++)
	{
		if (!strcmp(*++argv, "O_WRONLY"))
		{
			open_flags |= O_WRONLY;
		}
		if (!strcmp(*argv, "O_RDONLY"))
		{
			open_flags |= O_RDONLY;
		}
		if (!strcmp(*argv, "O_NONBLOCK"))
		{
			open_flags |= O_NONBLOCK;
		}
	}

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


	/*
	测试：
	（1）先运行./xxx O_RDONLY，打开另一个终端再运行./xxx O_WRONLY 
	（2）先运行./xxx O_WRONLY，打开另一个终端再运行./xxx O_RDONLY 
	（3）直接运行./xxx O_RDONLY O_NONBLOCK
	（4）直接运行./xxx O_WRONLY O_NONBLOCK
	（5）直接运行./xxx O_RDONLY O_NONBLOCK，并且读端关闭前运行./xxx O_WRONLY O_NONBLOCK
	测试结果：
	（1）读端open阻塞，只有写端运行才能open成功
	（2）写端open阻塞，只有读端运行才能open成功
	（3）不阻塞，能够成功open命名管道
	（4）不阻塞，但是open命令管道失败，提示 open: No such device or address
	（5）不阻塞，写端能够open成功
	*/	
	fifo_fd = open(FIFO_NAME, open_flags);
	if (-1 == fifo_fd)
	{
		perror("open");
		exit(1);
	}
	else
	{
		printf("the process %d has open %s\n", getpid(), FIFO_NAME);
	}

	close(fifo_fd);
	
	printf("the process %d has close fifo_fd\n", getpid());
	return 0;
}


