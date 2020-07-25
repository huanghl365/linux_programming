#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
程序功能描述：测试sigaction的sa_flags属性
*/

/*
sa_flags属性：

SA_RESETHAND	当调用信号处理函数时，将信号的处理函数重置为缺省值SIG_DFL
SA_RESTART		如果信号中断了进程的某个系统调用，那么信号处理函数执行完成后，系统调用将重启而不是被信号中断
SA_NODEFER 		一般情况下， 当信号处理函数运行时，内核将阻塞该给定信号。但是如果设置了 SA_NODEFER标记， 
				那么在该信号处理函数运行时，内核将不会阻塞该信号
*/

typedef void (*sighandler_t)(int);

void func(int sig);

int main(void)
{
	int ret = -1;

	struct sigaction act = {0};
	act.sa_handler = func;

	
	/*
	测试：给sa_flags中添加SA_NODEFER属性
	测试结果：
	处理过程中，如果接受到新的信号，会立即退出信号处理，进行新信号的处理
	*/
#if 0
	act.sa_flags = SA_NODEFER; 
#endif

	/*
	测试：给sa_flags中添加SA_RESETHAND属性
	测试结果：第一次发送ctrl+c，会捕捉并调用信号处理函数，之后信号的处理函数重置为缺省值SIG_DFL，因此在此发送ctrl + c程序中断
	*/
#if 1
	act.sa_flags = SA_RESETHAND; 
#endif

	
	ret = sigaction(SIGINT, &act, NULL);


	if (-1 == ret)
	{
		perror("sigaction");
		exit(1);
	}
	
	while(1)
	{
		printf("the process is running\n");
		sleep(1);
	}
}

void func(int sig)
{
	if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT to over process\n");
		//exit(0);//模仿SIGINT默认处理，程序退出

		sleep(5);
	}
}




