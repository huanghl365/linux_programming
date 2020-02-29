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
函数原型：int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
描述：初始化互斥锁，初始化成功的互斥锁变量默认是解锁的
参数：
mutex：输出型参数，指向互斥锁变量的指针
attr：输入型参数，用来设置互斥锁变量的属性，一般设定为NULL表示使用默认的参数
返回值：
调用成功返回0

函数原型：int pthread_mutex_lock(pthread_mutex_t *mutex);
描述：用来将互斥锁加锁，如果互斥锁已经在其他线程调用pthread_mutex_lock函数加锁，则这里就
		会阻塞无法继续执行直到互斥锁已经处于解锁状态这里才能够加锁并继续执行程序
参数：
mutex：指向互斥锁变量的指针
返回值：
调用成功返回0


函数原型：int pthread_mutex_unlock(pthread_mutex_t *mutex);
描述：用来将互斥锁解锁，这样需要互斥锁的线程就才能够利用互斥锁进行加锁
参数：
mutex：指向互斥锁变量的指针
返回值：
调用成功返回0


函数原型：int pthread_mutex_destroy(pthread_mutex_t *mutex);
描述：用来销毁互斥锁，销毁前互斥锁必须先解锁，销毁后的互斥锁可以重新使用pthread_mutex_init初始化
参数：
mutex：指向互斥锁变量的指针
返回值：
调用成功返回0
*/


/*
程序功能描述：使用信号量做线程同步，主线程从键盘读取字符串，子线程做统计，使用延时调试线程捕获互斥锁
*/
	
#if 0
#define READ_SIZE 50
char sharedbuffer[READ_SIZE] = "\0";
pthread_mutex_t mutex;

void*thread_func(void*arg) //线程执行函数
{

	sleep(1);
	pthread_mutex_lock(&mutex);
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		printf("The str:%s", sharedbuffer);
		printf("The len of str:%d\n", (int)strlen(sharedbuffer));			
		pthread_mutex_unlock(&mutex);
		
		sleep(1);   //这里加延时使得主线程有足够时间加锁
		
		pthread_mutex_lock(&mutex);
	}
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
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		
		printf("enter string to count(\"end\" to exit):");
		
		fgets(sharedbuffer, READ_SIZE, stdin);
		pthread_mutex_unlock(&mutex);

		/*
		这里加延时使得子线程有足够时间加锁
		测试结果：延时1秒子线程有可能会统计两次，2s则子线程一定会统计两次。这说明在使用互斥锁做线程同步的
		时候，直接使用延时调试线程捕获互斥锁是不准确的，效率也比不上用两个信号量(无需多余的延时等待)。
		*/
		sleep(1);
		//sleep(2);
		
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
#endif
	
	


/*
程序功能描述：使用信号量做线程同步，主线程从键盘读取字符串，子线程做统计，使用变量判断的方式调试线程捕获互斥锁
*/
#if 1
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
#endif
