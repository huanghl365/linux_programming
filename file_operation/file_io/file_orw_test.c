#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：使用read / write读写文件，分析越界写入问题。
*/
int main(void)
{
	int fd = -1;
	int ret = 0;
	char write_buf[50] = "Append write test";
	char read_buf[50] = "\0";
	//char read_buf[100] = "\0";
	fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC); 
	if (-1 == fd)
	{
		perror("open");
		exit(1);
	}

/*
测试：write写字符数组时，分别使用sizeof和strlen指定大小
测试结果：
使用wirte写字符数组时，使用sizeof指定大小，能够写入，但是会写入很多空字符。
使用strlen指定大小则不会写入空字符，只会写入有效数据。
*/
#if 0
	ret = write(fd, write_buf, sizeof(write_buf));
	//ret = write(fd, write_buf, strlen(write_buf));
	if (-1 != ret)
	{
		printf("ret = %d\n", ret);
	}
	else
	{
		printf("write failed\n");
		close(fd);
		exit(1);
	}
	

	lseek(fd, 0, SEEK_SET);
	
	ret = read(fd, read_buf, ret); //读取写入的数据
	if (-1 != ret)
	{
		printf("ret = %d\n", ret);
		printf("read buf :%s\n", read_buf);
	}
	else
	{
		printf("read failed\n");
		close(fd);
		exit(1);
	}
		
#endif

/*
测试：write写字符数组时，写入的大小指定为两倍数组大小，即越界写入，观察现象
测试结果：
如果write故意越界写入，写入两倍数组大小的数据，能够写入不报错。
按照write写入的大小读取数据，此时read的buf不够大，那么会发生错误。
*/
#if 1

	ret = write(fd, write_buf, sizeof(write_buf) * 2); //两倍数组大小，越界写入

	if (-1 != ret)
	{
		printf("ret = %d\n", ret);
	}
	else
	{
		printf("write failed\n");
		close(fd);
		exit(1);
	}
	

	lseek(fd, 0, SEEK_SET); 
	ret = read(fd, read_buf, ret);
	if (-1 != ret)
	{
		printf("ret = %d\n", ret);
		printf("read buf :%s\n", read_buf);
	}
	else
	{
		printf("read failed\n");
		close(fd);
		exit(1);
	}
	
#endif

	close(fd);
	return 0;

}

