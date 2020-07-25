
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/*
程序功能描述：使用文件锁实现进程互斥
*/

#if 1
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
			//这里要加一个延时，否则当抢不到文件锁时，会占用太多CPU
			usleep(50 * 1000);
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
		测试：这里加一个较长的延时，将本程序编译后同时运行几个进程，当占用锁的时候，观察其他进程是不是阻塞
		测试结果：其他进程被阻塞，文件锁是能生效的
		*/
		sleep(10); 
		
		remove_file_lock();
		
		printf("now unlock!!!\n");
		usleep(200*1000); //适当延时，给其他进程预留一点时间，避免进程一直抢占锁
	}
	
}

#endif 

