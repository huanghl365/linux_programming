#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	struct stat statbuf;
	uid_t uid;
	gid_t gid;
	
	//只有是文件的属主，或者具有超级用户权限才能够改变文件的权限
	chmod("/home/mxc/testfile", S_IRUSR | S_IWUSR | S_IXUSR);
	chmod("/home/mxc/testfile", S_IRGRP | S_IWGRP | S_IXGRP);
	
	stat("/home/mxc/testfile", &statbuf);
	
	if (S_ISREG(statbuf.st_mode))   	//S_ISREG 用来判断是否为普通文件的宏定义
	{
		printf("regular file\n");
		if (S_IRUSR & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("own have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("own have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("own have execute\n");
		
		if (S_IRGRP & statbuf.st_mode)  //S_IRUSR 属主具有读权限的掩码
			printf("grp have read\n");
		if (S_IWGRP & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXGRP & statbuf.st_mode) 
			printf("grp have execute\n");
	}
	printf("\n-----\n");
	
	uid = getuid();	//获取当前进程执行用户的用户ID
	gid = getgid(); //获取当前进程执行用户的用户组ID
	
	//如果要修改文件的属主，那么进程需要有超级用户的权限
	//1000 是 mxc的用户ID和组ID
	chown("/home/mxc/testfile", 1000, 1000); 
	return 0;
}
