#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：将同一个文件打开两次，使用O_APPEND关联文件指针，测试能否实现交替读写
*/

#if 0
int main(int argc, char *argv[])
{
	int fd1 = -1; 
	int fd2 = -1;
	int ret = -1;
	char buf[100] = {'\0'};
	
#if 0

	//将一个文件打开两次，推测内存中只有一份
	//使用O_APPEND可以关联两个文件管理表的文件指针，测试能否实现交替写
	fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	
		//fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); 
		//fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); 

	if (-1 == fd1||-1 == fd2)							  						
	{										 							
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("打开文件成功!\n");
	}
	
	
	
	while(1)
	{
		//测试交替写实验
		//测试结果：证明是交替写
		ret = write(fd1, "aa",2);
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd1\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		ret = write(fd2, "bb", 2);
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd2\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
	
		
#else

	//使用O_APPEND可以关联两个文件管理表的文件指针，测试能否实现交替读
	fd1 = open("test.txt", O_RDWR|O_APPEND); 
	fd2 = open("test.txt", O_RDWR|O_APPEND); 
	
	
	if (-1 == fd1||-1 == fd2)							  						
	{										 							
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("打开文件成功!\n");
	}
	
	while(1)
	{
		//测试交替读实验
		//测试结果：O_APPEND无法实现交替读，这里也说明O_APPEND只是用来做接续写操作的
		memset(buf, 0, sizeof(buf));
		ret = read(fd1, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd1：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		memset(buf, 0, sizeof(buf));
		ret = read(fd2, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd2：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
	
#endif
	
	//关闭文件
	ret = close(fd1);
	if(-1 != ret)
		printf("成功关闭文件1!\n");
	
	ret = close(fd2);
	if(-1 != ret)
		printf("成功关闭文件2!\n");
	
	return 0;
	
}
#endif


/*
程序功能描述：使用dup2复制文件描述符，测试能否交替读写
*/

#if 0
int main(int argc, char *argv[])
{
	int fd1 = -1,fd2 = -1;
	int ret;
	char buf[10] = {'0'};
	
	fd1 = open("test.txt", O_RDWR | O_CREAT, 0666);
	if(-1 == fd1)
	{
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("文件打开成功\n");
		printf("文件描述符fd1为：%d\n", fd1);
	}
	
	
	//使用dup2复制文件描述符
	close(4);
	fd2 = dup2(fd1, 4); 
	

#if 0
	//测试能否实现交替写
	//测试结果：能够实现交替写，说明使用dup2可以实现同一文件的不同文件描述符的文件指针关联
	while(1)
	{
		ret = write(fd1, "aa", strlen("aa"));
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd1\n", strlen("aa"));
		}
		else
		{
			perror("写入字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		ret = write(fd2, "bb", strlen("bb"));
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd2\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
#else
	//测试能否实现交替读
	//测试结果：能够实现交替读，说明使用dup2可以实现同一文件的不同文件描述符的文件指针关联
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		ret = read(fd1, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd1：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		memset(buf, 0, sizeof(buf));
		ret = read(fd2, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd2：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
		
	}
#endif
	
	close(fd1);
	close(fd2);
	return 0;
} 


#endif


/*
程序功能描述：父子进程各自打开同一个文件进行操作，测试父子进程能否交替写文件
*/
#if 0
int main()
{
	pid_t pid = -1;
	int fd1 = -1;
	int fd2 = -1;
	int status,ret;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	pid = fork();
	
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
	//测试结果：父子进程确实各自打开同一个文件进行操作，文件指针没有相互关联，实现各自读写
	if(pid == 0)
	{
		//sleep(2);
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		
		fd1 = open("test.txt", O_RDWR|O_CREAT);
		if(fd1 == -1)
		{
			printf("打开文件失败\n");
			exit(1);
		}
		//write(fd1, "WORLD", strlen("WORLD"));
		ret = read(fd1, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}
		close(fd1);
		exit(0);
	}
	
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		fd2 = open("test.txt", O_RDWR|O_CREAT);
		if(fd2 == -1)
		{
			printf("打开文件失败\n");
			//exit(1);   //不应该直接退出，最好回收一下子进程
		}
		else
		{
			//write(fd2, "HELLO", strlen("HELLO"));
			ret = read(fd2, readbuf1, 5);
			if (-1 != ret)
			{
				printf("the readbuf1 is : %s\n", readbuf1);
			}
		}
		close(fd2);
		pid = waitpid(pid, &status, WNOHANG);  

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
程序功能描述：先创建文件，再fork创建子进程，测试父子进程能否交替读写
*/

#if 0
int main()
{
	pid_t pid = -1;
	int fd = -1,ret;
	int status;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	fd = open("test.txt", O_RDWR|O_CREAT);
	//复制完成后生成两个进程，代码变为两份，fork返回两次
	//测试结果：子进程继承父进程的文件指针操作，文件指针确实相互关联实现交替读写
	if(fd == -1)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	pid = fork();
	
	if(pid == -1)
	{
		printf("进程复制失败\n");
		return -1;
	}
	
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		write(fd, "WORLD", strlen("WORLD"));
		/*ret = read(fd, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}*/
		exit(0);
	}
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		printf("父进程 文件描述符ID %d\n", fd);
		write(fd, "HELLO", strlen("HELLO"));
		/*ret = read(fd, readbuf1, 5);
		if (-1 != ret)
		{
			printf("the readbuf1 is : %s\n", readbuf1);
		}*/
		
		pid = waitpid(pid, &status, WNOHANG);  

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
	
	
	close(fd);
	return 0;
}
#endif


/*
程序功能描述：父子进程通过O_APPEND 各自打开同一个文件进行操作，测试父子进程能否交替写文件
*/

#if 0
int main()
{
	pid_t pid = -1;
	int fd1 = -1;
	int fd2 = -1;
	char readbuf[128] = "\0";
	char readbuf1[128] = "\0";
	int status,ret;
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
	//测试结果：不同进程的相同文件的文件指针相互关联，能够实现交替写，但是无法实现交替读
	if(pid == 0)
	{
		//sleep(2);
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());
		
		fd1 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
		if(fd1 == -1)
		{
			printf("打开文件失败\n");
			exit(1);
		}
		//write(fd1, "WORLD", strlen("WORLD"));
		
		ret = read(fd1, readbuf, 5);
		if (-1 != ret)
		{
			printf("the readbuf is : %s\n", readbuf);
		}
		
		close(fd1);
		exit(0);
	}
	if(pid > 0)
	{
		sleep(1);
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
		fd2 = open("test.txt", O_RDWR|O_CREAT|O_APPEND);
		if(fd2 == -1)
		{
			printf("打开文件失败\n");
			//exit(1); //不应该直接退出，最好回收一下子进程
		}
		else
		{
			//write(fd2, "HELLO", strlen("HELLO"));
			
			ret = read(fd2, readbuf1, 5);
			if (-1 != ret)
			{
				printf("the readbuf1 is : %s\n", readbuf1);
			}
		}
		
		pid = waitpid(pid, &status, WNOHANG);  
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
		
		
		close(fd2);
	}
	
	
	return 0;
}
#endif 


