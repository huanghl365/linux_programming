#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：测试给整个文件加锁
*/
const char *test_file = "./region_lock";
int lock_file(int file_fd)
{
	
	struct flock region_1;
	int res;
	region_1.l_type = F_WRLCK;
	region_1.l_whence = SEEK_SET;
	region_1.l_start = 0;	//l_start  l_len 都为0表示锁定整个文件
	region_1.l_len = 0; 
	
	res = fcntl(file_fd, F_SETLKW, &region_1);   //等待方式
	if (res == -1) 
	{
		fprintf(stderr, "Failed to lock file\n");
		return -1;
	}
	else
	{
		printf("lock file success\n");
		
		usleep(20*1000);//预留一点时间给其他进程关闭文件
		return 0;
	}

}

int unlock_file(int file_fd)
{
	struct flock region_1;
	int res;
	region_1.l_type = F_UNLCK;
	region_1.l_whence = SEEK_SET;
	region_1.l_start = 0;
	region_1.l_len = 0;
	printf("Process %d, trying F_UNLCK file", getpid());
		
	res = fcntl(file_fd, F_SETLK, &region_1);
	if (res == -1) {
		printf("Process %d - failed to unlock file\n", getpid());
		return -1;
	} else {
		printf("Process %d - unlocked file\n", getpid());
		return 0;
	}
}

int main()
{
	int res;
	struct flock region_1;
	int file_desc=10;
	int i=0;

	
	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (!file_desc) {
		fprintf(stderr, "Unable to open %s for read/write\n", test_file);
		exit(1);
	}

	lock_file(file_desc);


	while(i<30)
	{
		sleep(1);
		printf("sleep %d\n", i++);
	}
	

	unlock_file(file_desc);
	
	close(file_desc);
	return 0;
}
