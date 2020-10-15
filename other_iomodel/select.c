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
