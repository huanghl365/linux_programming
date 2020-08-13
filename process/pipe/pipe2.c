#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*
程序功能描述：创建管道，测试关闭管道后的read操作
*/

int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	char read_buf[50] = "\0";
	int pid, status;
	int i=0;
	ret = pipe(pipe_fd); 
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
	
/*
测试：关闭父进程和子进程的写端，观察read操作
测试结果：
在没有关闭父进程的写端时，read操作会阻塞，只有当父子进程的写端都关闭，才会认为管道已经关闭，
这时read读取不到数据，就会返回0
*/

#if 1
	if (0 == pid) 
	{
		while(i<6)
		{
			sleep(1);
			printf("child pid: %d\n", getpid());
			i++;
		}
		close(pipe_fd[1]);  //关闭子进程管道写端
		exit(1);
	}

	if (pid > 0)
	{
		printf("child pid: %d\n", pid);
		
		close(pipe_fd[1]); 		//关闭父进程管道写端
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

