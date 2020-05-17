#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：父子进程通过O_APPEND 各自打开同一个文件进行操作，测试父子进程能否交替写文件
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
	
	//测试结果：不同进程的相同文件的文件指针相互关联，能够实现交替写，但是无法实现交替读
	if(pid == 0)
	{
		//sleep(2);
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		
		fd1 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
		if(fd1 == -1)
		{
			printf("打开文件失败\n");
			exit(1);
		}
		//write(fd1, "WORLD", strlen("WORLD"));
		
		ret = read(fd1, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}
		
		close(fd1);
		exit(0);
	}
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		fd2 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
		if(fd2 == -1)
		{
			printf("打开文件失败\n");
			//exit(1); //不应该直接退出，最好回收一下子进程
		}
		else
		{
			//write(fd2, "HELLO", strlen("HELLO"));
			
			ret = read(fd2, readbuf1, 5);
			if (-1 != ret)
			{
				printf("the readbuf1 is : %s\n", readbuf1);
			}
		}
		
		pid = waitpid(pid, &status, WNOHANG);  
		if (-1 != pid)
		{
			printf("parent:父进程回收的子进程ID：%d\n", pid);
			printf("parent:子进程是否正常终止：%d\n", WIFEXITED(status));
			printf("parent:子进程是否非正常终止：%d\n", WIFSIGNALED(status));   
			printf("parent:子进程终止退出码：%d\n", WEXITSTATUS(status));
		}
		else
		{
			perror("waitpid");
			exit(1);
		}
		
		
		close(fd2);
	}
	
	
	return 0;
}