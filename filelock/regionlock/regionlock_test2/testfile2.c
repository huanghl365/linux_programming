#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
程序功能描述：分别以阻塞方式和分阻塞方式给文件区域设置独占锁
程序名：lock2
*/

const char *test_file = "./region_lock";
int main()
{
	int file_desc;
	struct flock region_1;
	struct flock region_2;
	int res;

	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (!file_desc) {
		fprintf(stderr, "Unable to open %s for read/write\n", test_file);
		exit(1);
	}

	//设置独占锁：40-50 byte
	region_2.l_type = F_WRLCK;
	region_2.l_whence = SEEK_SET;
	region_2.l_start = 40;
	region_2.l_len = 10;
	
	/*
	测试：先运行lock1, 再运行lock2，观察阻塞和非阻塞方式获设置独占锁的区别
	测试结果：非阻塞方式 会立即返回， 阻塞方式会一直等待直到给区域加锁才返回
	*/
#if 1
	printf("Process %d locking file\n", getpid());
	//非等待方式
	res = fcntl(file_desc, F_SETLK, &region_2);
	if (res == -1) 
	{
		fprintf(stderr, "Failed to lock region 2\n");	
	}
	else
	{
		printf("lock region 2 success");
	}


	//等待方式
	res = fcntl(file_desc, F_SETLKW, &region_2);
	if (res == -1) 
	{
		fprintf(stderr, "Failed to lock region 2\n");	
	}
	else
	{
		printf("lock region 2 success");
	}
#endif
	close(file_desc);
	exit(0);
}
