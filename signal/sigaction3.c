#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
程序功能描述：测试sigaction 信号屏蔽集相关系统调用
*/
typedef void (*sighandler_t)(int);

void func(int sig);

int main(void)
{
	int ret = -1;

	struct sigaction act = {0};
	
	sigset_t mask_sigset;		// 用于设置屏蔽信号集
	sigset_t mask_sigset_out;	// 用于保存被阻塞(屏蔽)的信号集

    sigemptyset(&mask_sigset); 	// 清空信号集
    sigemptyset(&mask_sigset_out);

	
    sigaddset(&mask_sigset, SIGINT);      // 向屏蔽信号集中添加 SIGINT
    //sigfillset(&mask_sigset);	     	  //将所有有定义的信号添加到屏蔽信号集中

	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	act.sa_handler = func;

	ret = sigaction(SIGINT, &act, 0);

	if (-1 == ret)
	{
		perror("sigaction");
		exit(1);
	}

	printf("waiting a SIGINT to break pause\n");
	pause();
	
	sigprocmask(SIG_SETMASK, &mask_sigset, 0);   // 设置进程屏蔽字, 在本例中为屏蔽 SIGINT

	printf("enter ctrl + c in 10 seconds\n");
	sleep(10);		  			   //10秒内发送SIGINT信号，因为SIGINT被屏蔽，因此阻塞后处于待处理状态

    sigpending(&mask_sigset_out);    // 拷贝被阻塞的信号中停留在待处理状态的一组信号，测试 SIGINT 是否被屏蔽
    if (sigismember(&mask_sigset_out, SIGINT))
    {
        printf("The SIGINT signal is ignored\n");
    }

	sigdelset(&mask_sigset, SIGINT);  // 从屏蔽信号集中删除信号 SIGINT
	printf("Wait the SIGINT to break the sigsuspend\n");
	
  	ret =  sigsuspend(&mask_sigset);  // 将进程的屏蔽字重新设置, 即取消对 SIGINT 的屏蔽并挂起进程
    if (-1 == ret)
    	{
 		perror("sigsuspend");
		//exit(1);
    }

	while(1)
	{
		printf("the process is running\n");
		sleep(1);
	}

	/*
	测试结果：
	首先，我们能过sigaction()函数改变了SIGINT信号的默认行为，使之执行指定的函数func，
	所以输出了语句：The signal value: 2。
	
	然后，通过sigprocmask()设置进程的信号屏蔽字，把SIGINT信号屏蔽起来，
	所以过了10秒之后，用sigpending()函数去获取被阻塞的信号集时，检测到了被阻塞的信号SIGINT，
	输出The SIGINT signal is ignored。
	
	最后，用函数sigdelset()函数去除先前用sigaddset()函数加在sigset上的信号SIGINT，
	再调用函数sigsuspend()，把进程的屏蔽字再次修改为sigset（不包含SIGINT)，并挂起进程。
	
	由于先前的SIGINT信号停留在待处理状态，而现在进程已经不再阻塞该信号，所以进程马上对该信号进行处理，
	从而在最后，你不用输入 Ctrl+C 也会出现后面的处理语句The SIGINT signal value: 2，最后进入while循环。
	*/
}

void func(int sig)
{
	if (sig == SIGINT)
	{
		printf("The SIGINT signal value: %d\n", sig);
		//exit(0);//模仿SIGINT默认处理，程序退出
	}
}