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


/*
程序功能描述：测试open调用中的 O_SYNC  的使用
*/
#if 0
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
			close(out);
			exit(1);
		}
		sleep(5);
	}

	close(out);
	return 0;
	
}
#endif 

/*
程序功能描述：测试open调用中的O_APPEND 的使用
*/
#if 0
int main(void)
{
	unsigned char readbuf[1024] = {'\0'};
	int out, ret;
	char writebuf[1024] = "THE WORD I WANT TO SAY : HELLO WORLD";

	//打开文件进行读写，写的时候为续写
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

	//当open中使用了O_APPEND选项的时候，如果先对文件写，那么写完后文件指针会移动到尾部，再去读的时候需要先用lseek进行定位
	//当open中使用了O_APPEND选项的时候，如果先对文件读，那么文件指针默认从文件头开始，此时并不需要使用lseek定位到文件头
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
#endif


/*
程序功能描述：将同一个文件打开两次，使用O_APPEND关联文件指针，测试能否实现交替读写
*/

#if 0
int main(int argc, char *argv[])
{
	int fd1 = -1; 
	int fd2 = -1;
	int ret = -1;
	char buf[100] = {'\0'};
	
#if 0

	//将一个文件打开两次，推测内存中只有一份
	//使用O_APPEND可以关联两个文件管理表的文件指针，测试能否实现交替写
	fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	
		//fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); 
		//fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0666); 

	if (-1 == fd1||-1 == fd2)							  						
	{										 							
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("打开文件成功!\n");
	}
	
	
	
	while(1)
	{
		//测试交替写实验
		//测试结果：证明是交替写
		ret = write(fd1, "aa",2);
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd1\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		ret = write(fd2, "bb", 2);
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd2\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
	
		
#else

	//使用O_APPEND可以关联两个文件管理表的文件指针，测试能否实现交替读
	fd1 = open("test.txt", O_RDWR|O_APPEND); 
	fd2 = open("test.txt", O_RDWR|O_APPEND); 
	
	
	if (-1 == fd1||-1 == fd2)							  						
	{										 							
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("打开文件成功!\n");
	}
	
	while(1)
	{
		//测试交替读实验
		//测试结果：O_APPEND无法实现交替读，这里也说明O_APPEND只是用来做接续写操作的
		memset(buf, 0, sizeof(buf));
		ret = read(fd1, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd1：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd1);
			exit(1);
		}
		sleep(1);
	
		memset(buf, 0, sizeof(buf));
		ret = read(fd2, buf, 2);
		if(-1 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd2：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
			close(fd2);
			exit(1);
		}
		sleep(1);
	}
	
#endif
	
	//关闭文件
	ret = close(fd1);
	if(-1 != ret)
		printf("成功关闭文件1!\n");
	
	ret = close(fd2);
	if(-1 != ret)
		printf("成功关闭文件2!\n");
	
	return 0;
	
}
#endif




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
#if 0
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










