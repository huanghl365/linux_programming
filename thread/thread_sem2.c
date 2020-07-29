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
程序功能描述：使用二值信号量做线程同步,主线程从键盘读取字符串，子线程做统计；
本例使用两个信号量来做线程同步，解决上个例子 主线程不会等子线程处理完就继续处理 用户输入的问题
*/

//#define SEM_CLAMP
#define READ_SIZE 50
sem_t sem;
char sharedbuffer[READ_SIZE] = "\0";

sem_t sem_clamp;

void*thread_func(void*arg) 
{
#ifdef SEM_CLAMP
	sem_post(&sem_clamp);
#endif 

	sem_wait(&sem); 
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		printf("The str:%s", sharedbuffer);
		printf("The len of str:%d\n", (int)strlen(sharedbuffer));
#ifdef SEM_CLAMP
		sem_post(&sem_clamp);
#endif		
		sem_wait(&sem);
	}
	
	pthread_exit("thanks for the get the cpu time");
	
}

int main()    
{
	pthread_t pth_id;
	int ret;
	void *retval;
	ret = pthread_create(&pth_id, NULL, thread_func, NULL);
	if (0 != ret)
	{
		perror("pthread_create");
		exit(1);
	}

	ret = sem_init(&sem, 0, 0); 
	if (ret != 0)
	{
		perror("sem_init");
		exit(1);
	}

#ifdef SEM_CLAMP
	ret = sem_init(&sem_clamp, 0, 0); 
	if (ret != 0)
	{
		perror("sem_init");
		exit(1);
	}
#endif 

	
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
#ifdef SEM_CLAMP
		sem_wait(&sem_clamp);
#endif 
		printf("enter string to count(\"end\" to exit):");
		fgets(sharedbuffer, READ_SIZE, stdin);
		sem_post(&sem);	//信号量加一

		/*
		测试：这里对标准输入流的字符串再做一个判断，如果符合条件则拷贝HELLOWORLD到sharedbuffer中并增加信号量
		测试结果：按照设想，线程应该分别统计一次FAST和HELLOWORLD的长度，但是测试发现线程统计了
		两次HELLOWORLD长度的长度，这是因为我们快速地刷新了两次sharedbuffer，但是线程来不及处理，信号量又增加了两次，
		因此才会出现这种结果。

		解决方法有两种：
			1、打开宏定义SEM_CLAMP，再增加一个信号量，完美解决问题。
			2、不使用信号量，换成互斥锁（但是使用互斥锁需要控制好延时，使线程能及时抢占锁，实现相对复杂）
		*/
#if 1
		if (0 == strncmp(sharedbuffer, "FAST", 4))
		{
#ifdef SEM_CLAMP
			sem_wait(&sem_clamp);
#endif				
			strcpy(sharedbuffer, "HELLOWORLD");
			sem_post(&sem);	
		}
#endif 

	}
	
	printf("waiting for join the thread\n");
	ret = pthread_join(pth_id, &retval); 
	if (0 == ret)
		{
		printf("the retval of thread is:%s\n", (char *)retval);
	}
	else
		{
		perror("pthread_join");
		exit(1);
	}
	ret = sem_destroy(&sem); 
	if (0 != ret)
	{
		perror("sem_destroy");
		exit(1);
	}
#ifdef SEM_CLAMP
	ret = sem_destroy(&sem_clamp); 
	if (0 != ret)
	{
		perror("sem_destroy");
		exit(1);
	}
#endif
	return 0;
}

