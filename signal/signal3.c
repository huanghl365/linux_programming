#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
函数原型：int kill(pid_t pid, int sig);
描述：kill可以用来包括他本身在内的 其他进程发送一个进程；
发送的进程必须拥有相应的权限，这意味两个进程必须拥有相同的用户ID或者发送信号的进程是超级用户

参数：
pid：指定要发送信号的进程PID
sig：指定要发送的信号。如果sig为0，则不发送信号，但仍执行错误检查，
这可用于检查是否存在pid对应的进程或进程组。

返回值：
调用成功返回0，失败返回-1，失败的原因可能有以下几种：
	EINVAL 给定的信号无效
	EPERM 进程权限不够
	ESRCH 目标进程不存在
*/


/*
程序功能描述：子进程使用kill调用发送信号，父进程捕捉并处理
*/
typedef void (*sighandler_t)(int);
void func(int sig)
{
	if (sig == SIGALRM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGALRM signal\n");
	}
	else if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT signal\n");
		//exit(0);//模仿SIGINT默认处理，程序退出
	}
	else if (sig == SIGKILL)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGKILL signal\n");
		//exit(0); //模仿SIGKILL默认处理，程序退出
	}
}

int main(void)
{
	pid_t pid;
	sighandler_t ret = (sighandler_t)-2;
	int ret2;
	int status;
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}

//测试使用kill 发送0 和 SIGALRM信号
#if 1
	if (0 == pid) 
	{
		sleep(2);
		/*
		测试：测试发送0
		测试：kill 的sig等于0，不发送信号，用来检查进程是否存在
		*/
		ret2 = kill(getppid(), 0); 
		if (0 == ret2)
		{
			printf("the process %d is exist\n", getppid());
		}
		else
		{
			perror("kill");
			exit(1);
		}


		/*
		测试：给父进程发送SIGALRM信号
		测试结果：
		
		*/
		sleep(2);
		ret2 = kill(getppid(), SIGALRM);
		if (-1 == ret2)
		{
			perror("kill");
			exit(1);
		}
		exit(0);
	}


	if (pid > 0)
	{
		ret = signal(SIGALRM, func);   
		//ret = signal(SIGALRM, SIG_DFL); 
		//ret = signal(SIGALRM, SIG_IGN);   //如果忽略信号的话那么父进程会一直卡死在pause那里
		
		if (ret == SIG_ERR)
		{
			perror("signal");
			exit(1);
		}
		
		pause();  


		pid = waitpid(pid, &status, 0);  
		printf("parent:父进程回收的子进程ID：%d\n", pid);
		if (-1 == pid) 
		{
			perror("waitpid");
			exit(1);
		}
	}
#endif 


//测试使用kill 发送SIGKILL
#if 1
	if (0 == pid)
	{
		
		sleep(5);
		ret2 = kill(getppid(), SIGKILL); //给父进程发送SIGKILL信号
		if (-1 == ret2)
		{
			perror("kill");
			exit(1);
		}

		/*
		测试：这里加个while，测试发送SIGKILL给父进程终止后子进程是否终止
		测试结果：子进程不会终止
		*/
		while(1) 
		{
			printf("子进程: %d\n", getpid());
			sleep(1);
		}
	}

	if (pid > 0)
	{
		ret = signal(SIGKILL, func);   //SIGKILL信号是无法捕获的
		if (ret == SIG_ERR)
		{
			perror("signal");
			exit(1);
		}
		
		/*
		测试：这里加个while，看接收到SIGKILL后父进程是否终止
		测试结果：父进程会终止
		*/
		while(1)	
		{
			printf("父进程: %d\n", getpid());
			sleep(1);
		}
		
	}
#endif 

	return 50;
}
