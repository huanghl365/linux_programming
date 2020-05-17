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
	//复制完成后生成两个进程，代码变为两份，fork返回两次
	//测试结果：子进程继承父进程的文件指针操作，文件指针确实相互关联实现交替读写
	if(fd == -1)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	pid = fork();
	
	if(pid == -1)
	{
		printf("进程复制失败\n");
		return -1;
	}
	
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		write(fd, "WORLD", strlen("WORLD"));
		/*ret = read(fd, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}*/
		exit(0);
	}
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		printf("父进程 文件描述符ID %d\n", fd);
		write(fd, "HELLO", strlen("HELLO"));
		/*ret = read(fd, readbuf1, 5);
		if (-1 != ret)
		{
			printf("the readbuf1 is : %s\n", readbuf1);
		}*/
		
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
		
		
	}
	
	
	close(fd);
	return 0;
}