#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_SIZE 1024
int main(void)
{
	FILE *fp1;  //指针前面一定加上*
	char ch,ch2;
	char read_buffer[READ_SIZE] = "\0";
	int ret =0 ;
	fp1 = fopen("/home/mxc/testfile", "r+");  //更新方式打开，读写


	while(NULL != fgets(read_buffer, READ_SIZE, fp1))  
	{
		printf("%s", read_buffer);
		memset(read_buffer, 0, READ_SIZE);
	} 
	
	//判断文件是否到达文件尾，如果到达文件尾，那么feof返回值为非零，否则返回值为0
	if (ret = feof(fp1) != 0)
	{
		printf("ret = %d the file is at end\n", ret);
	}
	
	//判断文件操作是否产生错误，有错误返回非零，否则返回0
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