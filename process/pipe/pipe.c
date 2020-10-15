#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*
程序功能描述：使用pipe创建管道，fork复制管道，测试父子进程通信
*/

int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	char read_buf[50] = "\0";
	int pid, status;
	int i=0;
	ret = pipe(pipe_fd); //创建管道
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
测试：测试通过一个管道进行父子进程单向数据通信
*/

#if 1
		if (0 == pid) 
		{
			close(pipe_fd[0]); //关闭读端
			
			sprintf(write_buf, "I am child process, my pid is %d", getpid());
			write(pipe_fd[1], write_buf, strlen(write_buf));		
			exit(0);
		}
	
		if (pid > 0)
		{
			close(pipe_fd[1]); //关闭写端
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


/*
测试通过一个管道进行父子进程双向通信
测试结果：管道作为单向数据通道，不应该用一个管道进行父子进程双向通信，
容易造成通信数据错乱，正确的做法是使用两个管道做双向通信。
*/
#if 0
	if (0 == pid) 
	{
		//子进程向管道写入数据，父进程从管道读取数据
		sprintf(write_buf, "I am child process, my pid is %d", getpid());
		write(pipe_fd[1], write_buf, strlen(write_buf));  		


		//延时1s再读，避免子进程读取了子进程写入的数据，导致父进程读取不到卡死
		//sleep(1);	


		//父进程向管道写入数据，子进程从管道读取数据
		ret = read(pipe_fd[0], read_buf, sizeof(read_buf)-1);   
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

		sprintf(write_buf, "I am parent process, my pid is %d", getpid()); 
		write(pipe_fd[1], write_buf, strlen(write_buf)); 
		
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

