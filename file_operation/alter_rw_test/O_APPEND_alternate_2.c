#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
程序功能描述：父子进程O_APPEND 方式各自打开同一个文件进行操作，测试父子进程能否交替写文件
*/
int main()
{
	pid_t pid = -1;
	int fd1 = -1;
	int fd2 = -1;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	int status,ret;
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}

/*
测试：父子进程O_APPEND 方式各自打开同一个文件进行写
测试结果：可以实现交替写
*/
#if 0
		if(pid == 0)
		{
			printf("子进程ID为：%d\n", getpid());
			printf("在子进程中,父进程ID为：%d\n", getppid());
			
			fd1 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
			if(fd1 == -1)
			{
				printf("打开文件失败\n");
				exit(1);
			}
			write(fd1, "WORLD", strlen("WORLD"));

			
			sleep(1);//等待父进程写完再close
			close(fd1);
		}
		if(pid > 0)
		{
			printf("父进程ID为：%d\n", getpid());
			printf("在父进程中,子进程ID为：%d\n", pid);
			
			fd2 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
			if(fd2 == -1)
			{
				printf("打开文件失败\n");
				exit(1);
			}
			else
			{
				write(fd2, "HELLO", strlen("HELLO"));
			}

			sleep(1);//等待子进程写完再close			
			close(fd2);
			
			pid = waitpid(pid, &status, 0);  
			if (-1 == pid)
			{
				perror("waitpid");
				exit(1);
			}			
		}
#endif 

/*
测试：父子进程O_APPEND 方式各自打开同一个文件进行读
测试结果：无法实现交替读
*/
#if 1
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		
		fd1 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
		if(fd1 == -1)
		{
			printf("打开文件失败\n");
			exit(1);
		}
		
		ret = read(fd1, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}

		
		sleep(1); //等待父进程读完再close
		close(fd1);
	}
	if(pid > 0)
	{
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		fd2 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
		if(fd2 == -1)
		{
			printf("打开文件失败\n");
			exit(1);
		}
		else
		{
			
			ret = read(fd2, readbuf1, 5);
			if (-1 != ret)
			{
				printf("the readbuf1 is : %s\n", readbuf1);
			}
		}

		sleep(1);//等待子进程读完再close			
		close(fd2);
		
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
