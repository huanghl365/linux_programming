#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
函数原型：int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);
描述：跟signal一样用来处理shell和终端处理器产生的错误信号，但比起signal更加健壮
参数：
signum：表示信号编号，当捕捉到信号时，会传递给捕捉函数，常见的信号如下：
	SIGINT 终端中断，通过ctrl + c产生
	SIGKILL 终止进程，这个信号不能捕捉和忽略，通过kill或者killall生成
	SIGALRM  超时警告信号，可以通过alarm调用或者kill调用发起这个信号
	SIGHUP 	连接挂断，这个信号可以通过<kill -HUP pid> 发起
	
act：配置信号的处理方式
struct sigaction
{
    void(*sa_handler)(int);		//信号处理函数，
    void(*sa_sigaction)(int, siginfo_t *, void *); 
    sigset_t sa_mask;
    int sa_flags;
    void(*sa_restorer)(void);
};
结构体参数说明：
	sa_handler：此参数和signal()的参数handler相同，代表新的信号处理函数
	
	sa_sigaction：
		信号处理函数可以采用void (*sa_handler)(int)或void (*sa_sigaction)(int, siginfo_t *, void *)。
		到底采用哪个要看sa_flags中是否设置了SA_SIGINFO位，如果设置了就采用void (*sa_sigaction)(int, siginfo_t *, void *)，
		此时可以向处理函数发送附加信息；默认情况下采用void (*sa_handler)(int)，此时只能向处理函数发送信号的数值。

	sa_mask：用来设置在处理该信号时暂时将sa_mask 指定的信号集屏蔽

	sa_flags：用来设置信号处理的其他相关操作
		SA_RESETHAND：	当调用信号处理函数时，将信号的处理函数重置为缺省值SIG_DFL
		SA_RESTART：		如果信号中断了进程的某个系统调用，那么信号处理函数执行完成后，系统调用将重启而不是被信号中断
		SA_NODEFER ：	一般情况下， 当信号处理函数运行时，内核将阻塞该给定信号。但是如果设置了 SA_NODEFER标记， 
						那么在该信号处理函数运行时，内核将不会阻塞该信号

	sa_restorer：此参数没有使用。    
	
oldact：用来备份旧的信号处理方式，一般设置为NULL

返回值：
成功返回0，失败返回-1。
*/

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




