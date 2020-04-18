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
#if 0
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
	if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT to over process\n");
		//exit(0);//模仿SIGINT默认处理，程序退出8

		/*
		这里加个延时测试当信号处理函数执行过程中，如果再次接受到多个SIGINT信号时是怎么处理的
		测试结果：在执行信号处理函数这段期间，多次操作ctrl+c，程序也不会调用信号处理函数，而是在本次
		信号处理函数完成之后，再执行一次信号处理函数（无论前面产生了多少次ctrl+c信号）
		
		原因如下：程序在执行信号处理函数过程中，发送的ctrl+c信号将会被阻塞，直到信号处理函数执行完成，
		才有机会处理信号函数执行期间产生的ctrl+c,但是在信号函数执行产生的多次ctrl+c，最后只会产生ctrl+c。

		程序后台多次运行，发送一个SIGHUP，在信号函数执行过程中，发送多个SIGHUP，然后执行fg命令变为前台进程，发送多个SIGINT
		测试结果：每种信号的处理逻辑同上面的测试，处理过程是SIGHUP - SIGINT - SIGINT - SIGHUP， 可以看出栈式处理
		*/
		//sleep(10); 
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
		//sleep(10); 
	}

	if (sig == SIGTERM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGTERM to over process\n");
		//exit(0); 
	}
}
#endif //kill -HUP 79396

/*
程序功能描述：使用alarm发送  SIGALRM信号并捕捉处理
*/

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
#if 0
void func(int sig)
{
	
	if (sig == SIGALRM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGALRM signal\n");
	}
	/*
	这里加个延时，测试 pause是否需要等待信号处理函数执行完毕才返回
	测试结果：测试发现是需要的
	*/
	//sleep(2); 
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
#endif


/*
程序功能描述：测试子进程kill调用发送信号，父进程捕捉并处理
*/
/*
函数原型：int kill(pid_t pid, int sig);
描述：kill可以用来包括他本身在内的 其他进程发送一个进程；
发送的进程必须拥有相应的权限，这意味两个进程必须拥有相同的用户ID或者发送信号的进程是超级用户

参数：
pid：指定要发送信号的进程PID
sig：指定要发送的信号。如果sig为0，则不发送信号，但仍执行错误检查，
这可用于检查是否存在pid对应的进程或进程组。

返回值：
调用成功返回0，失败返回-1，失败的原因可能有以下几种：
	EINVAL 给定的信号无效
	EPERM 进程权限不够
	ESRCH 目标进程不存在
*/

#if 1
void func(int sig)
{
	if (sig == SIGALRM)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGALRM signal\n");
	}
	else if (sig == SIGINT)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGINT signal\n");
		//exit(0);//模仿SIGINT默认处理，程序退出
	}
	else if (sig == SIGKILL)
	{
		printf("The signal value: %d\n", sig);
		printf("You send a SIGKILL signal\n");
		//exit(0); //模仿SIGKILL默认处理，程序退出
	}
}

int main(void)
{
	pid_t pid;
	sighandler_t ret = (sighandler_t)-2;
	int ret2;
	int status;
	pid = fork();
	if(pid == -1)
	{
		printf("进程复制失败\n");
		exit(1);
	}
	
#if 1   
	if (0 == pid) //测试使用kill 发送SIGALRM
	{
		sleep(2);

		ret2 = kill(getppid(), 0); //kill 的sig等于0，不发送信号，可以检查进程是否存在
		if (0 == ret2)
		{
			printf("the process %d is exist\n", getppid());
		}
		else
		{
			perror("kill");
			exit(1);
		}
		
		sleep(2);
		ret2 = kill(getppid(), SIGALRM); //给父进程发送SIGALRM信号
		if (-1 == ret2)
		{
			perror("kill");
			exit(1);
		}
		exit(0);
	}


	if (pid > 0)
	{
		ret = signal(SIGALRM, func);   
		//ret = signal(SIGALRM, SIG_DFL); 
		//ret = signal(SIGALRM, SIG_IGN);   //如果忽略信号的话那么父进程会一直卡死在pause那里
		
		if (ret == SIG_ERR)
		{
			perror("signal");
			exit(1);
		}
		
		pause();  


		pid = waitpid(pid, &status, 0);  //阻塞等待回收子进程
		printf("parent:父进程回收的子进程ID：%d\n", pid);
		if (-1 != pid) 
		{
			printf("parent:子进程是否正常终止：%d\n", WIFEXITED(status));    
			printf("parent:子进程是否非正常终止：%d\n", WIFSIGNALED(status));   
			printf("parent:子进程终止退出码：%d\n", WEXITSTATUS(status)); 
		}
		else
		{
			perror("waitpid");
			exit(1);
		}
	}
#endif 

#if 0 //测试使用kill 发送SIGKILL
	if (0 == pid)
	{
		
		sleep(5);
		ret2 = kill(getppid(), SIGKILL); //给父进程发送SIGKILL信号
		if (-1 == ret2)
		{
			perror("kill");
			exit(1);
		}

		/*
		这里加个while，测试发送SIGKILL给父进程终止后子进程是否终止
		测试结果：子进程不会终止
		*/
		while(1) 
		{
			printf("子进程: %d\n", getpid());
			sleep(1);
		}
	}

	if (pid > 0)
	{
		ret = signal(SIGKILL, func);   //SIGKILL信号是无法捕获的
		if (ret == SIG_ERR)
		{
			perror("signal");
			exit(1);
		}
		/*
		这里加个while，看接收到SIGKILL后父进程是否终止
		测试结果：父进程会终止
		*/
		while(1)	
		{
			printf("父进程: %d\n", getpid());
			sleep(1);
		}
		
	}
#endif 

	return 50;
}
#endif


/*
程序功能描述：使用signal调用和alarm调用封装自己的sleep函数
*/

#if 0
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
#endif 

