#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
程序功能描述：使用write写数据到空文件，lseek定位后使用read读取出来。在程序中分析了越界写入问题。
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

