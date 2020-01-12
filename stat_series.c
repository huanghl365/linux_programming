#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	
	ret = stat(argv[1], &statbuf);

	if (0 == ret)
	{
		//查看文件类型
		if (S_ISREG(statbuf.st_mode))	//S_ISREG 用来判断是否为普通文件的宏定义
			printf("regular file\n");

		if (S_ISDIR(statbuf.st_mode))	//S_ISREG 用来判断是否为目录文件的宏定义
			printf("dir file\n");


		//查看文件权限
		if (S_IRUSR & statbuf.st_mode)	//S_IRUSR 属主读权限的掩码
			printf("owner have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("owner have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("owner have execute\n");

		if (S_IRGRP & statbuf.st_mode)  //S_IRGRP 用户组具有读权限的掩码
			printf("grp have read\n");
		if (S_IWGRP & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXGRP & statbuf.st_mode) 
			printf("grp have execute\n");

		if (S_IROTH & statbuf.st_mode)  //S_IROTH 其他用户具有读权限的掩码
			printf("grp have read\n");
		if (S_IWOTH & statbuf.st_mode) 
			printf("grp have write\n");
		if (S_IXOTH & statbuf.st_mode) 
			printf("grp have execute\n");

		//查看文件其他属性
		printf("st_ino: %d\n", statbuf.st_ino); 	//文件的inode
		printf("st_nlink: %d\n", statbuf.st_nlink); //文件的硬链接数目
		printf("st_size: %d\n", statbuf.st_size);	//文件的大小，以byte为单位 
		printf("st_uid: %d\n", statbuf.st_uid); 	//文件的uid
		printf("st_gid: %d\n", statbuf.st_gid); 	//文件的gid
		printf("st_blksize: %d\n", statbuf.st_blksize);  //文件的块大小
		printf("st_blocks: %d\n", statbuf.st_blocks);	 //文件的块数目
		printf("st_atime: %d\n", statbuf.st_atime); 	 //文件的访问时间
		printf("st_mtime: %d\n", statbuf.st_mtime); 	 //文件的修改时间
		printf("st_ctime: %d\n", statbuf.st_ctime); 	 //文件的改变时间
	}


	printf("\n--------------------------------------------------------------------------\n");
	memset(&statbuf, 0, sizeof(statbuf));
	ret = fstat(fd, &statbuf); //判断的对象是文件描述符而不是文件

	if (0 == ret)
	{
		if (S_ISREG(statbuf.st_mode))	//S_ISREG 用来判断是否为普通文件的宏定义
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
	}
	
	printf("\n--------------------------------------------------------------------------\n");
	memset(&statbuf, 0, sizeof(statbuf));
	ret = lstat(argv[1], &statbuf); 	//当对象是软连接时，判断的对象是软连接本身而不是软连接指向的对象
	if (0 == ret)
	{
		if (S_ISLNK(statbuf.st_mode))   //S_ISLNK 用来判断是否为软连接的宏定义
			printf("link file\n");
		
		if (S_IRUSR & statbuf.st_mode) 
			printf("have read\n");
		if (S_IWUSR & statbuf.st_mode) 
			printf("have write\n");
		if (S_IXUSR & statbuf.st_mode) 
			printf("have execute\n");
	}
	
	close(fd);
	return 0;
}
