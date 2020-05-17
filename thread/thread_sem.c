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
关于原子操作：所谓的原子操作是指如果有两个线程企图同时给信号量加1，它们之间不会相互干扰，而不像两个程序对同一个文件进行
同一个文件读取、增加、写入操作时可能会引起冲突，信号量的值总是会被正确的加2。
*/


/*
函数原型：int sem_init(sem_t *sem, int pshared, unsigned int value);
描述：sem_init用来初始化信号量
参数：
sem：输出型参数，指向信号量变量的指针
pshared：设置信号量是在线程中还是在进程中共享，0表示在线程中共享，非零则表示在进程中共享
value：用来设置信号量的初始值
返回值：
调用成功返回0，失败返回-1

函数原型：int sem_post(sem_t *sem);
描述：以原子操作的方式给信号量的值加1。
参数：
sem：指向信号量变量的指针
返回值：
调用成功返回0，失败返回-1

函数原型：int sem_wait(sem_t *sem);
描述：以原子操作的方式给信号量的值减一，但它会等待直到信号量有个非零值才会进行减法操作
参数：
sem：指向信号量变量的指针
返回值：
调用成功返回0，失败返回-1

函数原型：int sem_destroy(sem_t *sem);
描述：函数用来销毁信号量
参数：
sem：指向信号量变量的指针
返回值：
调用成功返回0，失败返回-1
*/


/*
程序功能描述：使用二值信号量做线程同步,主线程从键盘读取字符串，子线程做统计
*/
#define READ_SIZE 50
sem_t sem;
char sharedbuffer[READ_SIZE] = "\0";

sem_t sem_clamp;

void*thread_func(void*arg) //线程执行函数
{
	sem_wait(&sem); //等待信号量，信号量减一
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		printf("The str:%s", sharedbuffer);
		printf("The len of str:%d\n", (int)strlen(sharedbuffer));	
		sem_wait(&sem);
	}
	
	pthread_exit("thanks for the get the cpu time");//退出线程，这里决不能指向一个局部变量的指针
	
}

int main()    
{
	pthread_t pth_id;
	int ret;
	void *retval;
	ret = pthread_create(&pth_id, NULL, thread_func, NULL);//创建线程
	if (0 != ret)
	{
		perror("pthread_create");
		exit(1);
	}

	ret = sem_init(&sem, 0, 0); //初始化信号量
	if (ret != 0)
	{
		perror("sem_init");
		exit(1);
	}
	
	while(0 != strncmp(sharedbuffer, "end", 3))
	{
		printf("enter string to count(\"end\" to exit):");
		fgets(sharedbuffer, READ_SIZE, stdin);
		sem_post(&sem);	//信号量加一
	}
	
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
	ret = sem_destroy(&sem); //销毁信号量
	if (0 != ret)
	{
		perror("sem_destroy");
		exit(1);
	}
	return 0;
}


