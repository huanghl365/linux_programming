#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
程序功能描述：测试取消一个线程
*/
#if 1
void*thread_func(void*arg) 
{
	int i,ret;
	int old_state, old_type;
	
	printf("enter the thread_func\n");
	printf("PTHREAD_CANCEL_ENABLE = %d PTHREAD_CANCEL_DISABLE = %d\n",PTHREAD_CANCEL_ENABLE, PTHREAD_CANCEL_DISABLE);
	ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state); 
	if (0 != ret)
	{
		perror("pthread_setcancelstate");
		pthread_exit("thread set cancel state failed\n");
	}
	printf("old_state = %d\n", old_state);

	printf("PTHREAD_CANCEL_ASYNCHRONOUS = %d PTHREAD_CANCEL_DEFERRED = %d\n",PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_DEFERRED);
	ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_type); 
	if (0 != ret)
	{
		perror("pthread_setcanceltype");
		pthread_exit("thread set cancel type failed\n");
	}
	printf("old_type = %d\n", old_type);
	
	for (i=0; i<1024; i++)	
	{
		sleep(1);
		printf("thread_func sleep time: %ds\n", i+1);
	}
	printf("leave the thread_func\n");
	pthread_exit(NULL);
	
	
}

int main()	  
{
	pthread_t pth_id;
	int ret;
	
	ret = pthread_create(&pth_id, NULL, thread_func, NULL);//创建线程
	if (0 != ret)
	{
		perror("pthread_create");
		exit(1);
	}
	
	sleep(5);	//不要立即取消线程，延时一下让线程先运行一段时间
	ret = pthread_cancel(pth_id);
	if (0 != ret)
	{
		perror("pthread_cancel");
		exit(1);
	}

	printf("finish send the cancel request\n");

	/*
	测试：分别设置线程的取消类型为PTHREAD_CANCEL_DEFERRED 和 PTHREAD_CANCEL_ASYNCHRONOUS，执行pthread_cancel之后，
	延时10s看线程是不是立即取消，还是需要等待pthread_join执行，如果需要等待pthread_join执行的话，那么子线程
	会继续打印10s
	
	测试结果：sleep调用可以成为取消点，因此无法延时来验证PTHREAD_CANCEL_DEFERRED 和 PTHREAD_CANCEL_ASYNCHRONOUS的区别
	*/
	sleep(10);  
	printf("waiting for join the thread\n");
	
	ret = pthread_join(pth_id, NULL); //阻塞等待回收线程
	if (0 != ret)
	{
		perror("pthread_join");
		exit(1);
	}
	
	
	return 0;
}
#endif

