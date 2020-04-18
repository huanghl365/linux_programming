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

#if 1
#if 1
/*
程序功能描述：编译一个hello可执行程序，用于测试exec
*/
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
#if 1
/*
程序功能描述：测试exec一系列调用的使用
*/
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
	#if 0
		/****************execl********************/
		//参数1为可执行程序的路径名，参数2任意字串都可以，其余参数为可执行程序的传参，并且以NULL结束
		//execl("/bin/ls","ls","-l","-a",NULL);
	
		execl("/home/mxc/winshare/my_github/linux-programming/hello", "program name:hello", "HELLO", "WORLD", NULL);
	#endif
	
	#if 0
		/****************execv*****************/
		//与execl类似，参数1为可执行程序的路径名，参数2是一个传参的指针数组
		//char *const ptrarray[] = {"ls", "-i","-a",NULL};
		//execv("/bin/ls", ptrarray);
	
		char *const ptrarray[] = {"program name:hello", "HELLO", "WORLD",NULL};
		execv("/home/mxc/winshare/my_github/linux-programming/hello", ptrarray);
	#endif
		
		
	#if 0
		/****************execlp和execvp****************/
		/*与前面的execl和execv类似，参数1传参不需要指明程序路径，函数会去环境变量PATH中查找程序，
		因此使用execl和execv必须将要执行的程序的路径设置到PATH中，否则查找不到程序*/
		
		
		//execlp("ls","ls","-l","-a",NULL); 		
		//char *const ptrarray[] = {"ls", "-i","-a",NULL};
		//execvp("ls", ptrarray);

		//execlp("hello", "program name:hello", "HELLO", "WORLD", NULL);
		char *const ptrarray[] = {"program name:hello", "HELLO", "WORLD",NULL};
		execvp("hello", ptrarray);
	#endif
		
	#if 1
		/*************execle****************/
		/*与前面的execl和execv类似，最后一个参数为环境变量，给调用程序中的main函数传递环境变量作为输入型参数*/
		//execle("hello", "program name:hello", "HELLO", "WORLD", NULL, environ);
		
		//自己定义传递给调用程序的环境变量，不使用从父进程继承的环境变量
		char *const envp[] = {"AA=aaaa", "XX=bbbb", NULL};
		execle("/home/mxc/winshare/my_github/linux-programming/hello", "program name:hello", "HELLO", "WORLD", NULL, envp); 
	#endif
	
		exit(0);
	
	}
	
	if(pid > 0)
	{
		sleep(1);							   //因为下面的waitpid使用非阻塞，因此这里要适当延时，才能顺利回收子进程
		ret = waitpid(pid, &status, WNOHANG);  //使用waitpid回收指定进程，WNOHANG表示非阻塞
		//ret = waitpid(2233, &status, WNOHANG);

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
#endif 



/*
程序功能描述：使用freopen重定向文件流为标准输入，给execl打开的程序输入数据，将小写字母替换为大写字母
*/
#if 0
int main(int argc, char *argv[])
{

	//1、编译出toupper.out,用来将小写字母翻译成大写
#if 0 
	int ch;
	while ((ch = getchar()) != EOF)
	{
		putchar(toupper(ch));
	}
#endif 

	//2、将文件流重定向到标准输入，使用execl调用toupper.out
#if 1
	FILE *fptr = NULL;
	if (argc != 2)
	{
		printf("usage: ./a.out filename");
		exit(1);
	}

	fptr = freopen(argv[1], "r", stdin); //打开文件，并且重新打开标准输入，将文件流绑定到标准输入
	if (fptr == NULL)
	{
		perror("freopen:");
		exit(2);
	}
	else
	{
		//已经打开的文件流在执行execl之后会保留下来,因此文件流会通过标准输入重定向给toupper.out
		execl("./toupper.out", "toupper.out", NULL);
		printf("done");
		exit(3);
	}
#endif 

	return 0;

}
#endif

