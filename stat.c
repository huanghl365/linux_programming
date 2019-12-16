#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
int main(void)
{
	int fd = open("/home/mxc/file", O_RDWR);
	struct stat statbuf;
	stat("/home/mxc/file", &statbuf);
	if (S_ISREG(statbuf.st_mode))   //S_ISREG 用来判断是否为普通文件的宏定义
	{
		printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	printf("\n-----\n");
	memset(&statbuf, 0, sizeof(statbuf));
	fstat(fd, &statbuf); //判断的对象是文件描述符而不是文件
	if (S_ISREG(statbuf.st_mode))   //S_ISREG 用来判断是否为普通文件的宏定义
	{
		printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	
	printf("\n-----\n");
	memset(&statbuf, 0, sizeof(statbuf));
	lstat("/home/mxc/winshare", &statbuf); //当对象是软连接时，判断的对象是软连接本身而不是软连接指向的对象
	if (S_ISLNK(statbuf.st_mode))   //S_ISLNK 用来判断是否为软连接的宏定义
	{
		printf("link file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	
	close(fd);
	exit(0)	;
}