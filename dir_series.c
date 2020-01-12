#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUF_SIZE 1024
void printdir_test(char* dirpath, int depth);
int mkdir_test(void);

int main(void)
{
	char read_buffer[BUF_SIZE] = "\0";
	printf("enter the dir:");
	if (NULL != fgets(read_buffer, BUF_SIZE, stdin))
	{
		if (read_buffer[strlen(read_buffer) - 1] == '\n')
		{
			read_buffer[strlen(read_buffer) - 1] = '\0';  //去除fgets读取的换行符
			printf("%s", read_buffer);
		}
		
		printdir_test(read_buffer, 0); //扫描目录
	}
	return 0;
}

void printdir_test(char* dirpath, int depth)
{
	DIR *pdir = NULL;
	struct dirent *entry = NULL;
	struct stat statbuf;
	long int ret = 0;
	pdir = opendir(dirpath); //打开目录，获取目录流指针，失败返回NULL
	if (pdir == NULL)
	{
		printf("open dir failed\n");
	}
	//opendir要想打开目录，对目录路径是有要求的，有两种方式指定目录路径：1、传递绝对路径        
	//2、不使用绝对路径，但要先切换到子目录所在的父目录，再打开子目录
	
	//因为有可能有子目录，遍历子目录时，传递的目录路径不是绝对路径，因此在遍历子目录之前要先切换到当前路径
	chdir(dirpath);	
	while ((entry = readdir(pdir)) != NULL)
	{
		lstat(entry->d_name, &statbuf); 
		if (S_ISDIR(statbuf.st_mode)) //判断是否是一个目录
		{	
			if (strcmp(".", entry->d_name) && strcmp("..", entry->d_name)) 
			{
				printf("%*s%s/\n", depth, "", entry->d_name);
				printdir_test(entry->d_name, depth + 4);  //遍历子目录
			}
		
		}
		else
		{
			printf("%*s%s\n", depth, "", entry-> d_name); //打印目录下的文件
		}
		
	}
	//遍历子目录时，目录路径会向下切换，因此每次遍历完目录都要返回上级目录，否则当遍历上级的其他目录时目录路径会出错导致目录打不开
	chdir("..");
	closedir(pdir); //关闭目录流并释放与之关联的资源
}

int mkdir_test(void)
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
	fd = open("testfile", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR | S_IXUSR);
	
	//删除空目录
	rmdir("/home/mxc/testdir1");

	close(fd); 
} 



