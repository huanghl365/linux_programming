
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>
/*
函数原型：int epoll_create(int size);
描述：用来创建一个新的epoll实例用来检查文件描述符IO
参数：
size：指定要检查的文件描述符的数目，从linux内核版本2.6.8开始，这个参数被忽略，但是必须大于0
返回值：
成功返回一个非负数的epoll实例，失败返回-1

函数原型：int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
描述：用来对epoll_create创建的epoll实例进行配置。
参数：
epfd：epoll实例
op:指定操作类型，主要是以下三种
EPOLL_CTL_ADD
	将文件描述符fd添加到epoll中并指定事件
EPOLL_CTL_MOD
	修改已经添加到epoll实例中的文件描述符的事件
EPOLL_CTL_DEL
	删除添加到epoll实例中的文件描述符，此操作忽略event

fd：指定需要执行操作的文件描述符
--------------------------------
event：指向配置文件描述符事件的结构体的指针
-struct epoll_event {
	uint32_t	 events;	  // Epoll events 
	epoll_data_t data;		  // User data variable 
};
events：掩码形式指定事件，常用事件如下
	EPOLLIN		输入事件
	EPOLLOUT	输出事件
data：指定文件描述符就绪时，回传给调用进程的用户数据，其中fd是一定要指定的，
这样才能知道就绪事件是在哪个文件描述符上发生的。
typedef union epoll_data {
               void        *ptr;
               int          fd;
               uint32_t     u32;
               uint64_t     u64;
           } epoll_data_t;	
--------------------------------
返回值：调用成功返回0，失败返回-1.

函数原型：int epoll_wait(int epfd, struct epoll_event *events,
                      int maxevents, int timeout);
描述：返回处于就绪的文件描述符信息
参数：
epfd：epoll实例
events：events指向的结构体数组由用户申请，保存返回的文件描述符就绪
信息（包含就绪事件以及就绪文件描述符）。
maxevents：指定events对应的结构体数组元素个数。
timeout：指定超时时间，单位为milliseconds 毫秒
-1 表示无限等待
0  表示非阻塞
返回值：
返回就绪的文件描述符个数，错误返回-1，超时返回0.

注意：
epoll_wait调用返回后，再次进行epoll_wait时，无需再次对描述符事件进行配置

*/

/*
函数功能描述：使用epoll实现多路复用，检测鼠标和键盘输入
*/

#define MAX_EVENTS     5  
int main(void)
{
	int ret = -1;
	char buf[100] = {0};
	int j;
	
    int epfd, ready, fd;
    struct epoll_event ev;
    struct epoll_event evlist[MAX_EVENTS];

	
	fd = open("/dev/input/mice", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}

	epfd = epoll_create(2);
	if (epfd == -1)
	{
		perror("epoll_create");
		exit(1);
	}

	ev.events = EPOLLIN;			
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
	{
		perror("epoll_ctl");
		exit(1);
	}

	ev.events = EPOLLIN;	
	ev.data.fd = 0;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &ev) == -1)
	{
		perror("epoll_ctl");
		exit(1);
	}



	while(1)
	{
		ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        if (ready == -1) 
		{
            if (errno == EINTR)
                continue;               /* Restart if interrupted by signal */
            else
                perror("epoll_wait");
        }

		printf("ready = %d\n", ready);
		for (j = 0; j < ready; j++) 
		{
			if (evlist[j].events & EPOLLIN) {
				if (fd == evlist[j].data.fd)
				{
					memset(buf, 0, sizeof(buf));
					ret = read(fd, buf, sizeof(buf));
					if(ret > 0)
					{
						printf("get data from key board：%s\n", buf);
					}
					
				}else if (0 == evlist[j].data.fd)
				{
					memset(buf, 0, sizeof(buf));
					ret = read(0, buf, sizeof(buf));
					if(ret > 0)
					{
						printf("get data from key board：%s\n", buf);
					}
				}

			} 
		}
		
	}

	close(fd);
	close(epfd);
	return 0;
}

