#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


/*
程序功能描述：使用dup、dup2测试复制文件描述符，绑定标准输出
*/
int main(int argc, char *argv[])
{
	int fd1 = -1,fd2 = -1;
	fd1 = open("test.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);
	if(-1 == fd1)
	{
		perror("open");
		exit(1);
	}
	else
	{
		printf("open success\n");
		printf("the fd1 is：%d\n", fd1);
	}
	//关闭标准输出描述符1，描述符1是标准输出
	close(1);
	
	//复制文件描述符，系统分配数字最小的描述符，上面因为关闭了标准输出，因此系统分配1，因此文件test.txt成为标准输出
	//fd2 = dup(fd1);

	//使用dup2可以指定文件描述符的号码
	fd2 = dup2(fd1,1);	
	
	//测试test.txt是不是标准输出
	printf("the fd2 is：%d\n", fd2);
	
	close(fd1);
	close(fd2);

	
	return 0;
} 


/*
程序功能描述：使用dup2复制文件描述符，测试能否交替读写
*/

#if 0
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
	

#if 0
	//测试能否实现交替写
	//测试结果：能够实现交替写，说明使用dup2可以实现同一文件的不同文件描述符的文件指针关联
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
#else
	//测试能否实现交替读
	//测试结果：能够实现交替读，说明使用dup2可以实现同一文件的不同文件描述符的文件指针关联
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


#endif


