#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


/*
程序功能描述：运行多个进程，并调用wirte向同一个文件写入数据，分析文件数据有没有错乱以判断write是不是原子操作
*/

#if 0
#define TEST_FILE "./test.txt"
#define WRITE_CNT	1024
int main(void)
{
	char write_buf[50] = "\0";
	unsigned int write_cnt = 0;
	int fd, ret;
	
	//O_APPEND 	 续写文件
	fd = open(TEST_FILE, O_WRONLY | O_CREAT  | O_APPEND , S_IRUSR | S_IWUSR);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	
	while(1)
	{

		/*
		测试：同时运行多个进程去写文件  ，命令如下：
		for i in 1 2 3 4 5;do  ./a.out  & done
			
		执行cat test.txt | grep <pid> 命令分析结果，观察写入是否正常

		测试结果：如果write不是原子操作，那么多个进程写入的数据后，文件内容应该会混乱，但实际结果却是正常的
		原因：网上的博客说write系统调用在buf大小不超过内核缓存的时候是原子操作
		*/
		
		if (write_cnt < WRITE_CNT)
		{
			memset(write_buf, 0, sizeof(write_buf));
			//写入进程PID和次数，后面好进行分析
			sprintf(write_buf, "pid:%d write cnt:%u\n", getpid(), write_cnt);
		}
		else
			break;

		
		ret = write(fd, write_buf, strlen(write_buf));
		if (-1 != ret)
		{
			//printf("write %s : %s\n", TEST_FILE, write_buf);
		}
		else
		{
			perror("write");
			close(fd);
			exit(1);
		}

		write_cnt++;
	}

	
	close(fd);
	return 0;
}
#endif 
