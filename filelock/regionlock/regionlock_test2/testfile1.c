#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：给文件区域设置独占锁，隔一段时间后解锁
程序名：lock1
*/

const char *test_file = "./region_lock";

int main() 
{
	int file_desc;
	int byte_count;
	char *byte_to_write = "A";
	struct flock region_1;
	struct flock region_2;
	
	struct flock region_to_test;
	int res;

	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (!file_desc) {
		fprintf(stderr, "Unable to open %s for read/write\n", test_file);
		exit(1);
	}

	for(byte_count = 0; byte_count < 100; byte_count++) {
		(void)write(file_desc, byte_to_write, 1);
	}


	//设置独占锁：40-50 byte
	region_2.l_type = F_WRLCK;
	region_2.l_whence = SEEK_SET;
	region_2.l_start = 40;
	region_2.l_len = 10;

	printf("Process %d locking file\n", getpid());
	res = fcntl(file_desc, F_SETLK, &region_1);
	if (res == -1) 
		fprintf(stderr, "Failed to lock region 1\n");
	res = fcntl(file_desc, F_SETLK, &region_2);
	if (res == -1) 
		fprintf(stderr, "Failed to lock region 2\n");	


#if 1
	sleep(30);
	//释放独占锁
	region_2.l_type = F_UNLCK;
	region_2.l_whence = SEEK_SET;
	region_2.l_start = 40;
	region_2.l_len = 10;
	printf("Process %d, trying F_UNLCK, region %d to %d\n", getpid(),
		   (int)region_2.l_start, (int)(region_2.l_start + region_2.l_len));
	res = fcntl(file_desc, F_SETLK, &region_2);
	if (res == -1) {
		printf("Process %d - failed to unlock region 2\n", getpid());
	} else {
		printf("Process %d - unlocked region 2\n", getpid());
	}
	
	sleep(30);
#endif 

	printf("Process %d closing file\n", getpid());	  
	close(file_desc);
	exit(0);
}


