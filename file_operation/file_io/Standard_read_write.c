#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
函数原型：FILE *fopen(const char *path, const char *mode);
描述：打开名称为path指向的字符串的文件，并将流与其关联。
参数：
path：指向文件的路径
mode：
r rb 以只读方式打开文件，文件必须存在
w wb 只写方式打开文件，并把文件截短为零，文件不存在则创建
a ab 以写方式打开文件，新内容追加文件尾，文件不存在则创建

r+ rb+ 以更新方式打开（读和写），文件必须存在
w+ wb+ 以更新方式打开文件，并把文件长度截短为零，文件不存在则创建
a+ ab+ 以更新方式打开，新内容追加文件尾，文件不存在则创建
返回值：
调用成功则返回文件指针，失败则返回NULL，并设置errno
*/

/*
函数原型：size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
描述：函数fread从stream指向的流中读取nmemb个长度的size的单元，并将它们存储在ptr给定的位置。
参数：
ptr:指向用来存储读取单元的buf
size:读取的单元的大小
nmemb:读取单元的数目
stream：指向文件流指针
返回值：
返回读取的单元的数目，当size等于1，则表示读取的字节数目。
如果发生或者错误或者到达文件尾部，则返回值可能小于指定值，或者等于0，fread
无法区分这两种情况，需要用feof和ferror来进行判断。
*/

/*
函数原型：size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream)
描述：函数fwrite从ptr给定的位置获取它们获取nmemb个长度的size的单元，并写入到stream指向的流中
参数：
ptr:指向用来写入到文件的数据buf
size:写入单元的大小
nmemb:写入单元的数目
stream：指向文件流指针
返回值：
返回写入的单元的数目，当size等于1，则表示写入的字节数目。
一般用nmemb大小与返回值做比较，如果不相等就是出错。

注意：写入数据的时候，需要写入多少数据就指定多大的数目，如果指定的count大小超过数组缓冲区的大小，
还是按照指定的大小输入数据，这样数组就会越界，写入的数据是不可知的。
*/


/*
程序功能说明：测试fopen调用，读取一个文件，写入另一个文件
*/

#if 0
#define READ_NUM  1024

int main(void)
{
	FILE *fp1, *fp2;  //指针前面一定加上*
	int read_ret,write_ret;
	char read_buffer[READ_NUM] = "\0";
	int read_cnt , write_cnt;
	fp1 = fopen("test1.txt", "a+");   			 //更新方式打开，接续写
	//fp1 = fopen("test1.txt", "w+");   		 //更新方式，清空文件
	fp2 = fopen("test2.txt", "r");   			 //读方式打开, 文件必须存在

	if (fp1 == NULL || fp2 == NULL)
	{
		perror("fopen");
		fp1 = NULL;
		fp2 = NULL;
		return -1;
	}

	while(1)
	{
		read_ret = fread(read_buffer, 1, READ_NUM, fp2);  //其实读一行最好还是用fgets,如果一行不超过READ_NUM,那么这里可以用fread
		printf("read_ret = %d\n", read_ret);
		if (read_ret == 0) //返回0，可能达到文件尾或者发生错误
		{
			if (ferror(fp1) != 0) //判断是不是到达文件尾
			{
				perror("fread");
				return -1;
			}				
			break;
			
		}
			
		write_ret = fwrite(read_buffer, 1, read_ret, fp1);
		if (write_ret != read_ret)
		{
			printf("fwrite failed\n");
			return -1;
		}
		
		fflush(fp1);   //将文件流中未写出的数据立即写出，不用加也可以
			
	}

	
	fclose(fp1);//关闭文件，隐含一次fflush
	fclose(fp2);
	fp1 = fp2 = NULL;
	return 0;
}
#endif 

/*
程序功能描述：使用fwrite写数据到文件，fseek定位之后，再使用fread读取出来。在程序中分析了越界写入问题。
*/
#if  1
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

	//使用fwirte写字符数组时，应该使用strlen指定数据大小，不要使用sizeof，因为这样有可能会写入很多空字符
	//write_ret = fwrite(writebuf, 1, strlen(writebuf), fp);
	//if (write_ret != strlen(writebuf))

	//write_ret = fwrite(writebuf, 1, sizeof(writebuf), fp);
	//if (write_ret != sizeof(writebuf))

	//如果指定的数目超过数组的大小，还是按照指定的大小输入数据，这样数组就会越界，写入的数据是不可知的
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
	
	
	fseek(fp, 0, SEEK_SET); //偏移文件指针到文件头
	//如果fwrite故意越界写入，写入两倍数组大小的数据，那么除非fread数组大小加倍，否者read读取到数组也会越界导致发生错误
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
	
	fclose(fp);
	fp = NULL;
	return 0;
}
#endif 


