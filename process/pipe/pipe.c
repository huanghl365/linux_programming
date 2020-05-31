#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/*
函数原型：int pipe(int pipefd[2]);
描述：pipe创建一个管道作为单向数据通道，配合fork可以用来做进程间通信。
参数：
pipefd[2]：输出型参数，pipefd用于返回两个引用管道末端的文件描述符。
pipefd[0]是指管道的读取端。pipefd[1]指向管道的写端。
写入管道的写入端的数据由内核缓冲，直到从管道的读取端读取数据为止。 

返回值：
成功时，返回零。如果出错，则返回-1，并正确设置errno。
*/

/*
关于管道通信
1、使用fork复制了文件描述符之后，父子进程都会各自拥有管道的读写端，
因此总共有四个管道端口，这四个端口是共通的，因此父子进程就可以通过管道进行通信
2、一个管道不适合做双向通信，最好是创建两个管道做双向通信。
*/


/*
程序功能描述：使用pipe创建管道，用于父子进程通信
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
	因此总共有四个管道端口，这四个端口是共通的，因此父子进程就可以通过管道进行通信了
	*/
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
	if (0 == pid) 
	{
		//子进程向管道写入数据，父进程从管道读取数据
		sprintf(write_buf, "I am child process, my pid is %d", getpid());
		write(pipe_fd[1], write_buf, strlen(write_buf));  		

		//延时1s再读，避免子进程读取了子进程写入的数据，导致父进程读取不到卡死
		//从这里也可以看出，一个管道不适合做双向通信，最好是创建两个管道做双向通信。
		sleep(1);	

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

