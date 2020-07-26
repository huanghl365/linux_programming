#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
函数原型：int pthread_detach(pthread_t thread);
描述：设置线程分离
参数：
thread：线程标识符
返回值：
调用成功则返回0，失败返回错误代码

函数原型：pthread_t pthread_self(void);
返回值：
这个函数调用总是成功，返回调用线程的标识符
*/

/*
程序功能描述：通过调用pthread_detach实现线程分离，然后测试创建1M个线程
*/

void*thread_func(void*arg) 
{
	int ret;
	ret = pthread_detach(pthread_self());   //通过pthread_detach设置线程分离
	if (0 != ret)
	{
		perror("pthread_detach");
		pthread_exit("thread detach failed");
	}
	pthread_exit(NULL);
	
}

int main(void)
{
	pthread_t pth_id;
	int ret, try_cnt = 0, success_cnt = 0;
	int i;

	/*
	测试创建1M个线程，连续三次创建失败则结束创建
	测试结果：
	主线程没有使用pthread_join回收线程，因此如果子线程调用了pthread_detach实现线程分离，可以顺利
	创建1M个线程，如果没有调用pthread_detach，因为过多的僵尸线程占用资源没有释放，只能创建32751个线程
	*/
	for (i = 0; i < 1024 * 1024 && try_cnt < 3; i++) 
	{
		ret = pthread_create(&pth_id, NULL, thread_func, NULL);//创建线程
		if (0 != ret)
		{
			try_cnt++;
			printf("creat pthread failded, pthread num = %d\n", i);
		}
		else
		{
			success_cnt++;
			try_cnt = 0;
		}
	}
	printf("success creat thread cnt = [%d]\n", success_cnt);

	return 0;
}

