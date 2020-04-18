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
程序功能描述：创建管道，关闭管道写端，观察从管道读端read的现象
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
#if 0
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


/*
程序功能描述：使用pipe创建管道，子进程将管道绑定到标准输入，execl调用程序从标准输入读取数据
*/
#if 0
int main(int argc, char *argv[])  //编译生成一个hello程序，用于测试从标准输入的描述符读取数据
{
	char read_buf[1024] = "\0";
	int ret; 
	ret = read(0, read_buf, sizeof(read_buf)-1);
	if (-1 != ret)
	{
		printf("read the string: %s\n", read_buf);
	}
	else
	{
		perror("read");
		exit(1);
	}
	return 0;
	
}
#endif
#if 0
int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	int pid, status;

	FILE *ptr =NULL;
	
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
		close(0); //关闭标准输入文件描述符，将键盘设备与标准输入解绑

		
		dup(pipe_fd[0]); //复制管道读端描述符，标准输入绑定到新的管道读端描述符

		
		close(pipe_fd[0]); //不需要从老的管道读端读取数据，因此关闭掉

		
		close(pipe_fd[1]); //不需要往写端写数据，因此也关掉，其实这里必须关掉，否者hexdump内部read不会返回0，就会一直卡在读取数据


		//运行一个程序，从标准输入(新的管道读端) 获取数据
		//ret = execlp("od", "od", "-c", NULL);
		//ret = execlp("hexdump", "hexdump", NULL);
		//ret = execlp("cat", "cat", NULL);
		ret = execl("./hello", "hello", NULL);
		if (-1 == ret)
		{
			perror("execl");
			exit(1);
		}
		exit(0);
	}

	if (pid > 0)
	{
		close(pipe_fd[0]); //不需要从管道读端读取数据，因此关闭管道读端

		sprintf(write_buf, "%s", "123456789");
		write(pipe_fd[1], write_buf, strlen(write_buf));


		/*疑问：必须关闭管道写端，hexdump才会处理数据并退出。
		原因：这是因为hexdump会不断从标准输入read数据，只有当hexdump内部read返回0的时候才会停止读取，
		表示读取到文件尾，这时才会处理数据并退出。前面我们已经知道只有当关闭管道写端的时候read的时候才会
		返回0，因此便可以解释这里的现象了。

		当然具体还要看程序内部是怎么read，比如cat命令虽然也需要我们关闭写端才会退出，但cat是读取多少输出多少，
		不需要等待read到0的时候才会处理数据。又比如我们自己写的hello，读多少处理多少并立即退出，不需要我们关闭写端。
		*/
		close(pipe_fd[1]);

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
程序功能描述：使用pipe创建管道，子进程将管道绑定到标准输入，父进程将管道绑定到标准输出，execl调用程序从标准输入读取数据
*/
#if 0
int main(void) //编译生成一个hello，用于测试从标准输入流(注意不是标准输入的描述符)读取数据
{
	FILE *ptr = NULL;
	char read_buf[1024] = "\0";
	
/*
	ptr = fdopen(0, "r");	//测试发现无需重新打开标准输入流
	if (NULL != ptr)
	{
		if (NULL != fgets(read_buf, sizeof(read_buf), stdin))
		if (NULL != fgets(read_buf, sizeof(read_buf), ptr))
		{
			printf("get the string: %s\n", read_buf);
		}
		else
		{
			printf("get string error or get null\n");
			perror("fgets");
			fclose(ptr);
			ptr = NULL;
			exit(1);
		}
		fclose(ptr);
		ptr = NULL;
	}	
	else
	{
		perror("fdopen");
		exit(1);
	}
*/
	
		if (NULL != fgets(read_buf, sizeof(read_buf), stdin))
		{
			printf("get the string: %s\n", read_buf);
		}
		else
		{
			printf("get string error or get null\n");
			perror("fgets");
			fclose(ptr);
			ptr = NULL;
			exit(1);
		}
		fclose(ptr);
		ptr = NULL;

	return 0;
}
#endif 

#if 0
int main(void)
{
	int pipe_fd[2];
	int ret;
	char write_buf[50] = "\0";
	int pid, status;

	FILE *ptr =NULL;
	
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
		
		close(0); //关闭标准输入文件描述符，将键盘设备与标准输入解绑

		
		dup(pipe_fd[0]); //复制管道读端描述符，标准输入绑定到新的管道读端描述符

		
		close(pipe_fd[0]); //不需要从老的管道读端读取数据，因此关闭掉

		
		close(pipe_fd[1]); //不需要往写端写数据，因此也关掉，其实这里必须关掉，否者hexdump内部read不会返回0，就会一直卡在读取数据


		//运行一个程序，从标准输入(新的管道读端) 获取数据
		//ret = execlp("od", "od", "-c", NULL);
		ret = execlp("hexdump", "hexdump", NULL);
		//ret = execlp("cat", "cat", NULL);
		//ret = execl("./hello", "hello", NULL);
		if (-1 == ret)
		{
			perror("execl");
			exit(1);
		}
		exit(0);

	}

	if (pid > 0)
	{
		sleep(1);	

		close(1); //关闭标准输出文件描述符，将显示设备与标准输入解绑
		
		dup(pipe_fd[1]); //复制管道写端描述符，标准输出绑定到新的管道写端描述符

		close(pipe_fd[1]); //不需要向老的管道写端写数据，因此关闭掉
		
	
		close(pipe_fd[0]);	//不需要从管道读端读取数据，因此关闭掉


#if 1	//使用write向管道(标准输出描述符)写数据
		//write(1, "123456789", strlen("123456789"));
		//close(1); 	//关闭标准输出，这样hexdump才能read到0，才能处理数据并退出
#endif

#if 1	//使用printf向管道(标准输出流)写数据


		/*
		疑问:使用printf 向管道写数据，close 写端后 子进程无法读取
		原因推测：使用printf写数据的时候，关闭写端应该要使用fclose关闭而不是close，因为数据是向标准输出流写的。
		而fclose在关闭标准输出流的同时，应该也是会关闭管道的写端，因为管道的写端是绑定到标准输出描述符的，而标准输出流
		应该是通过fdopen打开标准输出的描述符得到的。
		*/
			printf("123456789"); 
			//close(1);
			fclose(stdout);
		/*
		疑问：问什么重新关闭标准输出描述符之后，向标准输出流写数据之前不用重新打开标准输出流
		原因推测：我们重新绑定设备到标准输出描述符的时候，标准输出流应该是不知道的，它应该只是
		通过识别标准输出描述符输出到设备而已，因此无需重新打开。
		*/
		
		/*
		ptr = fdopen(1, "w");
		if (NULL == ptr)
		{
			perror("fdopen");
			exit(1);
		}
		fprintf(ptr, "123456789");
		fclose(stdout);*/
#endif 


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



