#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
程序功能描述：使用fread / fwrite读写文件，分析越界写入问题
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

/*
测试：fwrite写字符数组时，分别使用sizeof和strlen指定大小
测试结果：
使用fwirte写字符数组时，使用sizeof指定大小，能够写入，但是会写入很多空字符。
使用strlen指定大小则不会写入空字符，只会写入有效数据。
*/
#if 0
	//write_ret = fwrite(writebuf, 1, strlen(writebuf), fp);
	//write_ret = fwrite(writebuf, 1, sizeof(writebuf), fp);
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

	fseek(fp, 0, SEEK_SET); 
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
#endif

/*
测试：fwrite写字符数组时，写入的大小指定为两倍数组大小，即越界写入，观察现象
测试结果：
如果fwrite故意越界写入，写入两倍数组大小的数据，能够写入不报错。
按照fwrite写入的大小读取数据，此时fread的buf不够大，那么会发生错误。
*/

#if 1
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
	
	
	fseek(fp, 0, SEEK_SET); 
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
#endif
	fclose(fp);
	fp = NULL;
	return 0;
}

