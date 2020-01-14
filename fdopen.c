#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
FILE * fdopen(int fildes,const char * mode);
描述：fdopen取一个现存的文件描述符，并使一个标准的I / O流与该描述符相结合。此函数常用于由创建管道和网络通信通道函数获得的描述符。
因为这些特殊类型的文件不能用标准I/O fopen函数打开，首先必须先调用设备专用函数以获得一个文件描述符，然后用fdopen使一个标准I/O流与该描述符相结合。
对于fdopen，type参数的意义则稍有区别。因为该描述符已被打开，所以 fdopen 当mode时为"w" 并不截断该文件。
(例如，若该描述符原来是由 open函数打开的，该文件那时已经存在，则其O_TRUNC标志将决定是否截断该文件）。
另外，fdopen 不能用于创建该文件（因为如若一个描述符引用一个文件，则该文件一定已经存在）。
参数：
fildes：已经打开的文件描述符
mode：文件打开方式，与fopen一样
返回值：
转换成功时返回指向该流的文件指针。失败则返回NULL，并把错误代码存在errno中。
*/

/*
程序功能描述：简单测试fdopen函数，将文件描述符以文件流的方式打开，并使用C库文件流函数操作
*/
#define MAX_BUF_SIZE 256
int main(void)
{
	int fd = -1;
	FILE *streamptr = NULL;
	char streambuf[MAX_BUF_SIZE] = "\0";
	char *searchptr = NULL;
	fd = open("test.txt", O_RDONLY);
	if (-1 == fd)
	{
		perror("open");
		exit(1);
	}
	else
	{
		streamptr = fdopen(fd, "r");  //将文件描述符以文件流的只读方式打开
		if (NULL == streamptr)
		{
			perror("fdopen");
			streamptr = NULL;
			exit(1);
		}
		else
		{
			while (NULL != fgets(streambuf, MAX_BUF_SIZE, streamptr)) //循环读取文件的每行
			{
				if (NULL !=  (searchptr = strstr(streambuf, "printf")) ) //判断每行中是否有printf
				{
					printf("%s", searchptr);
					memset(streambuf, 0, sizeof(streambuf));
				}
			}
			close(fd);
			fclose(streamptr);
		}
		
	}
	return 0;
}
