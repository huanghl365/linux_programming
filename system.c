#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
程序功能描述：测试system函数的使用，system的内部实现逻辑可以参照 exec_series.c中的例子
*/

/*
函数原型：int system(const char *command);
描述：system库函数使用fork创建一个子进程，该子进程使用execl执行command中指定的shell命令，如下所示：
execl("/bin/sh", "sh", "-c", command, (char *) 0);
参数：
command：需要执行的命令
返回值：
system需要等待command执行完毕才会返回，如果调用成功，则返回shell程序的
终止状态(shell程序的终止状态是它执行的最后一个命令的终止状态status)。

如果执行的command添加了&即后台运行或者command为NULL，则system则不用等待command执行完成，直接返回0。

如果内部子进程创建失败，则返回-1。
*/
int main(int argc, char *argv[])
{

	int status;	
	status = system("ps");
	printf("status = %d done\n", status);
	if (-1 != status)	//system内部子进程创建失败，宏定义判断没有意义
	{
		printf("进程是否正常终止：%d\n", WIFEXITED(status));    
		printf("进程终止退出码：%d\n", WEXITSTATUS(status));
	}
	return 0;
	 
}

