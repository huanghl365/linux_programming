#include <stdio.h>
#include <unistd.h>
#include  <stdlib.h>

/*
程序功能描述：测试access调用，判断文件是否存在及程序对文件的权限
*/
int main(int argc, char *argv[])
{
	int ret = -1;
	if(argc < 2)
	{
		printf("Usage: ./a.out filename\n");
		exit(1);
	}
	
	//access参数1为文件名，参数2为一些flags，常用的如下：
	//F_OK测试文件是否存在，R_OK、W_OK、X_OK测试程序执行者对目标文件是否可读可写可执行
	//返回值：文件存在或者具有权限返回0，否则返回-1
	ret = access(argv[1], F_OK);
	if (ret == 0)
	{
		printf("the file exist\n");
		printf("R access: %d\n", access(argv[1], R_OK)? 0:1);
		printf("W access: %d\n", access(argv[1], W_OK)? 0:1);
		printf("X access: %d\n", access(argv[1], X_OK)? 0:1);
	}else
	{
		printf("the file not exist\n");
	}
	return 0;
	
	
}

