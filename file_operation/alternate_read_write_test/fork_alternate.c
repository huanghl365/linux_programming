#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：先创建文件，再fork创建子进程，测试父子进程能否交替读写
*/

int main()
{
	pid_t pid = -1;
	int fd = -1,ret;
	int status;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	fd = open("test.txt", O_RDWR|O_CREAT);
	
	if(fd == -1)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	pid = fork();
	
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}

//测试：打开文件再fork，测试交替写
//测试结果：能够实现交替写
#if 0
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		write(fd, "WORLD", strlen("WORLD"));
		
		sleep(1); //等待父进程写完再close
		
		close(fd);
	}
	
	if(pid > 0)
	{
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		printf("父进程 文件描述符ID %d\n", fd);
		write(fd, "HELLO", strlen("HELLO"));
		
		sleep(1); //等待子进程写完再close
		close(fd);
		
		pid = waitpid(pid, &status, 0);  

		if (-1 == pid)
		{
			perror("waitpid");
			exit(1);
		}
	}
#endif


//测试：打开文件再fork，测试交替写
//测试结果：能够实现交替写

#if 1
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		ret = read(fd, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}
		
		sleep(1); //等待父进程写完再close
		close(fd);
	}
	
	if(pid > 0)
	{
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		printf("父进程 文件描述符ID %d\n", fd);
		ret = read(fd, readbuf1, 5);
		if (-1 != ret)
		{
			printf("the readbuf1 is : %s\n", readbuf1);
		}

		sleep(1); //等待子进程写完再close
		close(fd);
		
		pid = waitpid(pid, &status, 0);  
		if (-1 == pid)
		{
			perror("waitpid");
			exit(1);
		}
	}
	
#endif
	
	return 0;
}
