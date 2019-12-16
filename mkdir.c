#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE 1024
int main(void)
{
	int fd,ret;
	char buffer[BUF_SIZE] = "\0";
	//创建一个目录，目录带有rwx权限，这样才能查看目录以及进入目录创建文件
	//跟opcn一样，mkdir创建文件夹指定的权限需要与执行用户的umask反值做与运算
	//如果目录存在，那么创建目录失败
	ret = mkdir("/home/mxc/testdir", S_IRUSR | S_IWUSR | S_IXUSR);
	if (ret != 0)
	{
		printf("creat dir failed\n");
	}
	mkdir("/home/mxc/testdir1", S_IRUSR | S_IWUSR | S_IXUSR);
	
	//切换目录
	chdir("/home/mxc/testdir");
	
	
	//获取当前目录到缓冲区
	//返回值指向缓冲区，如果路径超过指定大小，返回空指针
	if (getcwd(buffer, BUF_SIZE) != NULL)
	{
		printf("%s\n", buffer);
	}
	
	//在目录中创建文件
	fd = open("testfile", O_CREAT|O_TRUNC|O_EXCL, S_IRUSR | S_IWUSR | S_IXUSR);
	
	//删除空目录
	rmdir("/home/mxc/testdir1");

	close(fd); 
} 