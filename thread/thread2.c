#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
程序功能描述：通过设置线程属性实现线程分离，然后测试创建1M个线程
*/

void*thread_func(void*arg) //线程执行函数
{

	pthread_exit(NULL);
	
}

int main(void)
{
	pthread_t pth_id;
	int ret, try_cnt = 0, success_cnt = 0;
	pthread_attr_t pthread_attr;
	int i;
	
	ret = pthread_attr_init(&pthread_attr); //初始化默认线程属性
	if (0 != ret)
	{
		perror("pthread_attr_init");
		exit(1);
	}
	
	//ret = pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED); //设置分离线程
	ret = pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_JOINABLE); //设置回收线程
	
	if (0 != ret)
	{
		perror("pthread_attr_setdetachstate");
		exit(1);
	}
	
	/*
	测试：分别PTHREAD_CREATE_DETACHED和PTHREAD_CREATE_JOINABLE属性创建1M个线程，连续三次创建失败则结束创建
	测试结果：
	PTHREAD_CREATE_DETACHED：因为设置了线程资源，无需使用pthread_join，系统自动回收线程，因此成功创建1M个线程
	PTHREAD_CREATE_JOINABLE：因为没有使用pthread_join回收线程，导致过多的僵尸线程占用资源没有释放，因此只能创建32751个线程
	*/
	for (i = 0; i < 1024 * 1024 && try_cnt < 3; i++) 
	{
		ret = pthread_create(&pth_id, &pthread_attr, thread_func, NULL);//创建线程
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
	
	ret = pthread_attr_destroy(&pthread_attr); //清理回收线程属性
	if (0 != ret)
	{
		perror("pthread_attr_destroy");
		exit(1);
	}
	return 0;
}

