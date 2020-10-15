#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef void (*sighandler_t)(int);

/*
程序功能描述：signal调用的使用（信号的默认处理、捕捉、忽略）
*/
void func(int sig);

int main(void)
{
	sighandler_t ret = (sighandler_t)-2;
	
/*
SIGINT信号测试
测试结果：SIGINT可以捕捉和忽略
*/
#if 0
	ret = signal(SIGINT, func);      
	//ret = signal(SIGINT, SIG_IGN); 
	//ret = signal(SIGINT, SIG_DFL); 
#endif

/*
SIGKILL信号测试
测试结果：SIGINT可以捕捉和忽略
*/
#if 0
	//ret = signal(SIGKILL, SIG_IGN);   
	ret = signal(SIGKILL, func);	    
	//ret = signal(SIGKILL, SIG_DFL);   
#endif 


/*
SIGHUP信号测试
测试结果：SIGHUP可以捕捉和忽略
*/
#if 0

	//ret = signal(SIGHUP, SIG_IGN);   
	ret = signal(SIGHUP, func);	    
	//ret = signal(SIGHUP, SIG_DFL);   

#endif

/*
SIGTERM信号测试
测试结果：SIGTERM可以捕捉和忽略
*/
#if 1
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
	
}








