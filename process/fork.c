
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
#include <stdlib.h>


/*
关于进程：由程序代码、数据(静态变量和全局变量)、变量（栈变量和堆内存）、打开的文件（文件描述符）和环境组成。
同个程序的多个进程之间共享内存中的代码和函数库，因此内存只会存在这个程序的一份代码。

关于进程表：进程表就像一个数据结构，把当前加载在内存中的所有进程的有关信息保存在一个表中，
其中包括进程的PID、进程的状态、命令字符串和一些ps命令输出的各类信息。
*/

/*
函数原型：pid_t fork(void);
描述：fork用来复制当前进程创建一个新的子进程。

fork复制当前进程代码、数据、变量，复制之后变为两份，但是当前进程打开的文件是共享的。

fork会在进程表中创建一个新的子进程的表项，新表项中的许多属性与当前进程是相同的。


返回值：
fork函数调用一次会返回2次，返回值等于0的就是子进程，而返回值大于0的就是父进程，父子
进程会被内核同等调度，在各自的进程中做各自的事情
*/


/*
程序功能描述：使用fork创建子进程并回收子进程
*/

#if 1
int globalvar = 9;
int main()
{
	pid_t pid = -1;
	int status = 0;
	int var = 10;
	static int staticvar = 8;
	
	char *dymem = (char *)malloc(sizeof(char) * 20);

	if (NULL != dymem)
	{
		memset(dymem, 0, sizeof(char) * 20);
		sprintf(dymem, "origin string");
	}
	else
	{
		perror("malloc");
		exit(1);
	}
	
	
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
	if(pid == 0)
	{
		printf("子进程ID为：%d\n", getpid());
		printf("在子进程中,父进程ID为：%d\n", getppid());

		/*
		测试子进程有没有复制父进程的变量和数据
		测试结果：从打印上来看是有复制的，但是父子进程中打印同个变量地址发现是相同的，
		这是因为使用了"写时复制"，具体不深入研究。
		*/
#if 1
		var = 9; 
		globalvar = 8;
		staticvar = 7;

		if (dymem != NULL)
		{
			memset(dymem, 0, sizeof(char) * 20);
			sprintf(dymem, "new string");
			printf("dymem = %s %p\n", dymem,dymem );
			free(dymem);
			dymem = NULL;
		}
	
		printf("子进程var = %d %p\n",var, &var);
		printf("子进程globalvar = %d %p\n",globalvar, &globalvar);
		printf("子进程staticvar = %d %p\n",staticvar, &staticvar);
#endif
		exit(0);
	}
	
	if(pid > 0)
	{
		
		sleep(3);	//因为下面的waitpid使用非阻塞，因此这里要适当延时等待子进程执行完毕，这样才能顺利回收子进程
		printf("父进程ID为：%d\n", getpid());
		printf("在父进程中,子进程ID为：%d\n", pid);
		
#if 1
		//while(1)
		//{
			if (NULL != dymem)
			{
				printf("dymem = %s %p\n", dymem,dymem );
				free(dymem);
				dymem = NULL;
			}
			printf("父进程var = %d %p\n",var, &var);
			printf("父进程globalvar = %d %p\n",globalvar, &globalvar);
			printf("父进程staticvar = %d %p\n",staticvar, &staticvar);
			//sleep(1);
		//}
#endif 		
		pid = waitpid(pid, &status, WNOHANG);  //使用waitpid回收指定进程，WNOHANG表示非阻塞
		printf("parent:父进程回收的子进程ID：%d\n", pid);
		if (-1 != pid)
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
		
	

	/*
	测试子进程有没有复制父进程的代码
	测试结果：是有复制的，因为代码在内存中变为两份，所以这里的helloworld会被输出两次,并且PID值为父子进程各自的PID
	*/
	printf("进程PID = %d helloworld\n", getpid());

	return 0;
}
#endif







