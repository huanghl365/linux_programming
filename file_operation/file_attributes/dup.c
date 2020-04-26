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




