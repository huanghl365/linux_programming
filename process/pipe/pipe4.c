#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>


/*
程序功能描述：使用pipe创建管道，将管道绑定到标准输入输出（子进程将管道绑定到标准输入，父进程将管道绑定到标准输出，execl调用程序从标准输入读取数据）
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

#if 1
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

#if 0	//使用write向管道(标准输出描述符)写数据
		//write(1, "123456789", strlen("123456789"));
		//close(1); 	//关闭标准输出，这样hexdump才能read到0，才能处理数据并退出
#endif


#if 0	//使用printf向管道(标准输出流)写数据
		/*
		疑问:使用printf 向管道写数据，close 写端后 子进程无法读取
		原因推测：使用printf写数据的时候，关闭写端应该要使用fclose关闭而不是close，因为数据是向标准输出流写的。
		而fclose在关闭标准输出流的同时，应该也是会关闭管道的写端，因为管道的写端是绑定到标准输出描述符的，而标准输出流
		应该是通过fdopen打开标准输出的描述符得到的。
		*/
			printf("123456789"); 
			//close(1);
			fclose(stdout);
#endif

#if 1	//打开标准输出描述符，使用fprintf向管道(标准输出流)写数据
		/*
		测试结果：跟使用printf向管道(标准输出流)写数据效果一样
		疑问：问什么重新关闭标准输出描述符之后，向标准输出流写数据之前不用重新打开标准输出流
		原因推测：我们重新绑定设备到标准输出描述符的时候，标准输出流应该是不知道的，它应该只是
		通过识别标准输出描述符输出到设备而已，因此无需重新打开。
		*/
		
		ptr = fdopen(1, "w");
		if (NULL == ptr)
		{
			perror("fdopen");
			exit(1);
		}
		fprintf(ptr, "123456789");
		fclose(stdout);
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



