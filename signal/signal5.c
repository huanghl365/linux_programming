#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef void (*sighandler_t)(int);

/*
程序功能描述：测试signal执行处理函数时，接收到多个信号时的处理机制
*/
void func(int sig);

int main(void)
{
	sighandler_t ret = (sighandler_t)-2;
	
	ret = signal(SIGINT, func);      
	ret = signal(SIGTERM, func);	    

	if (ret == SIG_ERR)
	{
		perror("signal");
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

/*
测试：这里加个10s延时，测试当信号处理函数执行过程中，如果再次接受到多个SIGINT信号时是怎么处理的
测试结果：在执行信号处理函数这段期间，多次操作ctrl+c，程序也不会调用信号处理函数，而是在本次
信号处理函数完成之后，再执行一次信号处理函数（无论前面产生了多少次ctrl+c信号）
原因：程序在执行信号处理函数过程中，发送的ctrl+c信号将会被阻塞，直到信号处理函数执行完成，
才有机会处理信号函数执行期间产生的ctrl+c,但是在信号函数执行产生的多次ctrl+c，最后只会产生ctrl+c。


测试：程序后台运行，发送一个SIGHUP，在信号函数执行过程中，发送多个SIGHUP，然后执行fg命令变为前台进程，发送多个SIGINT
测试结果：每种信号的处理逻辑同上面的测试，处理过程是SIGHUP - SIGINT - SIGINT - SIGHUP， 可以看出信号是栈式处理
*/

#if  1
	if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT to over process\n");
		sleep(10); 
	}

	if (sig == SIGHUP)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGHUP to hup process\n");
		sleep(10); 
	}
#endif 
	
}








