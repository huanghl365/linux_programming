#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：简单测试chmod调用，修改文件权限
*/
int main(int argc, char *argv[])
{
	struct stat statbuf;
	uid_t uid;
	gid_t gid;
	int ret = -1;

	if(argc < 2)
	{
		printf("Usage: ./a.out filename\n");
		exit(1);
	}
	
	//只有是文件的属主，或者具有超级用户权限才能够改变文件的权限
	ret = chmod(argv[1], S_IRUSR | S_IWUSR | S_IXUSR);
	
	//ret = chmod("/home/mxc/file", S_IRGRP | S_IWGRP | S_IXGRP);
	if (-1 == ret)
	{
		perror("chmod");
	}
	
	ret = stat(argv[1], &statbuf);

	if (0 == ret)
	{
		if (S_ISREG(statbuf.st_mode))   	//S_ISREG 用来判断是否为普通文件的宏定义
			printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  	//S_IRUSR 属主具有读权限的掩码
			printf("own have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("own have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("own have execute\n");
		
		if (S_IRGRP & statbuf.st_mode)  	//S_IRGRP 用户组具有读权限的掩码
			printf("grp have read\n");
		if (S_IWGRP & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXGRP & statbuf.st_mode) 
			printf("grp have execute\n");
	}
	return 0;
}
