#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
描述：getline用于读取一行字符直到换行符,包括换行符。它会生成一个包含一串从输入流读入的字符的字符串，
直到以下情况发生会导致生成的此字符串结束。1）到文件结束，2）遇到函数的定界符，3）输入达到最大限度。

参数：
lineptr：指向存放该行字符的指针，如果是NULL，则由系统帮助malloc，请在使用完成后free释放。
n：如果是由系统malloc的指针，请填0。
stream：文件描述符
返回值：返回读取的字节数，失败返回-1。
*/

/*
程序功能描述：简单测试getline调用，读取文件的每一行并打印
*/
int main(void)
{
	FILE *streamptr = fopen("test.txt", "r");
	char *lineptr = NULL;
	size_t len = 0;
	if (streamptr == NULL)
	{
		perror("fopen:");
	}
	else
	{
		perror(NULL);
		//传参时第一个参数只能传指针的取地址，不能传递指向指针的指针
		while (getline(&lineptr, &len, streamptr) != -1)
		{
			fprintf(stdout, "%s", lineptr);
		}
			
	}
	free(lineptr);
	fclose(streamptr);
	return 0;
	
}
