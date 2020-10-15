 #include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

/*
程序功能描述：测试exec一系列调用的使用
*/

#if 1
int main(int argc, char *argv[])
{
	extern char **environ;
	pid_t pid = -1;
	pid_t ret = -1;
	int status;		 //获取pid的状态
	pid = fork();
	if(pid < 0)
	{
		printf("复制进程失败\n");
		exit(1);
	}
	
	if(pid == 0)
	{
		printf("child:子进程ID：%d\n", getpid());

//测试execl	
#if 1
		//execl("/bin/ls","ls","-l","-a",NULL);
		execl("./hello", "program name:hello", "HELLO", "WORLD", NULL);
#endif


//测试execv
#if 1
		//char *const ptrarray[] = {"ls", "-i","-a",NULL};
		//execv("/bin/ls", ptrarray);

		char *const ptrarray[] = {"program name:hello", "HELLO", "WORLD",NULL};
		execv("./hello", ptrarray);
#endif


//测试execlp	
#if 1
		//execlp("ls","ls","-l","-a",NULL); 		
		char *const ptrarray[] = {"ls", "-i","-a",NULL};
		execvp("ls", ptrarray);
#endif


//测试execvp	
#if 1
		//execlp("hello", "program name:hello", "HELLO", "WORLD", NULL);
		char *const ptrarray[] = {"program name:hello", "HELLO", "WORLD",NULL};
		execvp("hello", ptrarray);
#endif


//测试execle
#if 1
		//execle("hello", "program name:hello", "HELLO", "WORLD", NULL, environ);
		
		//自己定义传递给执行程序的环境变量，不使用从父进程继承的环境变量
		char *const envp[] = {"AA=aaaa", "XX=bbbb", NULL};
		execle("./hello", "program name:hello", "HELLO", "WORLD", NULL, envp); 
#endif
	
		exit(0);
	
	}
	
	if(pid > 0)
	{
		ret = waitpid(pid, &status, 0);  //阻塞回收
		if (-1 != ret)
		{
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
程序功能描述：编译一个hello可执行程序，用于测试exec
*/
#if 0
int main(int argc, char *argv[], char *env[])
{
	int i = 0;
	printf("coming to hello: argc=%d\n", argc);
	while(argv[i] != NULL)
	{
		printf("argv[%d]=%s\n", i, argv[i]);
		i++;
	}
	i = 0;
	while(env[i] != NULL)
	{
		printf("%s\n",env[i]);
		i++;
	}

	/*
	测试execl执行的程序是不是继承了子进程的pid，以及父进程回收的是子进程还是execl执行的程序
	测试结果：继承了子进程的pid，并且父进程回收的是execl执行的程序
	*/
	while(i<5)
	{
		printf("hello pid: %d\n", getpid());
		sleep(1);
		i++;
	}
	
	return 0;
	//return 20;
}
#endif

