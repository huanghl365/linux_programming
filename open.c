#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
int open(const char *pathname, int flags, mode_t mode);
描述：用于打开文件并获取文件描述符
参数：
pathname:文件的路径名

flags：以指定方式操作文件
O_CREAT 创建文件，文件存在则不创建，但不报错
O_RDWR：读写 O_WRONLY：只写 O_RDONLY：只读
O_TRUNC  清空文件
O_APPEND 续写文件
O_EXCL   如果文件存在则报错
O_SYNC   同步文件到存储设备
O_NONBLOCK  在读取不到数据或是写入缓冲区已满会马上return，而不会阻塞等待。

mode ：指定创建文件的权限， S_IRUSR 表示读 S_IWUSR 表示写，S_IXUSR表示执行，
指定的权限还要与linux系统的umask 反值（反值不包括执行权限） 取与

返回值：返回获取的文件描述符，失败返回-1并设置errno

*/


/*
程序功能描述：测试open调用 各种flags的使用效果
*/
#if 0
int main(void)
{
	unsigned char ch[1024] = {'\0'};
	int in, out;
	int nread = 0;
	//只读方式打开文件
	in = open("/home/mxc/test.txt", O_RDONLY);

	//创建文件，文件存在不创建，默认不清空文件也不接续文件，从头开始写文件
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	
	//创建文件，文件存在不创建并清空文件
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	//创建文件，文件存在不创建并接续操作文件
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

	//创建文件，文件存在则报错，返回-1
	out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

	out = open("/home/mxc/file.out")
	if (out < 0)
	{
		perror("open");
		exit(1);
	}
	
	//读取文件内容写到另一个文件
	//read属于系统调用，每次调用都会有用户到内核空间的切换开销，因此可以通过加大每次读取字节，减小切换次数减小开销。
	while ((nread = read(in, ch, 1024)) > 0 )
	{
		
		write(out, ch, nread);
	}

	close(in);
	close(out);
	return 0;
}
#endif


int main(int argc, char *argv[])
{
	int out = -1;
	int ret = -1;
	char buf[100] = {'\0'};
	char writebuf[50] = "the rest\n";
	 
	//创建文件，文件存在则不创建，不阻塞，写入数据立即同步到磁盘文件
	//可以一边写数据，一边用cat命令查看磁盘文件
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
			exit(1);
		}
		sleep(5);
	}

	close(out);
	return 0;
	
}





