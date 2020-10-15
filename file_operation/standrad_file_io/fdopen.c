#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：简单测试fdopen函数
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
		streamptr = fdopen(fd, "r");  //只读方式打开描述符
		if (NULL == streamptr)
		{
			perror("fdopen");
			streamptr = NULL;
			exit(1);
		}
		else
		{
			while (NULL != fgets(streambuf, MAX_BUF_SIZE, streamptr)) 
			{
				if (NULL !=  (searchptr = strstr(streambuf, "printf")) ) 
				{
					printf("%s", searchptr);
					memset(streambuf, 0, sizeof(streambuf));
				}
			}
			close(fd);
			fclose(streamptr); //关闭由fdopen打开的流时，对应的文件描述符也会被关闭
		}
		
	}
	return 0;
}
