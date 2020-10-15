#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：分别使用stat、fstat、lstat调用获取文件属性，分析三者的区别

测试结果：
1、stat、lstat参数1是文件路径名，fstat参数1是文件描述符
2、当对象是软连接时，stat、fstat判断的对象时软连接指向的对象，lstat判断的对象是软连接本身
*/

int main(int argc, char *argv[])
{
	struct stat statbuf;
	int ret = -1;

	if(argc < 2)
	{
		printf("Usage: ./a.out filename\n");
		exit(1);
	}
	
	int fd = open(argv[1], O_RDWR);


#if 1
	ret = stat(argv[1], &statbuf);

	if (0 == ret)
	{
		if (S_ISREG(statbuf.st_mode))	
			printf("regular file\n");

		if (S_ISDIR(statbuf.st_mode))	
			printf("dir file\n");


		if (S_IRUSR & statbuf.st_mode)	
			printf("owner have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("owner have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("owner have execute\n");

		if (S_IRGRP & statbuf.st_mode) 
			printf("grp have read\n");
		if (S_IWGRP & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXGRP & statbuf.st_mode) 
			printf("grp have execute\n");

		if (S_IROTH & statbuf.st_mode)  
			printf("grp have read\n");
		if (S_IWOTH & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXOTH & statbuf.st_mode) 
			printf("grp have execute\n");

		printf("st_ino: %d\n", statbuf.st_ino); 	
		printf("st_nlink: %d\n", statbuf.st_nlink); 
		printf("st_size: %d\n", statbuf.st_size);	 
		printf("st_uid: %d\n", statbuf.st_uid); 	
		printf("st_gid: %d\n", statbuf.st_gid); 	
		printf("st_blksize: %d\n", statbuf.st_blksize);
		printf("st_blocks: %d\n", statbuf.st_blocks);	
		printf("st_atime: %d\n", statbuf.st_atime); 	 
		printf("st_mtime: %d\n", statbuf.st_mtime); 	
		printf("st_ctime: %d\n", statbuf.st_ctime); 	 
	}

#endif

#if 1
	memset(&statbuf, 0, sizeof(statbuf));
	ret = fstat(fd, &statbuf); 			

	if (0 == ret)
	{
		if (S_ISREG(statbuf.st_mode))	
			printf("regular file\n");
		
		if (S_IRUSR & statbuf.st_mode)	
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	else
		{
		perror("fstat");
		exit(1);
	}
#endif

#if 1
	memset(&statbuf, 0, sizeof(statbuf));
	ret = lstat(argv[1], &statbuf); 	
	if (0 == ret)
	{
		if (S_ISLNK(statbuf.st_mode))  
			printf("link file\n");
		
		if (S_IRUSR & statbuf.st_mode) 
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
#endif

	close(fd);
	return 0;
}
