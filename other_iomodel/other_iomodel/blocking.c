#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


/*
函数功能描述：从键盘和鼠标以阻塞方式读取数据
*/
int main(void)
{
	char buf[100] = {'\0'};
	int ret = -1;
	int fd = -1;
	int flag = -1;

	//打开鼠标设备，默认阻塞
	fd = open("/dev/input/mice", O_RDONLY);  
	if(fd == -1)
	{
		perror("error");
		exit(1);
	}


	//阻塞轮询键盘和鼠标输入
	//阻塞式IO的最大缺点是无法实现同时多路读取，会一直阻塞在某个地方
	//while(1)
	//{

		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf)-1);
		if(ret > 0)
		{
			printf("get data from key board：%s\n", buf);
		}
		
	
		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, sizeof(buf)-1);  
		if(ret > 0)
		{
			printf("get data from mice：%s\n", buf);
		}
		
	
	//}
	return 0;
}

