#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

/*
程序功能描述：编译生成toupper.out，用来将输入的小写字母替换为大写字母
*/
#if 0
int main(int argc, char *argv[])
{
	int ch;
	while ((ch = getchar()) != EOF)
	{
		putchar(toupper(ch));
	}
	return 0;

}
#endif

/*
程序功能描述：使用freopen重定向文件流为标准输入，给execl打开的程序输入数据，将小写字母替换为大写字母
*/
#if 1
int main(int argc, char *argv[])
{

	FILE *fptr = NULL;
	if (argc != 2)
	{
		printf("usage: ./a.out filename");
		exit(1);
	}

	fptr = freopen(argv[1], "r", stdin); //打开文件，并且重新打开标准输入，将文件流绑定到标准输入
	if (fptr == NULL)
	{
		perror("freopen:");
		exit(2);
	}
	else
	{
		//已经打开的文件流在执行execl之后会保留下来,因此文件流会通过标准输入重定向给toupper.out
		execl("./toupper.out", "toupper.out", NULL);
		printf("done");
		exit(3);
	}

	return 0;

}
#endif 
