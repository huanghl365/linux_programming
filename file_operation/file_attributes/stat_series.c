#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
函数原型：
int stat(const char *pathname, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *pathname, struct stat *buf);
功能描述：
用来查询的文件的各种信息，三者的区别如下：
1、stat、lstat参数1是文件路径名，fstat参数1是文件描述符
2、当对象是软连接时，stat、fstat判断的对象时软连接指向的对象，lstat判断的对象是软连接本身

参数：
pathname	文件的路径名
fd			文件的描述符
buf			输出参数，用来保存文件的查询信息
struct stat {
               dev_t     st_dev;         //ID of device containing file 
               ino_t     st_ino;         //inode number 
               mode_t    st_mode;        // protection 
               nlink_t   st_nlink;       // number of hard links 
               uid_t     st_uid;         // user ID of owner 
               gid_t     st_gid;         // group ID of owner 
               dev_t     st_rdev;        // device ID (if special file) 
               off_t     st_size;        // total size, in bytes 
               blksize_t st_blksize;     // blocksize for filesystem I/O 
               blkcnt_t  st_blocks;      // number of 512B blocks allocated 
	struct timespec st_atim;  // time of last access 
	struct timespec st_mtim;  // time of last modification 
	struct timespec st_ctim;  // time of last status change 

#define st_atime st_atim.tv_sec 	 // Backward compatibility 
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
}

返回值：
成功返回0 失败返回-1

---------------------
一些判断宏定义：
S_ISREG	用来判断文件是否为普通文件
S_ISDIR	用来判断文件是否为目录
S_ISLNK 用来判断文件是否为软连接


一些判断掩码：
S_IRUSR	属主是否具有读权限
S_IWUSR
S_IXUSR

S_IRGRP 用户组是否具有读权限
S_IWGRP
S_IXGRP

S_IROTH	其他用户是否具有读权限
S_IWOTH
S_IXOTH
*/


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
