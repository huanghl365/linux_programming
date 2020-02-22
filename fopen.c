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
		  size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream)

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
函数原型：
描述：函数fwrite从ptr给定的位置获取它们获取nmemb个长度的size的单元，并写入到stream指向的流中
参数：
ptr:指向用来写入到文件的数据buf
size:写入单元的大小
nmemb:写入单元的数目
stream：指向文件流指针
返回值：
返回写入的单元的数目，当size等于1，则表示写入的字节数目。
*/

/*
程序功能说明：测试fopen调用，读取一个文件，写入另一个文件
*/
#define READ_NUM  50

int main(void)
{
	FILE *fp1, *fp2;  //指针前面一定加上*
	char read_buffer[READ_NUM] = "\0";
	int read_cnt , write_cnt;
	fp1 = fopen("test1.txt", "a+");   			 //更新方式打开，接续写
	fp2 = fopen("test2.txt", "r");   			 //读方式打开
	
	
	if (fp1 != NULL && fp2 != NULL)
	{
		read_cnt = fread(read_buffer, 1, READ_NUM, fp2);
		printf("readcnt = %d\n",read_cnt);

		//write_cnt = fwrite(read_buffer, 1, strlen(read_buffer), fp1);
		write_cnt = fwrite(read_buffer, 1, sizeof(read_buffer), fp1);
		fflush(fp1);   //将文件流中未写出的数据立即写出
		printf("write_cnt = %d\n",write_cnt);
	}
	else
	{
		printf("fopen file failed\n");
		return 1;
	}
	fclose(fp1);//关闭文件，隐含一次fflush
	fclose(fp2);
	return 0;
}
