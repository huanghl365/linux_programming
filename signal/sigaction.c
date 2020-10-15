#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
程序功能描述：测试sigaction调用的使用（信号的默认处理、捕捉、忽略）
*/

typedef void (*sighandler_t)(int);

void func(int sig);

int main(void)
{
	int ret = -1;

	struct sigaction act = {0};

	act.sa_handler = func;		//捕捉
	//act.sa_handler = SIG_DFL;	//默认
	act.sa_handler = SIG_IGN;	//忽略
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




