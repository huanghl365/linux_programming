#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
函数原型：FILE* popen(const char* command,const char* type);
函数描述：通过fork产生一个子进程，然后子进程来执行command对应的命令，并且创建了一个管道，这个管道的作用是实现这个子进程与调用
进程之间的命令、参数以及返回值的传递（本来管道就是一种进程间通信方式）。
参数：
popen返回的I/O流的方向由参数type确定，type参数只能是w（写）和r（读）中的一个:
如果 type 是 "r" 则fopen返回的文件指针连接到 command 的标准输出；
如果 type 是 "w" 则文件指针连接到 command 的标准输入。
返回值：
popen的返回值是个标准I/O流，并且这个流是单向的，须由pclose来关闭。
*/

/*
程序功能：简单测试popen调用，读取命令的执行输出
*/
#if 0
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
		read_fp = NULL;
    }
    return 0;
}
#endif



/*
程序功能：简单测试popen调用，循环从标准输入读取数据，并通过管道写给popen执行的程序
*/
#if  1
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
