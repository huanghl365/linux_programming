#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*
程序功能描述：创建管道，关闭父子进程的管道写端，观察从管道读端read的现象
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
测试关闭父子进程管道写端之后，对read的影响
测试结果：

只关闭父进程的管道写端，父进程读端读数据会阻塞，只有等子进程也关闭管道写端
管道才会真正无法写入数据，这时父进程读端才会返回0。
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

