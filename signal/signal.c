#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef void (*sighandler_t)(int);

/*
函数原型：       typedef void (*sighandler_t)(int);
       sighandler_t signal(int signum, sighandler_t handler);
描述：signal用来处理shell和终端处理器产生的错误信号

参数：
signum：表示信号编号，当捕捉到信号时，会传递给捕捉函数，常见的信号如下：
	SIGINT  终端中断，通过CTRL+C、kill -2 pid产生
	SIGKILL 强制终止进程，这个信号不能捕捉和忽略，通过kill -9 pid产生
	SIGHUP 	连接挂断，通过kill -HUP(1) pid产生
	SIGTERM	正常终止进程，与SIGKILL不一样，这个信号可以捕捉和忽略，通过kill pid或者kill -15 pid产生

	SIGALRM 超时警告信号，通过alarm调用产生
	
handler：指定信号的处理方式，有下面三种：
	SIG_IGN 忽略信号
	SIG_DFL 默认处理信号
	
	指定sighandler_t类型的函数用来捕捉处理信号
	
返回值：
返回sighandler_t类型的函数指针，若发生错误返回SIG_ERR

*/

/*
程序功能描述：signal调用的使用（信号的默认处理、捕捉、忽略）
*/
void func(int sig);

int main(void)
{
	sighandler_t ret = (sighandler_t)-2;
#if 0
	/*
	SIGINT信号测试
	测试结果：SIGINT可以捕捉和忽略
	*/

	ret = signal(SIGINT, func);      
	//ret = signal(SIGINT, SIG_IGN); 
	//ret = signal(SIGINT, SIG_DFL); 
#endif
	
#if 0
	/*
	SIGKILL信号测试
	测试结果：SIGKILL只能默认处理，无法捕捉和忽略
	*/
	//ret = signal(SIGKILL, SIG_IGN);   
	ret = signal(SIGKILL, func);	    
	//ret = signal(SIGKILL, SIG_DFL);   
#endif 

#if 0
	/*
	SIGHUP信号测试
	测试结果：SIGHUP可以捕捉和忽略
	*/
	//ret = signal(SIGHUP, SIG_IGN);   
	ret = signal(SIGHUP, func);	    
	//ret = signal(SIGHUP, SIG_DFL);   

#endif

#if 1
	/*
	SIGTERM信号测试
	测试结果：SIGTERM可以捕捉和忽略
	*/
	//ret = signal(SIGTERM, SIG_IGN);   
	ret = signal(SIGTERM, func);	    
	//ret = signal(SIGTERM, SIG_DFL);   
#endif

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
#if 1
	if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT to over process\n");
		//exit(0);//模仿SIGINT默认处理，程序退出8
	}
	
	if (sig == SIGKILL)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGKILL to over process\n");
		//exit(0); 
	}

	if (sig == SIGHUP)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGHUP to hup process\n");
		//exit(0); 
	}

	if (sig == SIGTERM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGTERM to over process\n");
		//exit(0); 
	}
#endif 

#if  0
	if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT to over process\n");

		/*
		这里加个延时测试当信号处理函数执行过程中，如果再次接受到多个SIGINT信号时是怎么处理的
		测试结果：在执行信号处理函数这段期间，多次操作ctrl+c，程序也不会调用信号处理函数，而是在本次
		信号处理函数完成之后，再执行一次信号处理函数（无论前面产生了多少次ctrl+c信号）
		
		原因：程序在执行信号处理函数过程中，发送的ctrl+c信号将会被阻塞，直到信号处理函数执行完成，
		才有机会处理信号函数执行期间产生的ctrl+c,但是在信号函数执行产生的多次ctrl+c，最后只会产生ctrl+c。

		程序后台运行，发送一个SIGHUP，在信号函数执行过程中，发送多个SIGHUP，然后执行fg命令变为前台进程，发送多个SIGINT
		测试结果：每种信号的处理逻辑同上面的测试，处理过程是SIGHUP - SIGINT - SIGINT - SIGHUP， 可以看出信号是栈式处理
		*/
		sleep(10); 
	}

	if (sig == SIGHUP) //kill -HUP 79396
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGHUP to hup process\n");
		sleep(10); 
	}
#endif 
	
}








