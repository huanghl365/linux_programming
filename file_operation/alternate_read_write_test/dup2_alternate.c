#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：使用dup2复制文件描述符，测试能否交替读写
*/

int main(int argc, char *argv[])
{


	int fd1 = -1,fd2 = -1;
	int ret;
	char buf[10] = {'0'};
	
	fd1 = open("test.txt", O_RDWR | O_CREAT, 0666);
	if(-1 == fd1)
	{
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("文件打开成功\n");
		printf("文件描述符fd1为：%d\n", fd1);
	}
	
	
	//使用dup2复制文件描述符
	close(4);
	fd2 = dup2(fd1, 4); 
	

//测试：能否实现交替写
//测试结果：能够实现交替写
#if 0
	while(1)
	{
		ret = write(fd1, "aa", strlen("aa"));
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd1\n", strlen("aa"));
		}
		else
		{
			perror("写入字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		ret = write(fd2, "bb", strlen("bb"));
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
#endif

//测试：能否实现交替读
//测试结果：能够实现交替读
#if 1
	while(1)
	{
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
	
	close(fd1);
	close(fd2);
	return 0;
} 
