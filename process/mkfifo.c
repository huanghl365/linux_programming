#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
函数原型：int mkfifo(const char *pathname, mode_t mode);
描述：用来创建一个命名管道用于进程间通信。
参数：
pathname: 指定命名管道的路径名
mode:指定创建的命令管道文件的权限，创建的文件的权限为mode＆~umask，~mask不包括执行权限。
返回值：
创建成功返回0，创建失败返回-1.
*/

/*
程序功能描述：观察open调用命名管道的现象
*/
#if 0
#define FIFO_NAME "/home/mxc/nfs/my_fifo"
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
	（1）先运行./xxx O_RDONLY 再运行./xxx O_WRONLY 
	（2）先运行./xxx O_WRONLY 再运行./xxx O_RDONLY 
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

	sleep(5);

	if (-1 != fifo_fd)
	{
		close(fifo_fd);
	}
	
	printf("the process %d has close %s\n", getpid(), FIFO_NAME);
	return 0;
}
#endif

/*
程序功能描述：测试读写命名管道
*/

int main()
{
	return 0;
}