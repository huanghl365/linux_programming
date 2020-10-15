#include <stdio.h>
#include <unistd.h>
#include  <stdlib.h>
	
/*
程序功能描述：测试access调用，判断文件是否存在、进程对文件的权限
*/

int main(int argc, char *argv[])
{
	int ret = -1;
	if(argc < 2)
	{
		printf("Usage: ./a.out filename\n");
		exit(1);
	}
	//文件是否存在
	ret = access(argv[1], F_OK);
	if (ret == 0)
	{
		//进程对文件的权限
		printf("the file exist\n");
		printf("R access: %d\n", access(argv[1], R_OK)? 0:1);
		printf("W access: %d\n", access(argv[1], W_OK)? 0:1);
		printf("X access: %d\n", access(argv[1], X_OK)? 0:1);
	}else
	{
		printf("the file not exist\n");
		exit(1);
	}
	return 0;
	
	
}

