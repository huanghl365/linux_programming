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

void*thread_func(void*arg) 
{
	char ret_str[] = "thanks for the get the cpu time";
	printf("enter the thread_func\n");
	if (NULL != arg)
	{
		printf("the arg is %d\n", *(int*)arg);
	}
	sleep(1);
	printf("leave the thread_func\n");


	/*
	测试：使用exit退出线程，不使用pthread_exit
	测试结果：不仅仅是线程，整个程序都终止掉；
	线程跟进程不一样，如果只是想要退出这一个线程，那么不能使用exit，应该使用pthread_exit。
	*/
	//exit(1);  


	/*
	测试：pthread_exit返回参数 分别使用局部变量和字符串常量
	测试结果：
	使用局部变量时，pthread_join无法收到pthread_exit的返回参数，打印为空，使用字符串常量则可以收到。
	这时因为线程终止退出后，这个局部变量就不存在导致的，因此返回参数不能使用局部变量。
	*/
	//pthread_exit(ret_str);
	pthread_exit("thanks for the get the cpu time"); 
	
}

int main()    
{
	pthread_t pth_id;
	int ret;
	void *retval;
	int pth_arg = 100; 
	//ret = pthread_create(&pth_id, NULL, thread_func, NULL);
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


