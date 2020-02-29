#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

/*
函数原型：pid_t waitpid(pid_t pid, int *status, int options);
函数描述：返回已经终止的子进程的进程ID号，并清除僵尸进程。
参数：
pid：指定需要回收的进程的pid；取值为-1表示不等待某个特定PID的子进程而是回收任意一个子进程
status：作为输出型参数，用来保存进程的退出状态，用来给一些宏传参做判断
options：这个选项可以用来指定是否阻塞回收，0表示阻塞回收，WNOHANG表示不阻塞
返回值：成功回收则返回回收进程的pid；如果使用了WNOHANG并且要回收的进程还没结束，则直接返回0；
回收失败则返回-1
*/

/*
程序功能描述：测试使用waitpid回收子进程
*/
int main(int argc, char *argv[])
{
	pid_t pid = -1;
	pid_t ret = -1;
	int status;
	pid = fork();
	
	if(pid == 0)
	{
		printf("child:子进程ID：%d\n", getpid());
		//while(1);
		exit(256);
	}
	
	if(pid > 0)
	{
#if 0
		ret = waitpid(pid, &status, 0);        //使用waitpid回收指定pid，WNOHANG表示非阻塞回收
		//ret = waitpid(pid+4, &status, 0);    //回收指定pid(不存在)
		//ret = waitpid(-1, &status, 0);       //回收任意pid
#endif
		
#if 1
		sleep(1);								 	//waitpid使用非阻塞时，父进程必须开启sleep才能回收成功
		//ret = waitpid(pid, &status, WNOHANG);  		//非阻塞回收指定pid
		//ret = waitpid(pid+4, &status, WNOHANG);	//非阻塞回收指定pid(不存在)
		ret = waitpid(-1, &status, WNOHANG);     	//非阻塞回收任意pid
#endif
		if (-1 != ret)
		{
			printf("父进程成功回收子进程：%d\n", ret);
			printf("status = %d\n", status);

			/*
			WIFEXITED(status)：进程正常终止，则取非零值
			WIFSIGNALED(status)：被未捕获的信号终止，则取非零值
			WEXITSTATUS(status)：取子进程传给exit或_exit的低8位。一般会先用 WIFEXITED 来判断是否正常结束才能使用此宏
			说明：先确认进程正常回收，否则判断没有什么意义
			*/
			printf("parent:子进程是否正常终止：%d\n", WIFEXITED(status));    
			printf("parent:子进程是否非正常终止：%d\n", WIFSIGNALED(status));   
			printf("parent:子进程终止退出码：%d\n", WEXITSTATUS(status));
		}
		else
		{
			perror("waitpid");
			//printf("status = %d\n", status);
			exit(1);
		}

		
	}
	
	if(pid < 0)
	{
		printf("复制进程失败\n");
		return -1;
	}
	return 50;
}

