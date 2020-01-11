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
	//使用open创建一个文件
	open("/home/mxc/testfile", O_CREAT|O_TRUNC|O_SYNC, S_IRUSR | S_IWUSR | S_IXUSR);
	
	//创建硬链接，成功返回0，失败返回-1
	link("/home/mxc/testfile","/home/mxc/hardlink");
	link("/home/mxc/testfile","/home/mxc/hardlink2");
	
	//创建一个软链接
	symlink("/home/mxc/testfile","/home/mxc/softlink");
	
	//删除文件
	unlink("/home/mxc/hardlink");
	return 0;
}
