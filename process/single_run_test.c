#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define FILENAME "./single_run.lock"
void delete_file(void);

/*
程序功能描述：测试一种 实现进程只能单次运行的方法
*/
int main(void)
{
	int ret = -1;
	//创建一个文件作为进程标志，进程结束时删除文件
	//当多次运行进程时由于文件存在，所以创建文件失败，进程无法多次运行
	ret = open(FILENAME, O_RDWR|O_TRUNC|O_CREAT|O_EXCL);

	if(ret < 0)
	{
		if (errno == EEXIST)
		{
			printf("the process exist, don't run again\n");
			return -1;
		}
	}
	
	//注册进程结束调用函数
	//注意进程正常退出时函数才会调用函数
	atexit(delete_file);
	int i;
	for(i=0; i<15; i++)
	{
		sleep(1);
		printf("the process is running\n");
	}
		
	return 0;
}



//删除进程标志文件
void delete_file(void)
{
	//remove用来删除一个文件，参数为文件名
	remove(FILENAME);
}

