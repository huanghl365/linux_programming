#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>


/*
函数功能描述：使用poll实现多路复用，检测鼠标和键盘输入
*/
int main(void)
{
	int fd = -1;
	int ret = -1;
	char buf[100] = {0};
	struct pollfd fds[2] = {0};
	
	int flag = -1;
	fd = open("/dev/input/mice", O_RDONLY|O_NONBLOCK);
	if (fd < 0)
	{
		perror("open:");
		exit(-1);
	}

	flag = fcntl(0, F_GETFL);		
	flag |= O_NONBLOCK; 			
	fcntl(0, F_SETFL, flag);	

	fds[0].fd = 0;
	//fds[0].fd = -1;			//标准输入，默认键盘
	fds[0].events = POLLIN;
	fds[1].fd = fd; 			//鼠标
	fds[1].events = POLLIN; 

	while(1)
	{
	
		ret = poll(fds, fd+1, 10000); //等待10s
		
		if(ret == -1)
		{
			perror("error");
			exit(-1);
		}
		else if(ret == 0)
		{
			printf("timeout\n");
		}
		else if(ret > 0)
		{
			if(fds[0].revents & POLLIN)
			{
				memset(buf, 0, sizeof(buf));
				ret = read(0, buf, sizeof(buf));
				if(ret > 0)
				{
					printf("get data from key board：%s\n", buf);
				}
			}
			
			if(fds[1].revents & POLLIN)
			{
				memset(buf, 0, sizeof(buf));
				ret = read(fd, buf, sizeof(buf));
				if(ret > 0)
				{
					printf("get data from mice：%s\n", buf);
				}
			}
		
		}

	}

	close(fd);
	
	
	return 0;
}

