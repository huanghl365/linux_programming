#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
函数原型：int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
	void *(*start_routine) (void *), void *arg);
描述：pthread_create用来创建一个新线程
参数：
thread：输出型参数，指向线程标识符的地址，我们用该标识符来引用新线程
attr：输入型参数，用来设置线程的属性，一般不需要设置，因此一般设置为NULL
start_routine：这个函数指针指向 线程启动函数，线程从该函数开始执行
arg：输入型参数，作为启动函数的函数参数，如果不需要传递则设置为NULL
返回值：
调用成功则返回0，失败返回错误代码

函数原型：int pthread_join(pthread_t thread, void **retval);
描述：pthread_join用来阻塞等待回收指定的线程,获取线程的终止状态
参数：
thread：线程标识符
retval：输出型参数，指向线程的返回值(pthread_exit的参数)，一般设置为NULL
返回值：
调用成功则返回0，失败返回错误代码	

函数原型：void pthread_exit(void *retval);
描述：pthread_exit用来终止调用它的线程，并返回一个指向某个对象的指针

参数：
retval：输入性参数，用来指向线程的返回值，一般设置为NULL。决不能返回一个指向局部变量的指针，因为线程终止退出后，
这个局部变量就不存在了，这样会引起程序漏洞。
*/

/*
程序功能描述：测试创建一个线程(创建、传参、回收、获取线程返回值)
*/

void*thread_func(void*arg) //线程执行函数
{
	printf("enter the thread_func\n");
	if (NULL != arg)
	{
		printf("the arg is %d\n", *(int*)arg);
	}
	sleep(1);
	printf("leave the thread_func\n");
	//exit(1);  //线程跟进程不一样，如果只是想要退出这一个线程，那么不能使用exit，否者整个程序都会终止掉
	pthread_exit("thanks for the get the cpu time"); //退出线程，这里决不能指向一个局部变量的指针
	
}

int main()    
{
	pthread_t pth_id;
	int ret;
	void *retval;
	int pth_arg = 100; //线程参数
	//ret = pthread_create(&pth_id, NULL, thread_func, NULL);//创建线程
	ret = pthread_create(&pth_id, NULL, thread_func, (void *)&pth_arg);
	if (0 != ret)
	{
		perror("pthread_create");
		exit(1);
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
	
	return 0;
}






/*
程序功能描述：创建多个线程
*/

#if 1
#define PTHREAD_NUM 6

void*thread_func(void*arg) //线程执行函数
{
	int sleep_time;
	//int pth_arg = *(int*)arg;
	int pth_arg = (int)arg; 
	printf("thread is running, the arg = %d\n", pth_arg);
	sleep_time = (int) (1.0 + 9.0*rand()/RAND_MAX); //1-10s
	sleep(sleep_time);
	printf("Bye from %d\n", pth_arg);
	pthread_exit(NULL);
}

int main()    
{
	pthread_t pth_id[PTHREAD_NUM];
	int ret;
	int pth_arg; //线程参数

	for (pth_arg=0; pth_arg<PTHREAD_NUM; pth_arg++)
	{
		
		//ret = pthread_create(&pth_id[pth_arg], NULL, thread_func, (void *)&pth_arg); //传递变量指针
		ret = pthread_create(&pth_id[pth_arg], NULL, thread_func, (void *)pth_arg); //直接传递变量值
		if (0 != ret)
		{
			perror("pthread_create");
			exit(1);
		}
		/*
		屏蔽延时，测试线程创建的时候有没有问题
		测试结果：线程传参不正确。
		原因分析：主线程创建线程太快，
		传递给线程的参数又是指针，这个指针指向的变量的内存空间被主线程快速刷新，
		导致子线程读取参数的时候不对。可以通过修改传参的方式解决,比如直接传递变量值
		*/
		//sleep(1); 
	}

	printf("waiting for join the thread\n");
	for (pth_arg=PTHREAD_NUM-1; pth_arg>=0; pth_arg--) //与创建顺序相反 回收线程
	{
		ret = pthread_join(pth_id[pth_arg], NULL); //阻塞等待回收线程
		
		if (0 != ret)
		{
			perror("pthread_join");
			exit(1);
		}
	}
	printf("finish join the pthread\n");
	
	
	return 0;
}

#endif 

