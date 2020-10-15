#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


const char *test_file = "./region_lock";
void show_lock_info(struct flock *to_show) {
    printf("\tl_type %d, ", to_show->l_type);
    printf("l_whence %d, ", to_show->l_whence);
    printf("l_start %d, ", (int)to_show->l_start);        
    printf("l_len %d, ", (int)to_show->l_len);
    printf("l_pid %d\n", to_show->l_pid);
}



/*
程序功能描述：给文件区域设置共享锁和独占锁
程序名：lock1
*/

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

	//设置共享锁：10-30 byte
	region_1.l_type = F_RDLCK;
	region_1.l_whence = SEEK_SET;
	region_1.l_start = 10;
	region_1.l_len = 20; 

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
		
	sleep(30);

	printf("Process %d closing file\n", getpid());	  
	close(file_desc);
	//进程退出会自动释放锁
	exit(0);
}

