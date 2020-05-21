#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
1、int sigemptyset(sigset_t *set);
该函数的作用是将信号集初始化为空。

2、int sigfillset(sigset_t *set);
该函数的作用是把信号集初始化包含所有已定义的信号。

3、int sigaddset(sigset_t *set, int signo);
该函数的作用是把信号signo添加到信号集set中，成功时返回0，失败时返回-1。

4、int sigdelset(sigset_t *set, int signo);
该函数的作用是把信号signo从信号集set中删除，成功时返回0，失败时返回-1.

5、int sigismember(sigset_t *set, int signo);
该函数的作用是判断给定的信号signo是否是信号集中的一个成员，如果是返回1，如果不是，返回0，如果给定的信号无效，返回-1；

6、int sigpromask(int how, const sigset_t *set, sigset_t *oset);
该函数可以根据参数指定的方法修改进程的信号屏蔽字。新的信号屏蔽字由参数set（非空）指定，
而原先的信号屏蔽字将保存在oset（非空）中。如果set为空，则how没有意义，但此时调用
该函数，如果oset不为空，则把当前信号屏蔽字保存到oset中。
如果sigpromask成功完成返回0，如果how取值无效返回-1，并设置errno为EINVAL。
注意：调用这个函数才能改变进程的屏蔽字，之前的函数都是为改变一个变量的值而已，并不会真正影响进程的屏蔽字。

how的不同取值及操作如下所示：
SIG_BLOCK	将参数set中的信号添加到信号屏蔽字中
SIG_SETMASK	把信号屏蔽字设置为set中的信号
SIG_UNBLOCK 从信号屏蔽字中删除参数set中的信号

7、int sigpending(sigset_t *set);
该函数的作用是将被阻塞的信号中停留在待处理状态的一组信号写到参数set指向的信号集中，成功调用返回0，否则返回-1，并设置errno表明错误原因。

8、int sigsuspend(const sigset_t *sigmask);
该函数通过将进程的屏蔽字替换为由参数sigmask给出的信号集，然后挂起进程的执行。注意操作的先后顺序，是先替换再挂起程序的执行。
程序将在信号处理函数执行完毕后继续执行。如果接收到信号终止了程序，sigsuspend()就不会返回，如果
接收到的信号没有终止程序，sigsuspend()就返回-1，并将errno设置为EINTR。

特别提醒：如果一个信号被进程阻塞，它就不会传递给进程，但会停留在待处理状态，当进程解除对待处理信号的阻塞时，待处理信号就会立刻被处理。
*/


/*
程序功能描述：测试信号屏蔽集相关系统调用的使用
*/
typedef void (*sighandler_t)(int);

void func(int sig);

int main(void)
{
	int ret = -1;

	struct sigaction act = {0};
	
	sigset_t sigset;	// 用于记录屏蔽字
	sigset_t ign;		// 用于记录被阻塞(屏蔽)的信号集

    sigemptyset(&sigset); // 清空信号集
    sigemptyset(&ign);

	
    sigaddset(&sigset, SIGINT);  // 向信号集中添加 SIGINT
    //sigfillset(&sigset);	//将所有有定义的信号添加到信号集中

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
	
	sigprocmask(SIG_SETMASK, &sigset, 0);   // 设置进程屏蔽字, 在本例中为屏蔽 SIGINT

	printf("enter ctrl + c in 10 seconds\n");
	sleep(10);		   //10秒内发送SIGINT信号，因为SIGINT被屏蔽，因此阻塞后处于待处理状态

    sigpending(&ign);  // 拷贝被阻塞的信号中停留在待处理状态的一组信号，测试 SIGINT 是否被屏蔽
    if (sigismember(&ign, SIGINT))
    {
        printf("The SIGINT signal is ignored\n");
    }

	sigdelset(&sigset, SIGINT);  // 从信号集中删除信号 SIGINT
	printf("Wait the SIGINT to break the sigsuspend\n");
	
  	ret =  sigsuspend(&sigset);  // 将进程的屏蔽字重新设置, 即取消对 SIGINT 的屏蔽并挂起进程
    if (-1 == ret)
    	{
 		perror("sigsuspend");
		exit(1);
    }

	while(1)
	{
		printf("the process is running\n");
		sleep(1);
	}

	/*
	测试结果：
	首先，我们能过sigaction()函数改变了SIGINT信号的默认行为，使之执行指定的函数func，
	所以输出了语句：The signal value: 2。然后，通过sigprocmask()设置进程的信号屏蔽字，把SIGINT信号屏蔽起来，
	所以过了10秒之后，用sigpending()函数去获取被阻塞的信号集时，检测到了被阻塞的信号SIGINT，
	输出The SIGINT signal is ignored。最后，用函数sigdelset()函数去除先前用sigaddset()函数加在sigset上的信号SIGINT，
	再调用函数sigsuspend()，把进程的屏蔽字再次修改为sigset（不包含SIGINT)，并挂起进程。
	由于先前的SIGINT信号停留在待处理状态，而现在进程已经不再阻塞该信号，所以进程马上对该信号进行处理，
	从而在最后，你不用输入 Ctrl+C 也会出现后面的处理语句Wait the SIGINT to break the sigsuspend，最后进入while循环。
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