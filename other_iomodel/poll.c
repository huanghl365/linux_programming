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
函数原型：int poll(struct pollfd *fds, nfds_t nfds, int timeout);
描述：poll实现函数外部阻塞式，内部非阻塞式自动轮询多路阻塞式IO
参数：
fds：指定轮询的文件描述符以及事件，结构体定义如下
------------
struct pollfd {
               int   fd;         
               short events;     
               short revents;     
           };

fd		：文件描述符
events	：指定为文件描述符检查的事件的位掩码，举例如下：
	POLLIN	输入事件
	POLLOUT	输出事件
	POLLERR	发生错误事件	
revents	：poll返回时，设定该文件描述符上发生的事件的位掩码
------------
nfds：指定轮询文件描述符的数目，一般为轮询中的最大的文件描述符+1
timeout：指定超时时间，单位为milliseconds 毫秒
-1 表示无限等待
0  表示非阻塞
返回值：
返回发生的事件的文件描述符的数目，不同于select，即使一个描述符发生多个事件也只会计算一次。
-1 表示发生错误
0  表示超时

注意：
poll就绪返回之后，再次poll时，无需对描述符的事件再次进行配置
*/

/*
如何屏蔽文件描述符的事件？
1、将events设置为0将忽略fd上发生的事件。
2、将fd设定为负数，将导致events上的事件被忽略，revents返回0。
*/


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

