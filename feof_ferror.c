#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_SIZE 1024

/*
函数原型：int feof(FILE *stream);
描述：判断文件流是不是到达文件结尾
参数：
stream：文件流指针
返回值：
如果到达文件尾，那么feof返回值为非零，否则返回值为0

函数原型：int ferror(FILE *stream);
描述：判断文件流是否发生错误 
参数：
stream：文件流指针
返回值：有错误返回非零，否则返回0

函数原型：void clearerr(FILE *stream);
描述：用来清除文件流的文件尾标识和错误标识
参数：文件流指针
返回值：无
*/

/*
程序功能描述：测试feof和ferror的使用
*/
int main(void)
{
	FILE *fp1;  //指针前面一定加上*
	char ch,ch2;
	char read_buffer[READ_SIZE] = "\0";
	int ret =0 ;
	fp1 = fopen("test.txt", "r+");  //更新方式打开，读写


	while(NULL != fgets(read_buffer, READ_SIZE, fp1))  
	{
		printf("%s", read_buffer);
		memset(read_buffer, 0, READ_SIZE);
	} 
	
	//判断文件是否到达文件尾
	if (ret = feof(fp1) != 0)
	{
		printf("ret = %d the file is at end\n", ret);
	}
	
	//判断文件操作是否产生错误
	if (ferror(fp1) != 0)
	{
		printf("the file operation have error\n");
	}
	//clearerr(fp1); //清除文件指针指向的文件流的文件尾标识和错误标识
	
	if (feof(fp1) != 0)
	{
		printf("the file is at end\n");
	}
	fclose(fp1);
	return 0;
}
