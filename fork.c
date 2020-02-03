#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>


/*
程序功能描述：使用fork创建子进程并回收子进程
*/

#if 0
int main()
{
	pid_t pid = -1;
	int status;
	//fork复制完成后生成两个进程，在内存中代码变为两份
	//fork函数调用一次会返回2次，返回值等于0的就是子进程，而返回值大于0的就是父进程，父子进程会被内核同等调度，在各自的进程中做各自的事情
	pid = fork();
	
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
	}
	
	if(pid > 0)
	{
		
		sleep(1);	//因为下面的waitpid使用非阻塞，因此这里要适当延时等待子进程执行完毕，这样才能顺利回收子进程
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		pid = waitpid(pid, &status, WNOHANG);  //使用waitpid回收指定进程，WNOHANG表示非阻塞
		printf("parent:父进程回收的子进程ID：%d\n", pid);
		if (-1 != pid)
		{
			/*
			WIFEXITED(status)：进程正常终止，则取非零值
			WIFSIGNALED(status)：被未捕获的信号终止，则取非零值
			WEXITSTATUS(status)：取子进程传给exit或_exit的低8位。一般会先用 WIFEXITED 来判断是否正常结束才能使用此宏
			注意：如果waitpid指定回收的进程不存在，那么这些宏返回的值跟进程正常终止时是一样的,因此最好是
			判断waitpid的返回值，确认进程正常回收才对进程的状态做进一步的判断。
			*/
			printf("parent:子进程是否正常终止：%d\n", WIFEXITED(status));    
			printf("parent:子进程是否非正常终止：%d\n", WIFSIGNALED(status));   
			printf("parent:子进程终止退出码：%d\n", WEXITSTATUS(status)); 
		}
		else
		{
			perror("waitpid");
		}
	}
	if(pid == -1)
	{
		printf("进程复制失败\n");
	}
	//因为代码在内存中变为两份，所以这里的helloworld会被输出两次
	printf("helloworld\n");

	return 8;
}
#endif


/*
程序功能描述：先创建文件，再fork创建子进程，测试父子进程能否交替读写
*/

#if 0
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
	}
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		write(fd, "HELLO", strlen("HELLO"));
		/*ret = read(fd, readbuf1, 5);
		if (-1 != ret)
		{
			printf("the readbuf1 is : %s\n", readbuf1);
		}*/
		
		pid = waitpid(pid, &status, WNOHANG);  

		if (-1 == pid)
		{
			printf("parent:父进程回收的子进程ID：%d\n", pid);
			printf("parent:子进程是否正常终止：%d\n", WIFEXITED(status));
			printf("parent:子进程是否非正常终止：%d\n", WIFSIGNALED(status));   
			printf("parent:子进程终止退出码：%d\n", WEXITSTATUS(status));
		}
		else
		{
			perror("waitpid");
		}
		
		
	}
	if(pid == -1)
	{
		printf("进程复制失败\n");
	}
	
	close(fd);
	return 0;
}
#endif

/*
程序功能描述：父子进程各自打开同一个文件进行操作，测试父子进程能否交替写文件
*/
#if 0
int main()
{
	pid_t pid = -1;
	int fd1 = -1;
	int fd2 = -1;
	int status,ret;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	pid = fork();
	
	//测试结果：父子进程确实各自打开同一个文件进行操作，文件指针没有相互关联，实现各自读写
	if(pid == 0)
	{
		//sleep(2);
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		
		fd1 = open("test.txt", O_RDWR|O_CREAT);
		if(fd1 == -1)
		{
			printf("打开文件失败\n");
			exit(-1);
		}
		//write(fd1, "WORLD", strlen("WORLD"));
		ret = read(fd1, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}
		close(fd1);
	}
	
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		fd2 = open("test.txt", O_RDWR|O_CREAT);
		if(fd2 == -1)
		{
			printf("打开文件失败\n");
			//exit(-1);   //不应该直接退出，最好回收一下子进程
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
		}
		
		
		close(fd2);
	}
	if(pid == -1)
	{
		printf("进程复制失败\n");
	}
	
	
	return 0;
}
#endif


/*
程序功能描述：父子进程通过O_APPEND 各自打开同一个文件进行操作，测试父子进程能否交替写文件
*/

#if 1
int main()
{
	pid_t pid = -1;
	int fd1 = -1;
	int fd2 = -1;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	int status,ret;
	pid = fork();
	
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
			exit(-1);
		}
		//write(fd1, "WORLD", strlen("WORLD"));
		
		ret = read(fd1, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}
		
		close(fd1);
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
			//exit(-1); //不应该直接退出，最好回收一下子进程
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
		}
		
		
		close(fd2);
	}
	if(pid == -1)
	{
		printf("进程复制失败\n");
	}
	
	return 0;
}
#endif 






