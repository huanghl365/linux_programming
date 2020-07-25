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
O_RDWR：读写 
O_WRONLY：只写 
O_RDONLY：只读
O_TRUNC  清空文件
O_APPEND 续写文件
O_EXCL   如果文件存在则报错
O_SYNC   同步文件到存储设备
O_NONBLOCK  在读取不到数据或是写入缓冲区已满会马上return，而不会阻塞等待。

mode ：指定创建文件的权限， S_IRUSR 表示读 S_IWUSR 表示写，S_IXUSR表示执行，
创建的文件的权限为mode＆~umask，~mask不包括执行权限。
返回值：返回获取的文件描述符，失败返回-1并设置errno



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
程序功能描述：测试open调用 O_RDONLY O_WRONLY O_TRUNC O_CREAT O_EXCL 这几个flag的使用
*/
int main(void)
{
	unsigned char ch[1024] = {'\0'};
	int in, out;
	int nread = 0;
	//只读方式打开文件
	in = open("./test.txt", O_RDONLY);
	if (in  < 0)
	{
		perror("open");
		exit(1);
	}


	//创建只写文件，文件存在则不创建
	//out = open("./file.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	
	//创建只写文件，文件存在则不创建，并清空文件
	out = open("./file.out", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	
	//创建只写文件，文件存在则报错
	//out = open("./file.out", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (out < 0)
	{
		perror("open");
		exit(1);
	}
	
	while ((nread = read(in, ch, 1024)) > 0 )
	{
		write(out, ch, nread);
	}

	close(in);
	close(out);
	return 0;
}
