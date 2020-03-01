#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>


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
#if 0
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
		/*子进程向管道写入数据，父进程从管道读取数据*/
		sprintf(write_buf, "I am child process, my pid is %d", getpid());
		write(pipe_fd[1], write_buf, strlen(write_buf));  		

	
		sleep(1);	//延时1s再读，避免读取了上面写入的数据，导致父进程读取不到卡死

		/*父进程向管道写入数据，子进程从管道读取数据*/
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
		printf("child pid: %d\n", pid);
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

/*
程序功能描述：创建管道，再进行关闭管道测试
*/
#if 0

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

#endif



/*
程序功能描述：使用pipe创建管道，并与子进程中execl启动的进程进行通信
*/
#if 0
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
#else
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

		//执行一个新的程序，因为调用execl，因此子进程
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
#endif 


/*
程序功能描述：使用pipe创建管道，并将管道绑定到标准输入输出
*/

#if 0
int main(void)
{
	
	return 0;
}
#endif
