#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：测试open调用中的 O_SYNC  的使用
*/
int main(int argc, char *argv[])
{
	int out = -1;
	int ret = -1;
	char buf[100] = {'\0'};
	char writebuf[50] = "the rest\n";
	 
	/*
	测试：添加了O_SYNC后，不用close，写入到文件的数据立即同步到磁盘
	测试结果：用watch -n 1 cat命令查看文件，可以发现文件内容不断增加，write写入的数据立即同步到磁盘
	*/
	out = open("test.txt", O_WRONLY | O_CREAT | O_SYNC |O_NONBLOCK); 		  
	if (out < 0)
	{
		perror("open");
		exit(1);
	}

	while(1)
	{
		ret = write(out, writebuf, strlen(writebuf));
		if(-1 != ret)
		{
			printf("write success:%s\n", writebuf);
		}
		else
		{
			printf("write failed\n");
			close(out);
			exit(1);
		}
		sleep(5);
	}

	close(out);
	return 0;
	
}


