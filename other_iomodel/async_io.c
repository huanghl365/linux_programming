#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

/*
int fcntl(int fd, F_SETOWN, pid);
描述：获取及设置文件描述符flags
参数:
fd:文件描述符
cmd: 
F_SETOWN	设定文件描述符的属主，即文件描述符上可执行IO时会接收到通知信号的进程或进程组。
arg:
指定进程或进程组的pid。
返回值：
F_SETOWN	失败返回-1
*/


/*
程序功能描述：测试信号驱动IO的使用
*/

int fd = -1;
void async_io_func(int sig)
{
	char buf[200] = {0};
	int ret = -1;
	if (sig != SIGIO)
		return;
	ret = read(fd, buf, 50);
	if (ret > 0)
	{
		printf("get data from mice：%s\n", buf);
	}
}

int main(void)
{

	char buf[100] = {'\0'};
	int ret = -1;
	int flag = -1;
	
	fd = open("/dev/input/mice", O_RDONLY);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

//正常使用信号驱动IO
#if 0
	signal(SIGIO, async_io_func);			// 注册SIGIO信号处理函数
	
	flag = fcntl(fd, F_GETFL);				
	//设置O_ASYNC使文件描述符具有信号驱动IO属性，这样文件描述符才会向内核通知IO就绪状态
	fcntl(fd, F_SETFL, flag | O_ASYNC);		

	//设定文件描述符的属主，即文件描述符上可执行IO时会接收到通知信号的进程
	fcntl(fd, F_SETOWN, getpid());  		
		
	while(1)  //阻塞读取键盘数据
	{
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if(ret > 0)
		{
			printf("get data from key board：%s\n", buf);
		}

	}

#endif

/*
测试：先配置信号驱动IO，然后在注册SIGIO处理函数之前 发送SIGIO信号
测试结果：程序会挂掉，应该先注册SIGIO信号处理函数再配置信号驱动IO*/

#if 1

	
	flag = fcntl(fd, F_GETFL);				
	fcntl(fd, F_SETFL, flag | O_ASYNC);		

	fcntl(fd, F_SETOWN, getpid());  		

	sleep(10);
	signal(SIGIO, async_io_func);			
		
	while(1)  
	{
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if(ret > 0)
		{
			printf("get data from key board：%s\n", buf);
		}

	}
#endif
	close(fd);
	return 0;
}

