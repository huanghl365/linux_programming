#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_SIZE  1024
#define WRITE_SIZE  1024
int main(void)
{
	FILE *fp1;  //指针前面一定加上*
	char read_buffer[READ_SIZE] = "\0";
	char write_buffer[WRITE_SIZE] = "FUCK YOUR MOTHER";
	long int filesize = 0;
	//如果是r+,则读写都可以通过fseek定位到任意位置
	//如果是a+,则读可以通过fseek定位到任意位置，但是写只能接续写，无法通过fseek进行定位
	//fp1 = fopen("/home/mxc/testfile2", "r+");  //更新方式打开，读写
	fp1 = fopen("/home/mxc/testfile2", "a+");  //更新方式打开，接续写
	
	fseek(fp1, 10L, SEEK_SET);
	
	fread(read_buffer, 1, READ_SIZE, fp1);
	printf("%s\n", read_buffer);
	
	fseek(fp1, 10L, SEEK_SET);
	fwrite(write_buffer, 1, strlen(write_buffer), fp1);
	
	fseek(fp1, 0L, SEEK_END);
	filesize = ftell(fp1);		//配合fseek用来计算文件的大小
	printf("filesize = %ld\n", filesize);
	
	fclose(fp1);
	return 0;
}

