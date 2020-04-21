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
函数原型：int mkfifo(const char *pathname, mode_t mode);
描述：用来创建一个命名管道用于进程间通信。
管道是单向的，因此使用open打开管道时，只能以O_RDONLY或者 O_WRONLY方式打开。
参数：
pathname: 指定命名管道的路径名
mode:指定创建的命令管道文件的权限，创建的文件的权限为mode＆~umask，~mask不包括执行权限。
返回值：
创建成功返回0，创建失败返回-1.
*/

/*
程序功能描述：观察open阻塞及非阻塞方式调用命名管道的现象
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

	close(fifo_fd);
	
	printf("the process %d has close fifo_fd\n", getpid());
	return 0;
}
#endif

/*
程序功能描述：测试读写命名管道
*/
#if 1
#define FIFO_NAME "/home/mxc/nfs/my_fifo"
#define BUFFER_SIZE PIPE_BUF/16   //PIPE_BUF为命名管道的buffer大小，默认为4096字节
//#define BUFFER_SIZE PIPE_BUF
#define MSG_SIZE	1024*1024*10

#if 0	//写端进程
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

	memset(write_buffer, 'A', BUFFER_SIZE);
	
	while(write_cnt < MSG_SIZE)
	{
		/*
		测试：设置不同大小的BUFFER_SIZE大小，观察读写效率
		读取时间测试方法：time ./xxx
		测试结果：
		当读写大小刚好为PIPE_BUF，效率是最高的，读取10MB只用real 0m0.019s
		当读写大小为PIPE_BUF/16时，效率降低，读取10MB用时real	0m6.349s
		*/
		ret = write(fifo_fd, write_buffer, BUFFER_SIZE);
		if (-1 != ret)
			write_cnt += ret;
		else
		{
			perror("write");
			exit(1);
		}
		printf("write %d bytes write_cnt = %d\n", ret, write_cnt);

		/*
		测试：适当延时，观察读端的现象
		测试结果：当每次写入的数据小于PIPE_BUF时，读端也能立即读取，不用写端写满PIPE_BUF。
		*/
		//usleep(500*1000);
		
	}

	
	close(fifo_fd); //当关闭命名管道写端时，读端会read到0
	printf("the process %d has close fifo_fd\n", getpid());
	
	return 0;
}
#else	//读端进程
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


	sleep(5);  //适当延时，观察写端的现象
	while(1)
	{
		
		ret = read(fifo_fd, read_buffer, BUFFER_SIZE);
		if (ret > 0)
		{
			read_cnt += ret;
			printf("read %d bytes read_cnt = %d\n", ret, read_cnt);
			//printf("read content:%s\n", read_buffer); //这里打印会影响读写效率
			memset(read_buffer, 0, BUFFER_SIZE);

			/*
			测试：适当延时，观察写端的现象
			测试结果：
			一开始读端延时5s，此时写端写入65536个字节后才处于阻塞状态。			
			之后当读端读取累计PIPE_BUF大小数据后，写端才能继续写入。

			疑问：什么一开始写端写入			65536个字节后才处于阻塞状态，为何不是PIPE_BUF就阻塞。
			*/
			usleep(500*1000);
		}
		else
			break;
	}

	close(fifo_fd);
	printf("the process %d has close fifo_fd\n", getpid());
	
	return 0;
}
#endif
#endif 

/*
程序功能描述：
*/


