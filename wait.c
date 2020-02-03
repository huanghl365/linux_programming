#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
程序功能描述：测试使用wait回收子进程，回收子进程本身所占用的内存空间
*/
int main(int argc, char *argv[])
{
	pid_t pid = -1;
	pid_t ret = -1;
	int status;
	pid = fork();
	
	if(pid == 0)
	{
		sleep(2);
		printf("子进程的ID：%d\n", getpid());
		//while(1);
	}
	if(pid > 0)
	{
		/*wait 调用用来阻塞等待回收进程，参数作为输出型参数，用来保存进程的回收状态，用来给一些宏传参做判断，
		成功回收则返回回收进程的pid，回收失败则返回-1 */
		
		ret = wait(&status);

		if (-1 != ret)
		{
			printf("父进程成功回收子进程：%d\n", ret);

			/*
			WIFEXITED(status)：进程正常终止，则取非零值
			WIFSIGNALED(status)：被未捕获的信号终止，则取非零值
			WEXITSTATUS(status)：取子进程传给exit或_exit的低8位。一般会先用 WIFEXITED 来判断是否正常结束才能使用此宏
			注意：下面的这些宏应该是在进程正常回收时使用的，如果需要回收的进程不存在，那么用这些宏是没有意义的，因此
			在使用这些宏判断之前先确保进程已经成功回收
			*/
			printf("子进程是否正常终止：%d\n", WIFEXITED(status));      
			printf("子进程是否非正常终止：%d\n", WIFSIGNALED(status));  
			printf("子进程退出状态：%d\n", WEXITSTATUS(status));    
		}
		else
		{
			perror("wait");
		}
		  
	}
	if(pid < 0)
	{
		printf("复制进程失败\n");
		return -1;
	}
	return 50;
}

