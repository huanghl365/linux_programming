#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
