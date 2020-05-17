#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：将同一个文件打开两次，使用O_APPEND关联文件指针，测试能否实现交替读写
*/
int main(int argc, char *argv[])
{
	int fd1 = -1; 
	int fd2 = -1;
	int ret = -1;
	char buf[100] = {'\0'};
	
#if 0

	//将一个文件打开两次，推测内存中只有一份
	//使用O_APPEND可以关联两个文件管理表的文件指针，测试能否实现交替写
	fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	
		//fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); 
		//fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); 

	if (-1 == fd1||-1 == fd2)							  						
	{										 							
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("打开文件成功!\n");
	}
	
	
	
	while(1)
	{
		//测试交替写实验
		//测试结果：证明是交替写
		ret = write(fd1, "aa",2);
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd1\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		ret = write(fd2, "bb", 2);
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd2\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
	
		
#else

	//使用O_APPEND可以关联两个文件管理表的文件指针，测试能否实现交替读
	fd1 = open("test.txt", O_RDWR|O_APPEND); 
	fd2 = open("test.txt", O_RDWR|O_APPEND); 
	
	
	if (-1 == fd1||-1 == fd2)							  						
	{										 							
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("打开文件成功!\n");
	}
	
	while(1)
	{
		//测试交替读实验
		//测试结果：O_APPEND无法实现交替读，这里也说明O_APPEND只是用来做接续写操作的
		memset(buf, 0, sizeof(buf));
		ret = read(fd1, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd1：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		memset(buf, 0, sizeof(buf));
		ret = read(fd2, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd2：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
	
#endif
	
	//关闭文件
	ret = close(fd1);
	if(-1 != ret)
		printf("成功关闭文件1!\n");
	
	ret = close(fd2);
	if(-1 != ret)
		printf("成功关闭文件2!\n");
	
	return 0;
	
}


