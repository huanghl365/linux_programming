#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：简单测试chown调用，修改文件属主
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


	uid = getuid();	
	gid = getgid(); 
	printf("the execute user id of the process: uid %d  gid %d\n", uid, gid);

	
	ret = stat(argv[1], &statbuf);
	if (0 == ret)
	{
		printf("st_uid: %d\n", statbuf.st_uid); 	
		printf("st_gid: %d\n", statbuf.st_gid); 	
	}
	
	//如果要修改文件的属主，那么进程需要有超级用户的权限
	//1000 是 mxc的用户ID和组ID
	ret = chown("/home/mxc/test.txt", 1000, 1000); 
	if (-1 == ret)
	{
		perror("chown");
		exit(1);
	}

	memset(&statbuf, 0, sizeof(statbuf));
	ret = stat(argv[1], &statbuf);
	if (0 == ret)
	{
		printf("st_uid: %d\n", statbuf.st_uid); 	
		printf("st_gid: %d\n", statbuf.st_gid); 	
	}
	
	return 0;
	
}

	

