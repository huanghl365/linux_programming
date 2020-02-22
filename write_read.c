#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
函数原型：ssize_t read(int fd, void *buf, size_t count);
描述：read用来将文件描述符fd中读取count个字符取到缓冲区buf中
参数：
fd：文件描述符
buf：用来存储读取字节的缓冲区
count：从文件中读取的字节总数
返回值：
返回读取的字节数，并且文件当前的位置会偏移返回的字节数大小。
如果读取的字节数小于指定的字节总数count，并不一定代表错误（可能到达文件尾等原因造成）。
如果读取错误，则返回-1，并设置对应的errno.


函数原型：ssize_t write(int fd, const void *buf, size_t count);
描述：write用来将缓冲区buf的count个字节写入到文件描述符fd指定的文件中。
如果使用O_APPEND打开文件，则在写入之前会先将文件偏移到文件的末尾。
参数：
fd：文件描述符
buf：用来写入文件的字符缓冲区
count：指定要写入到文件的字节总数
返回值：
返回写入的字节数，并且文件当前的位置会偏移实际写入的字节数目的大小。
如果写入的字节数小于指定的字节总数count，并不一定代表错误（可能是因为磁盘空间不够造成）。
如果写入错误，则返回-1，并设置对应的errno。

注意：写入数据的时候，需要写入多少数据就指定多大的数目，如果指定的count大小超过数组缓冲区的大小，
还是按照指定的大小输入数据，这样数组就会越界，写入的数据是不可知的。
*/

/*
程序功能描述：使用read和write操作 标准输入输出的文件描述符
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
程序功能描述：使用write写数据到空文件，lseek定位后使用read读取出来。在程序中分析了越界写入问题。
*/
#if 1
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

	lseek(fd, 0, SEEK_END);

	//使用wirte写字符数组时，应该使用strlen指定数据大小，不要使用sizeof，因为这样有可能会写入很多空字符
	//ret = write(fd, write_buf, sizeof(write_buf));
	//ret = write(fd, write_buf, strlen(write_buf));

	
	//如果指定的数目超过数组的大小，还是按照指定的大小输入数据，这样数组就会越界，写入的数据是不可知的
	ret = write(fd, write_buf, sizeof(write_buf) * 2); //故意越界

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
	

	lseek(fd, 0, SEEK_SET); //定位文件位置到开头
	//如果write故意越界写入，写入两倍数组大小的数据，那么除非read数组大小加倍，否者read读取到数组也会越界导致发生错误
	ret = read(fd, read_buf, ret);
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









