#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
函数原型：int pipe(int pipefd[2]);
描述：pipe创建一个管道，一个单向数据通道，可用于进程间通信。
参数：
pipefd[2]：输出型参数，pipefd用于返回两个引用管道末端的文件描述符。
pipefd[0]是指管道的读取端。pipefd[1]指向管道的写端。
写入管道的写入端的数据由内核缓冲，直到从管道的读取端读取数据为止。 
返回值：
成功时，返回零。如果出错，则返回-1，并正确设置errno。
*/

/*
程序功能描述：使用pipe创建管道，用于父子进程通信
*/
#if 1
int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	char read_buf[50] = "\0";
	int pid, status;
	ret = pipe(pipe_fd); //创建子进程之前先创建管道，之后父子进程共享
	if (0 != ret)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
	if (0 == pid) 
	{
		sprintf(write_buf, "I am child process, my pid is %d", getpid());
		write(pipe_fd[1], write_buf, strlen(write_buf));  //子进程向管道写入数据

		
		sleep(1);	//延时1s再读，避免读取了上面写入的数据，导致父进程读取不到卡死
		ret = read(pipe_fd[0], read_buf, sizeof(read_buf)-1); //直接操作read_buf，因为fork会复制变量
		if (ret != -1)
		{
			printf("read from pipe: %s\n", read_buf);
		}
		else
		{
			perror("read");
			exit(1);
		}
		
		exit(0);
	}

	if (pid > 0)
	{
		printf("child pid: %d\n", pid);
		ret = read(pipe_fd[0], read_buf, sizeof(read_buf)-1); //父进程从管道读取数据
		if (ret != -1)
		{
			printf("read from pipe: %s\n", read_buf);
		}
		else
		{
			perror("read");
			exit(1);
		}

		sprintf(write_buf, "I am parent process, my pid is %d", getpid()); //直接操作write_buf，因为fork会复制变量
		write(pipe_fd[1], write_buf, strlen(write_buf)); 
		pid = waitpid(pid, &status, 0);  //阻塞回收子进程
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
	
	return 0;
}
#endif



