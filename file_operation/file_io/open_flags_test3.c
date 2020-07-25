#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：测试open调用中的O_APPEND 的使用
*/
int main(void)
{
	unsigned char readbuf[1024] = {'\0'};
	int out, ret;
	char writebuf[1024] = "THE WORD I WANT TO SAY : HELLO WORLD";

	/*
	测试：打开文件进行读写，O_APPEND表示续写文件
	测试结果：
	写数据的时候，文件指针会移动文件末尾，从末尾接续写入数据。
	打开文件后如果先对文件写，那么写完后文件指针会移动到尾部，再去读的时候需要先用lseek进行定位。
	打开文件后如果先对文件读，那么文件指针默认从文件头开始，此时并不需要使用lseek定位到文件头。
	*/
	out = open("test.txt", O_RDWR | O_APPEND);
	if (out < 0)
	{
		perror("open");
		exit(1);
	}
	ret = write(out, writebuf, strlen(writebuf));
	if (-1 == ret)
	{
		perror("write");
		close(out);
		exit(1);
	}

	lseek(out, 0, SEEK_SET);
	
	ret = read(out, readbuf, 10);
	if (-1 != ret)
	{
		printf("读取的字符串: %s\n", readbuf);
	}
	else
	{
		perror("read");
		close(out);
		exit(1);
	}

	
	close(out);
	return 0;
}

