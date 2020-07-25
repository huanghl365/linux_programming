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
程序功能描述：查询文件区域能否设置共享锁以及独占锁
程序名：lock2
*/
#define SIZE_TO_TRY 5
int main() {
	int file_desc;
	int res;
	struct flock region_to_test;
	int start_byte;

	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (!file_desc) {
		fprintf(stderr, "Unable to open %s for read/write", test_file);
		exit(1);
	}

	/*
	测试：先运行lock1程序给文件部分区域加锁	共享锁：10-30 byte 独占锁：40-50 byte
	然后运行本进程，查询文件区域能否设置共享锁以及独占锁
	
	测试结果：
	10-30 byte是共享锁，因此其他进程可以设置共享锁，不能设置独占锁
	40-50 byte是独占锁，因此其他进程共享锁和独占锁都无法设置
	其他文件区域都是不加锁的，因此其他进程可以设置共享锁或者独占锁
	*/
#if 1
	for (start_byte = 0; start_byte < 99; start_byte += SIZE_TO_TRY) {

		/*
		有两种方式可以判断区域能否加锁：

		1、设置l_pid为-1，如果区域能够设置锁的话，F_GETLK查询之后 l_pid不会被设置为进程ID，
		表示区域没有被独占。

		2、F_GETLK查询之后，如果区域能够设置锁的话，l_type会被设置为F_UNLCK，表示区域没有被独占。
		*/
		
		region_to_test.l_type = F_WRLCK; //独占锁
		region_to_test.l_whence = SEEK_SET;
		region_to_test.l_start = start_byte;
		region_to_test.l_len = SIZE_TO_TRY;
		region_to_test.l_pid = -1;	   
		
		printf("Testing F_WRLCK on region from %d to %d\n", 
			   start_byte, start_byte + SIZE_TO_TRY);
		
		res = fcntl(file_desc, F_GETLK, &region_to_test);
		if (res == -1) {
			fprintf(stderr, "F_GETLK failed\n");
			exit(1);
		}

		
		if (region_to_test.l_type != F_UNLCK) {
		//if (region_to_test.l_pid != -1) {
			printf("Lock would fail. F_GETLK returned:\n");
			//show_lock_info(&region_to_test);
		}
		else {
			printf("F_WRLCK - Lock would succeed\n");
		}
		show_lock_info(&region_to_test);
		
		region_to_test.l_type = F_RDLCK; //共享锁
		region_to_test.l_whence = SEEK_SET;
		region_to_test.l_start = start_byte;
		region_to_test.l_len = SIZE_TO_TRY;
		region_to_test.l_pid = -1;	   

		printf("Testing F_RDLCK on region from %d to %d\n", 
			   start_byte, start_byte + SIZE_TO_TRY);
		
		res = fcntl(file_desc, F_GETLK, &region_to_test);
		if (res == -1) {
			fprintf(stderr, "F_GETLK failed\n");
			exit(EXIT_FAILURE);
		}
		if (region_to_test.l_type != F_UNLCK) {
		//if (region_to_test.l_pid != -1) {
			printf("Lock would fail. F_GETLK returned:\n");
			//show_lock_info(&region_to_test);			
		}
		else {
			printf("F_RDLCK - Lock would succeed\n");
		}
		show_lock_info(&region_to_test);	
	} 
#endif 

	close(file_desc);
	exit(0);
}
