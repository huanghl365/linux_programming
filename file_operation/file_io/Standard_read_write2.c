#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：使用fwrite写数据到文件，fseek定位之后，再使用fread读取出来。在程序中分析了越界写入问题。
*/
int main(void)
{
	FILE *fp = NULL;
	char writebuf[50] = "GOOD MORNING TEACHER";
	char readbuf[50] = "\0";
	//char readbuf[100] = "\0";
	int read_ret,write_ret;
	
	fp = fopen("test.txt", "w+"); //更新方式，清空文件
	if (NULL == fp)
	{
		perror("fopen");
		return -1;
	}

	//使用fwirte写字符数组时，应该使用strlen指定数据大小，不要使用sizeof，因为这样有可能会写入很多空字符
	//write_ret = fwrite(writebuf, 1, strlen(writebuf), fp);
	//if (write_ret != strlen(writebuf))

	//write_ret = fwrite(writebuf, 1, sizeof(writebuf), fp);
	//if (write_ret != sizeof(writebuf))

	//如果指定的数目超过数组的大小，还是按照指定的大小输入数据，这样数组就会越界，写入的数据是不可知的
	write_ret = fwrite(writebuf, 1, sizeof(writebuf)*2, fp);
	if (write_ret != sizeof(writebuf)*2)
	{
		printf("fwrite failed\n");
		fclose(fp);
		fp = NULL;
		return -1;
	}
	else
	{
		printf("write_ret = %d\n", write_ret);
	}
	
	
	fseek(fp, 0, SEEK_SET); //偏移文件指针到文件头
	//如果fwrite故意越界写入，写入两倍数组大小的数据，那么除非readbuf大小加倍，否者read读取到数组也会越界导致发生错误
	read_ret = fread(readbuf, 1, write_ret, fp);
	if (read_ret != write_ret)
	{
		printf("fread failed\n");
		fclose(fp);
		fp = NULL;
		return -1;
	}
	else
	{
		printf("read_ret = %d\n", read_ret);
		printf("readbuf = %s\n", readbuf);
	}
	
	fclose(fp);
	fp = NULL;
	return 0;
}

