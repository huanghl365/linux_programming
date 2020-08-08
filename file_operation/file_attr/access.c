#include <stdio.h>
#include <unistd.h>
#include  <stdlib.h>



/*
函数原型：int access(const char *pathname, int mode);
功能描述：用来查询进程对文件的访问权限、查询文件是否存在等

参数：
pathname  表示文件的路径名
mode	  用来指定一些flags，常用的如下：
F_OK				测试文件是否存在
R_OK、W_OK、X_OK		测试程序执行者对目标文件是否可读可写可执行

返回值：
文件存在或者具有权限返回0，否则返回-1
*/
	
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

