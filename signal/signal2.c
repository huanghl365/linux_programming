#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
