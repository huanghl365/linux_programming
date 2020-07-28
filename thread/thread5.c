#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*
程序功能描述：快速创建多个线程，并给每个线程传参，并分析传参出错的问题
*/

#if 1
#define PTHREAD_NUM 6

void*thread_func(void*arg) 
{
	int sleep_time;
	//int pth_arg = *(int*)arg;  //参数是变量地址
	int pth_arg = (int)arg;  //参数是整形变量
	
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
	int pth_arg; 

	for (pth_arg=0; pth_arg<PTHREAD_NUM; pth_arg++)
	{

/*
测试：不加1s的延时，快速创建多个线程，并给每个线程传递变量地址
测试结果：创建的6个线程打印传参，发现都是一样，传参错误。
原因分析：主线程创建线程太快，传递给线程的参数又是指针，这个指针指向的变量的内存空间被主线程快速刷新，
导致线程读取参数不对。
当添加1s的延时之后，传参就正确了。
*/

#if 1
		ret = pthread_create(&pth_id[pth_arg], NULL, thread_func, (void *)&pth_arg); //传递变量指针
		if (0 != ret)
		{
			perror("pthread_create");
			exit(1);
		}
		//sleep(1);
#endif


/*
测试：不加1s的延时，快速创建多个线程，并给每个线程直接传递变量值
测试结果：传参正确。
*/
#if 0
		ret = pthread_create(&pth_id[pth_arg], NULL, thread_func, (void *)pth_arg); //直接传递变量值
		if (0 != ret)
		{
			perror("pthread_create");
			exit(1);
		}
#endif 
	}

	printf("waiting for join the thread\n");
	for (pth_arg=PTHREAD_NUM-1; pth_arg>=0; pth_arg--) //与创建顺序相反 回收线程
	{
		ret = pthread_join(pth_id[pth_arg], NULL); 
		
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

