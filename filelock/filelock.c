#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
/*
程序功能描述：使用文件锁实现进程只能运行一次
*/

#if 0
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


/*
程序功能描述：使用文件锁实现 资源独占式访问
*/

#if 0
#define	FILELOCK 	"./file.lock"
int creat_file_lock(void)
{
	int fd_lock;
	while(1)
	{
		fd_lock = open(FILELOCK, O_RDWR|O_TRUNC|O_CREAT|O_EXCL);
		if (fd_lock < 0)
		{
			printf("creat %s failed\n", FILELOCK);
			fflush(stdout);
			usleep(100 * 1000);//注意这里的延时不能省略，否则进程会占用太多CPU时间
			continue;
		}
		else
		{
			close(fd_lock);
			break;
		}
	}
	
	return 0;
}

int remove_file_lock(void)
{
	int ret;
	ret = unlink(FILELOCK);
	if (ret != 0)
	{
		printf("unlink %s fail\n", FILELOCK);
		fflush(stdout);
		return -1;
	}
	return 0;
}


int main(void)
{
	int get_lock_cnt = 0;


	while(1)
	{
		creat_file_lock();

		
		get_lock_cnt++;
		if (get_lock_cnt > 1000000) get_lock_cnt = 0;
 		printf("pid:%d get_lock_cnt:%d\n", getpid(), get_lock_cnt);
		printf("now locking...\n");

		/*
		测试：这里加一个较长的延时，同时运行几个进程，当占用锁的时候，观察其他进程是不是阻塞
		测试结果：其他进程被阻塞，文件锁是能生效的
		*/
		sleep(10); 

		
		remove_file_lock();
		
		printf("now unlock!!!\n");
		usleep(200*1000); //适当延时，避免进程长时间抢占锁
	}
	
}

#endif 

