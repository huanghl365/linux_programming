#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
函数原型：FILE* popen(const char* command,const char* type);
函数描述：
popen内部通过创建管道，fork创建子进程以及调用shell来执行command指定的程序。
根据定义，由于管道是单向的，因此type参数只能为只读或者只写，返回的流相应地为只读或仅写。

参数：
command：指定需要执行的程序，因为popen内部调用shell来执行command，因此command支持使用各种shell扩展

type：popen返回的I/O流的方向由参数type确定，type参数只能是w（写）和r（读）中的一个

	如果 type 是 "r" 则fopen返回的文件指针连接到 command 的标准输出；
	如果 type 是 "w" 则文件指针连接到 command 的标准输入。
	
返回值：
popen的返回值是个标准I/O流，并且这个流是单向的，须由pclose来关闭，不能使用fclose。


函数原型：int pclose(FILE *stream);
描述：pclose 函数等待关联的进程终止，并返回由wait4返回的命令的退出状态。
参数：
stream：popen返回的文件流指针
返回值：
返回由wait4返回的命令的退出状态。如果wait4返回错误或检测到其他错误，则pclose将返回-1，并设置errno。
*/


/*
分析popen与system的异同点：
相同点：
（1）都是内部fork创建子进程
（2）都是在子进程中调用shell执行程序，system使用execl执行shell，popen有没有使用execl不清楚。
（3）因为内部都调用了shell，因此都支持shell扩展。
不同点：
（1）popen内部创建了个管道，并且推测应该是使用了fdopen打开管道的文件描述符，并返回文件流指针。
（2）system阻塞等待程序执行结束并获取进程的退出状态；而popen不会阻塞，而是使用pclose等待进程结束获取进程退出状态。
*/


/*
程序功能描述：简单测试popen调用，读取命令的执行输出
*/
#if 1
int main(void)
{
	FILE *read_fp = NULL;
	char read_buf[32] = {0};
	char *strptr = NULL;
	char readcnt = 0;
	int status;
	//popen内部调用shell来执行command，因此command支持使用各种shell扩展
	//read_fp = popen("cat s* |grep \"printf\"", "r");
	read_fp = popen("./hello", "r");
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
					exit(1);
				}
				break;
			}
		}
		status = pclose(read_fp);
		printf("status = %d\n", status);
		if (-1 != status)
		{
			printf("子进程是否正常终止：%d\n", WIFEXITED(status));	
			printf("子进程是否非正常终止：%d\n", WIFSIGNALED(status));	
			printf("子进程终止退出码：%d\n", WEXITSTATUS(status));
		}
		read_fp = NULL;
	}
	return 0;
}
#endif 


//程序功能描述：编译一个hello可执行程序用来输出数据，用来测试popen
#if 0
int main(int argc, char *argv[]) 
{
	int i = 0;
	printf("coming to hello: argc=%d\n", argc);
	
	return 0;
	//return 20;
}
#endif 




