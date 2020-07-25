#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
程序功能：简单测试popen调用，循环从标准输入读取数据，并通过管道写给popen执行的程序处理
*/


/*疑问：测试发现每次只有执行pclose的时候hexdump才会处理我们写出的数据，
因此如果想要做循环处理，则每次都必须重新调用popen打开程序,为什么必须先执行pclose
hexdump才会进行处理呢？

原因推测：hexdump应该是等读取完数据才会进行处理，而当执行pclose的时候，会将文件流关闭，这时
管道的写端会关闭，因此hexdump 中的read调用返回0表示读取结束，hexdump也就开始处理了。
*/

int main(void)
{
	FILE *write_fp = NULL;
	char *pstr = NULL;
	int ret;
	char write_buf[32] = "\0";
	while(1)
	{
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

