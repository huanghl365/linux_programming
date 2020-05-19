#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*
程序功能描述：使用pipe创建管道，并通过管道与子进程中execl启动的程序通信
*/
#if 0  //编译生成pipetest,用于传递管道读端并读取数据
int main(int argc, char *argv[]) 
{
	int pipe_read_fd = -1;
	int ret, i=0;
	char read_buf[50] = "\0";
	
	if (NULL != argv[1])
	{
		sscanf(argv[1], "%d", &pipe_read_fd);
	
		ret = read(pipe_read_fd, read_buf, sizeof(read_buf)-1); //从管道 读端 读取数据
		if (ret != -1)
		{
			printf("read from pipe: %s\n", read_buf);
		}
		else
		{
			perror("read");
			exit(1);
		}
		
	}
	else
	{
		printf("argv[1] is null\n");
		exit(1);
	}
	
	return 0;
}
#endif 

#if 1
int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	char pipe_read_fd_buf[10] = "\0";
	int pid, status;
	ret = pipe(pipe_fd); //创建子进程之前先创建管道，父子进程共享管道
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
		
		sprintf(pipe_read_fd_buf, "%d", pipe_fd[0]);

		//执行一个新的程序，因为调用execl，因此pipetest会继承进程资源比如管道
		execl("./pipetest","pipetest",pipe_read_fd_buf, NULL); 
		exit(0);
	}

	if (pid > 0)
	{
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

