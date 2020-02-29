#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
函数原型：FILE* popen(const char* command,const char* type);
函数描述：通过fork产生一个子进程，然后子进程来执行command对应的命令，并且创建了一个管道，这个管道的作用是实现这个子进程与调用
进程之间的命令、参数以及返回值的传递（本来管道就是一种进程间通信方式）。
参数：
popen返回的I/O流的方向由参数type确定，type参数只能是w（写）和r（读）中的一个，如果 type 是 "r" 则fopen返回的文件指针
连接到 command 的标准输出；如果 type 是 "w" 则文件指针连接到 command 的标准输入。
返回值：
popen的返回值是个标准I/O流，并且这个流是单向的，须由pclose来关闭。
*/

/*
程序功能：简单测试popen调用，读取命令的执行输出
*/
int main()
{
    FILE *read_fp = NULL;
    char read_buf[32] = {0};
    char *strptr = NULL;
    char readcnt = 0;
    read_fp = popen("ls ./", "r");
    if (read_fp != NULL)
    {
	while(1)
	{
	   //strptr = fgets(read_buf, 32, read_fp);
	    memset(read_buf, 0 ,sizeof(read_buf));
	    readcnt = fread(read_buf, sizeof(char), 32-1, read_fp);
	    //if (strptr != NULL)
	    if (readcnt > 0)
	    {
		printf("%s", read_buf);
	    }
	    else
	    {
			//判断文件操作是否产生错误，有错误返回非零，否则返回0
			if (ferror(read_fp) != 0)
			{
				printf("the file operation have error\n");
			}
			break;
	    }
	}
	pclose(read_fp);
    }
    return 0;
}
