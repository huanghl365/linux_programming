#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>

/*
函数原型：int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
描述：select实现外部阻塞，内部非阻塞自动轮询检测多路阻塞IO
参数：
nfds：指定轮询文件描述符的数目，一般为集合中最大的文件描述符+1
readfds：检测输入是否就绪的文件描述符集合
writefds：检测输出是否就绪的文件描述符集合
exceptfds：检测异常是否发生的文件描述符集合
timeout：指定超时时间
当就绪时，timeout返回剩余时间
直接设置timeout为NULL表示无限等待
tv_sec和tv_usec为0表示不阻塞
struct timeval {
	long	tv_sec; 		// seconds 
	long	tv_usec;		// microseconds 
};
返回值：
错误返回-1
超时返回0，并且清空集合
返回正整数表示各个集合中就绪的文件描述符总数

注意：
当select就绪返回后，因为集合会被修改，因此再次select时，需要重新初始化集合。

------------------
void FD_ZERO(fd_set *set);			//清空
void FD_SET(int fd, fd_set *set);	//添加
void FD_CLR(int fd, fd_set *set);	//移除
int  FD_ISSET(int fd, fd_set *set); //判断哪路IO就绪
*/


/*
函数功能描述：使用select实现多路复用，检测鼠标和键盘输入
*/
int main(void)
{
	int fd = -1;
	int ret = -1;
	char buf[100] = {0};
	fd_set set;
		struct timeval tm;
	
	fd = open("/dev/input/mice", O_RDONLY);
	if (fd < 0)
	{
		perror("open:");
		exit(-1);
	}

	while(1)
	{
		FD_ZERO(&set);   //select之前要初始化集合
		FD_SET(fd, &set); 
		FD_SET(0, &set);  
		
		tm.tv_sec = 10;
		tm.tv_usec = 100000;
		
		ret = select(fd+1, &set, NULL, NULL, &tm);
		
		if(-1 == ret)
		{
			perror("error");
			exit(-1);
		}
		else if(0 == ret)
		{
			printf("timeout\n");
		}
		else if(ret > 0)
		{
			
			if(FD_ISSET(fd, &set))
			{
				memset(buf, 0, sizeof(buf));
				ret = read(fd, buf, sizeof(buf));
				if(ret > 0)
				{
					printf("get data from mice：%s\n", buf);
				}
			}
			
			if(FD_ISSET(0, &set))
			{
				memset(buf, 0, sizeof(buf));
				ret = read(0, buf, sizeof(buf));
				if(ret > 0)
				{
					printf("get data from key board：%s\n", buf);
				}
			}
			printf("the rest time: %lds %ldus\n", tm.tv_sec, tm.tv_usec); //打印剩余时间
		}
	}

	close(fd);
	return 0;
}
