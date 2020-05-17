#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
/*
程序功能描述：测试一种实现进程只能单次运行的方法
*/

#if 1
#define FILENAME "./single_run.lock"
void delete_file(void);

int main(void)
{
	int ret = -1;

	/*
	O_CREAT  创建文件，文件存在则不创建，但不报错
	O_EXCL   如果文件存在则报错
	因此这里创建文件满足两点要求：
	1、文件存在的话报错，因此只能创建一次   
	2、创建文件的过程是原子操作，及这个文件不可能被其他进程在同一时间创建
	所以我们可以利用这个特性创建一个锁文件，实现进程只能单次运行
	*/
	ret = open(FILENAME, O_RDWR|O_TRUNC|O_CREAT|O_EXCL);

	if(ret < 0)
	{
		if (errno == EEXIST)
		{
			printf("the process exist, don't run again\n");
			return -1;
		}
	}
	

	//当进程退出的时候要删除锁文件
	atexit(delete_file);
	int i;
	for(i=0; i<15; i++)
	{
		sleep(1);
		printf("the process is running\n");
	}
		
	return 0;
}


void delete_file(void)
{
	//remove用来删除一个文件，参数为文件名
	remove(FILENAME);
}
#endif 


