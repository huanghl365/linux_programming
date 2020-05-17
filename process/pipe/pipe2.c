#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*
程序功能描述：创建管道，关闭管道写端，观察从管道读端read的现象
*/

int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	char read_buf[50] = "\0";
	int pid, status;
	int i=0;
	ret = pipe(pipe_fd); //创建子进程之前先创建管道，父子进程共享管道
	if (0 != ret)
	{
		perror("pipe");
		exit(1);
	}
	/*使用fork复制了文件描述符之后，父子进程都会各自拥有管道的读写端，
	因此总共有四个管道端口，这四个端口是共通的，因此父子进程都可以从管道读写数据
	*/
	pid = fork();
	
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
	if (0 == pid) 
	{
		while(i<6)
		{
			sleep(1);
			printf("child pid: %d\n", getpid());
			i++;
		}
		close(pipe_fd[1]);
		exit(1);
	}

	if (pid > 0)
	{
		printf("child pid: %d\n", pid);

		/*
		测试关闭管道写端之后，对read的影响
		测试结果：只关闭父进程的管道写端，子进程还是会阻塞等待，只有同时父子进程的写端，管道才会真正
		无法写入数据，这样read才会返回0。

		如果关闭父进程的读端，然后从父进程的读端读取数据，这时就会报错，返回-1.
		*/
		close(pipe_fd[1]); //关闭父进程管道写端
		//close(pipe_fd[0]);//关闭父进程管道读端
		ret = read(pipe_fd[0], read_buf, sizeof(read_buf)-1);
		if (ret != -1)
		{
			printf("ret = %d\n",ret);
			printf("read from pipe: %s\n", read_buf);
		}
		else
		{
			perror("read");
			exit(1);
		}
		
		/*pid = waitpid(pid, &status, 0);  //阻塞回收子进程
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
		}*/
	}
	
	return 0;
}

