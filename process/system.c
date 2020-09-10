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
函数原型：int system(const char *command);
描述：system库函数使用fork创建一个子进程，该子进程使用execl执行command中指定的shell命令，如下所示：
execl("/bin/sh", "sh", "-c", command, (char *) 0);
参数：
command：需要执行的命令
返回值：
如果command为NULL，那么如果存在shell程序，则返回非零值；如果没有shell程序，则返回0。

如果执行的command添加了&即后台运行，则system则不用等待command执行完成，直接返回0。

如果无法创建子进程，或者无法获取进程的退出状态，则返回值为-1。

如果无法在子进程中执行shell程序，则返回-127。

system需要等待command执行完毕才会返回，如果调用成功，则返回shell程序的
退出状态(shell程序的终止状态是它执行的最后一个命令的退出状态status)。
*/


/*
程序功能描述：测试system函数的使用，system的内部实现逻辑可以参照 exec_series.c中的例子
*/
#if 1
int main(int argc, char *argv[])
{

	int status;	
	status = system("ps");
	//status = system("./ggg");
	//status = system("./hello");
	printf("status = %d done\n", status);
	
	//-1:system内部子进程创建失败或者内部wait回收失败
	//127:命令无法执行
	if (-1 != status && -127 != status)	
	{
		printf("进程是否正常终止：%d\n", WIFEXITED(status));    
		printf("进程终止退出码：%d\n", WEXITSTATUS(status));
	}
	return 0;
	 
}
#endif  


/*
程序功能描述：编译一个hello可执行程序,用来测试popen读取
*/
#if 0 
int main(int argc, char *argv[])
{
	int i = 0;
	printf("coming to hello\n");
	return 0;
	//return 20;
}
#endif

