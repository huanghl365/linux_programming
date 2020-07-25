#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：使用read / write 读写标准输入输出描述符
*/

/*
0 		表示标准输入的文件描述符
1		表示标准输出的文件描述符
stdin	表示标准输入的文件指针 
stdout	表示标准输出的文件指针
*/

int main(void)
{
	int nread = 0;
	int nwrite = 0;
	char readbuf[128];
	
	nread = read(0, readbuf, sizeof(readbuf));
	if (nread != -1 )
	{
	nwrite = write(1, readbuf, nread);
	}
	return 0;
}

