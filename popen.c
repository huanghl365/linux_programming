#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
什么是管道：当一个进程连接数据流到另一个进程时，我们使用术语管道。
我们通常把一个进程的输出通过管道连接到另一个进程的输入。


函数原型：FILE* popen(const char* command,const char* type);
函数描述：
popen通过创建管道，fork创建子进程以及调用shell来执行command指定的程序。
管道的作用是实现这个子进程与调用进程之间的数据的传输（本来管道就是一种进程间通信方式）。
根据定义，由于管道是单向的，因此type参数只能指定读取或写入，不能同时指定两者。 生成的流相应地为只读或仅写。

参数：
command：指定需要执行的程序，因为popen内部调用shell来执行command，因此command支持使用各种shell扩展

type：popen返回的I/O流的方向由参数type确定，type参数只能是w（写）和r（读）中的一个:
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
程序功能：简单测试popen调用，读取命令的执行输出
*/

/*
程序功能描述：编译一个hello可执行程序,用来测试popen读取
*/
#if 0
int main(int argc, char *argv[])
{
	int i = 0;
	printf("coming to hello\n");
	//return 0;
	return 20;
}
#endif

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



/*
程序功能：简单测试popen调用，循环从标准输入读取数据，并通过管道写给popen执行的程序
*/
#if  0
int main(void)
{
	FILE *write_fp = NULL;
	char *pstr = NULL;
	int ret;
	char write_buf[32] = "\0";
	while(1)
	{
		//测试发现每次只有执行pclose的时候才能将数据写出，因此如果想要做循环处理，则每次都必须重新调用popen打开程序
		write_fp = popen("hexdump", "w");  
		if (NULL == write_fp)
		{
			perror("popen");
			break;
		}
		
		memset(write_buf, 0, sizeof(write_buf));
		pstr = fgets(write_buf, 32, stdin);
		//printf("pstr = %s\n",write_buf);
		if ('\n' != write_buf[0])
		{
			write_buf[strlen(write_buf)-1] = '\0'; //去除换行符
			ret = fwrite(write_buf, sizeof(char), strlen(write_buf), write_fp);
			if (ret != strlen(write_buf))
			{
				printf("fwrite failed\n");
				pclose(write_fp);
				write_fp = NULL;
				break;
			}
			else
			{
				pclose(write_fp);
				write_fp = NULL;
			}
		}
		else
		{
			printf("fgets only get \\n,so end\n");
			pclose(write_fp);
			write_fp = NULL;
			break;
		}
	}
	
	return 0;
}
#endif 
