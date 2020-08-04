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

flock_structure:区域锁定结构体，结构体成员如下：
short l_type
short l_whence
short l_start
off_t l_len
off_t l_pid
--------------------------

type：指定锁类型，锁类型如下所示：
F_RDLCK 建立共享锁（读锁），区域被一个进程设置共享锁之后，可以被其他进程设置共享锁，但是不能设置独占锁
F_WRLCK 建立独占锁（写锁），区域被一个进程设置独占锁之后，不能被其他进程设置共享锁和独占锁
F_UNLCK 解锁，即清除锁


l_whence：指定区域起始位置，SEEK_CUR、SEEK_END、SEEK_SET中的一个
l_start：指定区域的第一个字节
l_len：指定区域字节数
l_pid：记录持有锁的进程
----------------------------

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

