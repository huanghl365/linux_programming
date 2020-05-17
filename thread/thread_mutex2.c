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
程序功能描述：使用互斥锁做线程同步，主线程从键盘读取字符串，子线程做统计，使用变量判断的方式调试线程捕获互斥锁
*/
#define READ_SIZE 50
char sharedbuffer[READ_SIZE] = "\0";
pthread_mutex_t mutex;
int time_to_exit;

void*thread_func(void*arg) //线程执行函数
{

	sleep(1);
	pthread_mutex_lock(&mutex);
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		printf("The str:%s", sharedbuffer);
		printf("The len of str:%d\n", (int)strlen(sharedbuffer));
		sharedbuffer[0] = '\0';
		
		pthread_mutex_unlock(&mutex);
				
		while(1)
		{
			pthread_mutex_lock(&mutex);
			if ('\0' == sharedbuffer[0])
			{
				pthread_mutex_unlock(&mutex);
				//sleep(1);
			}
			else
			{
				pthread_mutex_unlock(&mutex);
				break;
			}
		}
		pthread_mutex_lock(&mutex);
	}
	sharedbuffer[0] = '\0';	 //这里要注意清零，否者主线程会跳不出while循环
	time_to_exit = 1;			
	pthread_mutex_unlock(&mutex);
	pthread_exit("thanks for the get the cpu time");//退出线程，这里决不能指向一个局部变量的指针
	
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
	
	ret = pthread_create(&pth_id, NULL, thread_func, NULL);//创建线程
	if (0 != ret)
	{
		perror("pthread_create");
		exit(1);
	}

	pthread_mutex_lock(&mutex);

	while(!time_to_exit )
	{
		if (0 != strncmp(sharedbuffer, "end", 3))
		{
			printf("enter string to count(\"end\" to exit):");
			
			fgets(sharedbuffer, READ_SIZE, stdin);
			pthread_mutex_unlock(&mutex);

			/*
			采取变量判断的方式调试线程捕获信号量
			测试结果：效果正常，不会出现延时调试线程捕获互斥锁是不准确的问题。
			其他：采用这种轮询的方式其实并不是好的编程方式，比不上用两个信号量来做同步
			*/
			while(1)
			{
				pthread_mutex_lock(&mutex);
				if ('\0' != sharedbuffer[0])
				{
					pthread_mutex_unlock(&mutex);   //在延时之前先释放锁，使子线程能加锁，提高效率
					//因为采用的是轮询判断，因此这里可以适当延时，避免浪费cpu时间资源，虽然加了延时处理效率就比不上用两个信号量做同步的方式
					//sleep(1);    
				}
				else
				{
					pthread_mutex_unlock(&mutex); //这里一定要释放锁，不要漏掉，否者会导致出现死锁的情况
					break;
				}
			}
			
			pthread_mutex_lock(&mutex);
		}
		
		
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
	//pthread_mutex_lock(&mutex);


	ret = pthread_mutex_destroy(&mutex);
	if (0 != ret)
	{
		perror("pthread_mutex_destroy");
		exit(1);
	}
	
	return 0;
}

