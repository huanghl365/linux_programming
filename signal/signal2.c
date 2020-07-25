#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
函数原型：int pause(void);
描述：
让当前进程暂停运行，交出CPU给其他进程去执行。当前进程进入pause状态后当前进
程会表现为“卡住、阻塞住”，要退出pause状态当前进程需要被信号唤醒。


返回值：
当信号发生并且信号处理函数执行完毕时，pause才会返回，并且返回-1，errno设置为EINTR
*/

/*
函数原型：unsigned int alarm(unsigned int seconds);
描述：用来在seconds之后发送一个SIGALRM信号
返回值：
重复调用alarm会重新设置时间并 返回上次alarm设置之后剩余的时间
*/

/*
程序功能描述：使用alarm调用发送SIGALRM信号并捕捉处理
*/

typedef void (*sighandler_t)(int);

void func(int sig)
{
	
	if (sig == SIGALRM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGALRM signal\n");
	}
	/*
	测试：这里加个延时，测试 pause是否需要等待信号处理函数执行完毕才返回
	测试结果：测试发现是需要的
	*/
	sleep(2); 
}

int main(void)
{
	sighandler_t ret = (sighandler_t)-2;
	int ret2;
	ret = signal(SIGALRM, func);    
	if (ret == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	
	alarm(5);
	sleep(2);
	ret2 = alarm(5); //重新设置时间
	printf("the left alarm time is %d s\n", ret2);
	

	ret2 = pause();  //挂起进程等待信号
	if (-1 == ret2)  //当信号发生并且信号处理函数执行完毕时，pause才会返回
	{
		perror("pause");
		printf("signal was caught and the signal-catching function returned\n");
		exit(1);
	}

	return 0;
}
