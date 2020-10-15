#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


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


