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

#if 1
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


