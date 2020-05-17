#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
程序功能描述：使用signal调用和alarm调用封装自己的sleep函数
*/
typedef void (*sighandler_t)(int);
void func(int sig)
{
	if (sig == SIGALRM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGALRM signal\n");
	}
}

//限制一下睡眠时间：1-255s
void mysleep(unsigned int seconds)
{
	sighandler_t ret = (sighandler_t)-2;

	if (seconds < 1 || seconds  > 255)
	{
		printf("sleep time ranges: 1-255\n");
		return;
	}
	ret = signal(SIGALRM, &func);

	if (ret == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	
	alarm(seconds);

	pause(); //等待alarm 时间结束发送SIGALRM
}

int main(void)
{
	int i;
	for (i=0; i<5; i++)
	{
		
		printf("sleeptime %d\n", i);
		mysleep(i);
	}
	return 0;
}
