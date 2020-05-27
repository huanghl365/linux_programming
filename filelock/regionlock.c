#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
函数基本原型：int fcntl(int fd, int cmd, ... arg);
描述：
fcntl对打开的文件描述符fd执行cmd指定的操作，函数具体原型由cmd决定
参数：
fd:文件描述符
cmd:指定操作
arg:指定操作的参数
返回值：
由cmd操作决定
*/

/*
函数具体原型：int fcntl(int fd, int cmd, struct flock *flock_structure);
描述：
做文件区域锁定
参数
fd:文件描述符

cmd:区域锁定操作
F_GETLK	获取打开的文件fd的锁信息
F_SETLK	对fd指向的文件的某个区域加锁或解锁
F_SETLKW	与F_SETLK作用一样，不用的是获取不到锁时会一直等待知道获取成功

----------------
flock_structure:区域锁定结构体，结构体成员如下：
short l_type
short l_whence
short l_start
off_t l_len
off_t l_pid

type：指定锁类型，锁类型如下所示：
F_RDLCK 建立共享锁（读锁），区域被一个进程设置共享锁之后，可以被其他进程设置共享锁，但是不能设置独占锁
F_WRLCK 建立独占锁（写锁），区域被一个进程设置独占锁之后，不能被其他进程设置共享锁和独占锁
F_UNLCK 解锁，即清除锁


l_whence：指定区域起始位置，SEEK_CUR、SEEK_END、SEEK_SET中的一个
l_start：指定区域的第一个字节
l_len：指定区域字节数
l_pid：记录持有锁的进程
-----------------

返回值：
F_GETLK和F_SETLK调用成功返回非-1，失败返回-1。
F_SETLKW等待获取才会返回。
*/




const char *test_file = "./region_lock";
void show_lock_info(struct flock *to_show) {
    printf("\tl_type %d, ", to_show->l_type);
    printf("l_whence %d, ", to_show->l_whence);
    printf("l_start %d, ", (int)to_show->l_start);        
    printf("l_len %d, ", (int)to_show->l_len);
    printf("l_pid %d\n", to_show->l_pid);
}

#if 1
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


/*
测试：当前进程锁定区域后，查询锁定区域的锁信息
测试结果：区域虽然加了锁，但是查询的时候发现区域是处于解锁状态，
说明同个进程可以对同一个区域反复加锁。
*/
#if 0
	region_to_test.l_type = F_WRLCK;
	region_to_test.l_whence = SEEK_SET;
	region_to_test.l_start = 10;
	region_to_test.l_len = 20;
	region_to_test.l_pid = -1;	   
	

	printf("Testing F_WRLCK on region from %d to %d\n", 
		   10, 30);
	
	res = fcntl(file_desc, F_GETLK, &region_to_test);
	if (res == -1) {
		fprintf(stderr, "F_GETLK failed\n");
		exit(1);
	}
	
	if (region_to_test.l_pid != -1) {
		printf("Lock would fail. F_GETLK returned:\n");
	}
	else {
		printf("F_WRLCK - Lock would succeed\n");
	}
	show_lock_info(&region_to_test);

	region_to_test.l_type = F_WRLCK;
	region_to_test.l_whence = SEEK_SET;
	region_to_test.l_start = 40;
	region_to_test.l_len = 10;
	region_to_test.l_pid = -1;	  
	printf("Testing F_WRLCK on region from %d to %d\n", 
		   40, 50);
	
	res = fcntl(file_desc, F_GETLK, &region_to_test);
	if (res == -1) {
		fprintf(stderr, "F_GETLK failed\n");
		exit(1);
	}
	
	if (region_to_test.l_pid != -1) {
		printf("Lock would fail. F_GETLK returned:\n");
	}
	else {
		printf("F_WRLCK - Lock would succeed\n");
	}
	show_lock_info(&region_to_test);
#endif 
		
	sleep(30);

	printf("Process %d closing file\n", getpid());	  
	close(file_desc);
	//进程退出会自动释放锁
	exit(0);
}
#else
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
	for (start_byte = 0; start_byte < 99; start_byte += SIZE_TO_TRY) {

	
		region_to_test.l_type = F_WRLCK; //独占锁
		region_to_test.l_whence = SEEK_SET;
		region_to_test.l_start = start_byte;
		region_to_test.l_len = SIZE_TO_TRY;
		/*这里要设置l_pid为-1，如果能够设置锁的话，l_pid是不会被刷新的，表示
		进程没有被独占，因此我们可以通过判断l_pid是否为-1确定能否设置锁*/
		region_to_test.l_pid = -1;	   
		

		printf("Testing F_WRLCK on region from %d to %d\n", 
			   start_byte, start_byte + SIZE_TO_TRY);
		
		res = fcntl(file_desc, F_GETLK, &region_to_test);
		if (res == -1) {
			fprintf(stderr, "F_GETLK failed\n");
			exit(1);
		}

		//查询的时候，l_type会被刷新，因此也可以查询l_type来判断是否可以加锁
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
	
	close(file_desc);
	exit(0);
}
#endif

