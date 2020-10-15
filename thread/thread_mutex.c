#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>


/*
程序功能描述：使用互斥锁做线程同步，主线程从键盘读取字符串，子线程做统计，使用延时调试线程捕获互斥锁
*/
	
#define READ_SIZE 50
char sharedbuffer[READ_SIZE] = "\0";
pthread_mutex_t mutex;

void*thread_func(void*arg) 
{

	sleep(1);
	pthread_mutex_lock(&mutex);
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		printf("The str:%s", sharedbuffer);
		printf("The len of str:%d\n", (int)strlen(sharedbuffer));			
		pthread_mutex_unlock(&mutex);
		
		usleep(50*1000);   //加延时使得主线程有足够时间加锁
				
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit("thanks for the get the cpu time");
	
}

int main()	  
{
	pthread_t pth_id;
	int ret;
	void *retval;
	
	ret = pthread_mutex_init(&mutex, NULL);
	if (0 != ret)
		{
		perror("pthread_mutex_init");
		exit(1);
	}
	
	ret = pthread_create(&pth_id, NULL, thread_func, NULL);
	if (0 != ret)
	{
		perror("pthread_create");
		exit(1);
	}
	
	pthread_mutex_lock(&mutex);
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		
		printf("enter string to count(\"end\" to exit):");
		
		fgets(sharedbuffer, READ_SIZE, stdin);
		pthread_mutex_unlock(&mutex);
		/*
		测试：适当延时，这样子线程就能够有时间来抢占锁
		测试结果：效果正常，子线程能及时统计，主线程也能及时处理输入
		*/
		usleep(50*1000); 
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	
	printf("waiting for join the thread\n");
	ret = pthread_join(pth_id, &retval); //阻塞等待回收线程
	if (0 == ret)
		{
		printf("the retval of thread is:%s\n", (char *)retval);
	}
	else
		{
		perror("pthread_join");
		exit(1);
	}

	ret = pthread_mutex_destroy(&mutex);
	if (0 != ret)
		{
		perror("pthread_mutex_destroy");
		exit(1);
	}
	
	return 0;
}
