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

#if 1
int main(void)
{
	int fd = -1;
	int ret = 0;
	char write_buf[50] = "Append write test";
	char read_buf[50] = "\0";
	fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC);
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
	printf("ret = %d\n", ret);

	lseek(fd, 0, SEEK_SET);
	//ret = read(fd, read_buf, ret);
	ret = read(fd, read_buf, 100);

	printf("ret = %d\n",ret);
	printf("read buf :%s\n", read_buf);
	
	close(fd);
	return 0;

}
#endif


/*
程序功能描述：将同一个文件打开两次，使用append关联文件指针，做交替读写测试
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
	//测试交替写，使用O_APPEND可以关联两个文件管理表的文件指针
	fd1 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 
	fd2 = open("test.txt", O_RDWR|O_TRUNC|O_CREAT|O_APPEND, 0666); 

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
			exit(1);
		}
		sleep(1);
	}
	
		
#else

	//测试交替读，使用O_APPEND可以关联两个文件管理表的文件指针
	fd1 = open("test.txt", O_RDONLY|O_APPEND); 
	fd2 = open("test.txt", O_RDONLY|O_APPEND); 
	
	
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
		//测试各交替读实验
		//测试结果：无法实现交替读
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
			exit(1);
		}
		sleep(1);
	
		memset(buf, 0, sizeof(buf));
		ret = read(fd2, buf, 2);
		if(0 != ret)
		{
			if(0 == ret)
				exit(1);
			printf("成功读取fd2：[%s]\n", buf);
		}
		else
		{
			perror("读取字符失败");
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
程序功能描述：使用dup2复制文件描述符，做交替读写测试
*/

#if 0
int main(int argc, char *argv[])
{
	int fd1 = -1,fd2 = -1;
	int ret;
	char buf[10] = {'0'};
	
	fd1 = open("test.txt", O_RDWR | O_CREAT, 0666);
	if(-1 == fd1)
	{
		perror("文件打开失败");
		exit(1);
	}
	else
	{
		printf("文件打开成功\n");
		printf("文件描述符fd1为：%d\n", fd1);
	}
	
	
	//使用dup2复制文件描述符
	close(4);
	fd2 = dup2(fd1, 4); 
	

#if 0
	//测试能否实现交替写
	//测试结果：能够实现交替写，说明使用dup2可以实现同一文件的不同文件描述符的文件指针关联
	while(1)
	{
		ret = write(fd1, "aa", strlen("aa"));
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd1\n", strlen("aa"));
		}
		else
		{
			perror("写入字符失败");
			exit(1);
		}
		sleep(1);
	
		ret = write(fd2, "bb", strlen("bb"));
		if(-1 != ret)
		{
			printf("成功写入%lu个字符到fd2\n", strlen("bb"));
		}
		else
		{
			perror("写入字符失败");
			exit(1);
		}
		sleep(1);
	}
#else
	//测试能否实现交替读
	//测试结果：能够实现交替读，说明使用dup2可以实现同一文件的不同文件描述符的文件指针关联
	while(1)
	{
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
			exit(1);
		}
		sleep(1);
		
	}
#endif
	
	close(fd1);
	close(fd2);
	return 0;
} 
#endif

