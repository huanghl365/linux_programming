#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：使用read write操作 标准输入输出的文件描述符
*/

#if 0
int main(void)
{
	int nread = 0;
	int nwrite = 0;
	char readbuf[128];
	//0 表示标准输入的文件描述符   1表示标准输出的文件描述符
	//stdin 表示标准输入的文件指针	stdout表示标准输出的文件指针
	nread = read(0, readbuf, sizeof(readbuf));
	if (nread != -1 )
	{
	nwrite = write(1, readbuf, nread);
	}
	return 0;
}
#endif 


/*
程序功能描述：使用write写数据到空文件，并使用lseek定位读取出来
*/
#if 0
int main(void)
{
	int fd = -1;
	int ret = 0;
	char write_buf[50] = "Append write test";
	char read_buf[50] = "\0";
	fd = open("test.txt", O_RDWR | O_CREAT | O_APPEND);
	if (-1 == fd)
	{
		perror("open");
		exit(1);
	}

	lseek(fd, 0, SEEK_END);

	//write是指定写入多大就写入多大，如果指定的范围超过数组的大小，还是按照指定的大小输入数据，这样数组就会越界，写入的数据是不可知的
	//read并不是指定读取多少就一定能读取多少，在不超过指定的大小的情况下，能读取多少就读取多少，跟文件的数据量大小有关
	
	//使用wirte写数据时，应该使用strlen指定数据大小，不要使用sizeof，因为这样有可能会写入很多空字符
	//ret = write(fd, write_buf, sizeof(write_buf));
	//ret = write(fd, write_buf, strlen(write_buf));
	ret = write(fd, write_buf, 100);

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
	//ret = read(fd, read_buf, ret);
	ret = read(fd, read_buf, 100);

	if (-1 != ret)
	{
		printf("ret = %d\n", ret);
	}
	else
	{
		printf("read failed\n");
		close(fd);
		exit(1);
	}
	
	printf("ret = %d\n",ret);
	printf("read buf :%s\n", read_buf);
	
	close(fd);
	return 0;

}
#endif









