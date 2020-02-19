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

#if 0
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
#endif


/*
关于未分离线程：
在我们使用默认属性创建一个线程的时候，线程是 joinable的。joinable状态的线程，
必须在另一个线程中使用pthread_join() 等待其结束，如果一个 joinable 的线程在结束后，
没有使用 pthread_join() 进行操作，这个线程就会变成"僵尸线程"。每个僵尸线程都会消耗
一些系统资源，当有太多的僵尸线程的时候，可能会导致创建线程失败。

关于分离线程：
当线程被设置为分离状态后，线程结束时，它的资源会被系统自动的回收，
而不再需要在其它线程中对其进行 pthread_join() 操作。
*/

/*
函数原型：int pthread_attr_init(pthread_attr_t *attr);
描述：初始化默认线程属性，获取默认线程属性
参数：
attr：输出型参数，指向线程属性的指针
返回值：
调用成功则返回0，失败返回错误代码


函数原型：int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
描述：设置线程默认分离状态，使用pthread_attr_init初始化后的线程默认是PTHREAD_CREATE_JOINABLE的
参数：
attr：输出型参数，指向线程属性的指针
detachstate：设置线程是否分离
	PTHREAD_CREATE_DETACHED	//分离线程
	PTHREAD_CREATE_JOINABLE //回收线程
返回值：
调用成功则返回0，失败返回错误代码


函数原型：int pthread_attr_destroy(pthread_attr_t *attr);
描述：清理回收线程属性
参数：
attr：指向线程属性的指针
返回值：
调用成功则返回0，失败返回错误代码
*/

/*
程序功能描述：通过设置线程属性实现线程分离
*/

#if 0
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
	/*
	设置线程分离，可以通过pthread_attr_setdetachstate也可以通过pthread_detach
	*/
	//ret = pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED); //设置分离线程
	ret = pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_JOINABLE); //设置回收线程
	
	if (0 != ret)
	{
		perror("pthread_attr_setdetachstate");
		exit(1);
	}
	
	/*
	以PTHREAD_CREATE_DETACHED和PTHREAD_CREATE_JOINABLE属性 测试创建1M个线程，连续三次创建失败则结束创建
	测试结果：
	PTHREAD_CREATE_DETACHED：因为设置了线程资源，无需使用pthread_join，系统自动回收线程，因此成功创建1M个线程
	PTHREAD_CREATE_JOINABLE：因为没有使用pthread_join回收线程，导致过多的僵尸线程占用资源没有释放，
	因此只能创建32751个线程
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
#endif 

/*
函数原型：int pthread_detach(pthread_t thread);
描述：设置线程分离
参数：
thread：线程标识符
返回值：
调用成功则返回0，失败返回错误代码

函数原型：pthread_t pthread_self(void);
返回值：
这个函数调用总是成功，返回调用线程的标识符
*/


/*
程序功能描述：通过调用pthread_detach实现线程分离
*/

#if 0
void*thread_func(void*arg) //线程执行函数
{
#if 1 //通过pthread_detach设置线程分离
	int ret;
	ret = pthread_detach(pthread_self()); 
	if (0 != ret)
	{
		perror("pthread_detach");
		pthread_exit("thread detach failed");
	}
#endif 
	pthread_exit(NULL);
	
}

int main(void)
{
	pthread_t pth_id;
	int ret, try_cnt = 0, success_cnt = 0;
	int i;

	/*
	测试创建1M个线程，连续三次创建失败则结束创建
	测试结果：主线程没有使用pthread_join回收线程，因此如果子线程调用了pthread_detach实现线程分离，可以顺利
	创建1M个线程，如果没有调用pthread_detach，因为过多的僵尸线程占用资源没有释放，只能创建32751个线程
	*/
	for (i = 0; i < 1024 * 1024 && try_cnt < 3; i++) 
	{
		ret = pthread_create(&pth_id, NULL, thread_func, NULL);//创建线程
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

	return 0;
}
#endif 


/*
函数原型：int pthread_cancel(pthread_t thread);
描述：给指定线程发送取消请求，终止线程
参数：
thread：指定线程标识符
返回值：
调用成功则返回0，失败返回错误代码

函数原型：int pthread_setcancelstate(int state, int *oldstate);
描述：用来设置当前线程的取消状态，线程默认取消状态为PTHREAD_CANCEL_ENABLE
参数：
state：设置线程的取消状态
	PTHREAD_CANCEL_ENABLE	表示允许接收取消请求
	PTHREAD_CANCEL_DISABLE  表示忽略接收取消请求
oldstate：输出型参数，保存原先的取消状态

返回值：
调用成功则返回0，失败返回错误代码

函数原型：int pthread_setcanceltype(int type, int *oldtype);
描述：用来设置线程的取消类型，调用前必须先调用pthread_setcancelstate设置好取消状态，线程默认取消类型为PTHREAD_CANCEL_DEFERRED
参数：
type：设置线程的取消类型
	PTHREAD_CANCEL_ASYNCHRONOUS  表示接收到取消请求后立即采取行动终止线程
	PTHREAD_CANCEL_DEFERRED		 表示接收到取消请求后，需要一直等待知道线程执行了以下函数
	才采取行动(pthread_join\pthread_cont_wait\pthread_cont_timedwait\pthread_testcancel
	\semwait\sigwait),其他阻塞函数也可能成为取消点，比如read、wait
oldtype：输出型参数，保存原先的取消类型
返回值：
调用成功则返回0，失败返回错误代码
*/

/*
程序功能描述：测试取消一个线程
*/
#if 0
void*thread_func(void*arg) //线程执行函数
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
	pthread_exit(NULL);//退出线程，这里决不能指向一个局部变量的指针
	
	
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
	}

	printf("finish send the cancel request\n");

	/*
	分别设置线程的取消类型为PTHREAD_CANCEL_DEFERRED 和 PTHREAD_CANCEL_ASYNCHRONOUS，
	延时一段时间测试线程是不是立即取消，还是需要等待pthread_join执行，如果需要等待pthread_join的话那么子线程
	会继续打印消息
	
	测试结果：书中说sleep调用可以成为取消点，因此无法延时来验证PTHREAD_CANCEL_DEFERRED 和 PTHREAD_CANCEL_ASYNCHRONOUS的区别
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

