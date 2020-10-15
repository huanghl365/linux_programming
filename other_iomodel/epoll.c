
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

