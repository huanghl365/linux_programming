#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

/*
int fcntl(int fd, F_GETFL);
int fcntl(int fd, F_SETFL, ... arg);
描述：获取及设置文件描述符flags
参数:
fd:文件描述符
cmd: 
F_GETFL	获取文件描述符flags，忽略参数arg
F_SETFL	通过arg参数设置文件描述符flags
arg:
文件描述符的flags

返回值：
F_GETFL	返回文件描述符flags
F_SETFL	失败返回-1
*/

/*
函数功能描述：从键盘和鼠标以非阻塞方式读取数据
*/
int main(void)
{

	char buf[100] = {'\0'};
	int ret = -1;
	int fd = -1;
	int flag = -1;
	

	//0：标准输入，即键盘设备，默认已经阻塞打开
	//将标准输入文件描述符修改为非阻塞
	flag = fcntl(0, F_GETFL);		
	flag |= O_NONBLOCK;				
	fcntl(0, F_SETFL, flag);		
	

	//非阻塞方式打开鼠标设备
	fd = open("/dev/input/mice", O_RDONLY|O_NONBLOCK);  
	if(fd == -1)
	{
		perror("error");
		exit(1);
	}


	//非阻塞轮询键盘和鼠标输入
	while(1)
	{
		
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if(ret > 0)
		{
			printf("get data from key board：%s\n", buf);
		}

		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, sizeof(buf));
		if(ret > 0)
		{
			printf("get data from mice：%s\n", buf);
		}
	}
	return 0;
}

