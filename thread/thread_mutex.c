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
