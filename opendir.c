#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUF_SIZE 1024

void printdir(char* dirpath, int depth);
void printdir(char* dirpath, int depth)
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
	//因为有可能有子目录，遍历子目录时，传递的目录路径不是绝对路径，因此在遍历子目录之前要先切换到当前路径，否则打不开目录
	chdir(dirpath);	
	while ((entry = readdir(pdir)) != NULL)
	{
		lstat(entry->d_name, &statbuf); 
		if (S_ISDIR(statbuf.st_mode)) //判断是否是一个目录
		{	
			if (strcmp(".", entry->d_name) && strcmp("..", entry->d_name)) 
			{
				printf("%*s%s/\n", depth, "", entry->d_name);
				printdir(entry->d_name, depth + 4);  //遍历子目录
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
		
		printdir(read_buffer, 0); //扫描目录
	}
	return 0;
}


