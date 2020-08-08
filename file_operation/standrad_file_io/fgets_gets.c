#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
函数原型：char *fgets(char *s, int size, FILE *stream);
描述：从文件流中读取最多size-1个字符到s指定的buffer中。当发生下面三种情况时结束读取：
（1）读取到size-1个字符
（2）读取到换行符（保留换行符）
（3）读取到文件尾
	fgets会给读取的字符串添加'\0'。

参数：
s：指向用来保存读取字符串的buffer
size：指定要读取的字符数目，只会读取size-1
stream：文件流指针
返回值：
读取成功时返回指向s的指针；当发生错误、或者在没有读取到任何字符的情况下(直接读取到文件尾)返回NULL。


函数原型：char *gets(char *s);
描述：从标准输入读取一行到s指定的buffer中，当发生以下情况时结束读取：
（1）当读取到换行符（丢弃换行符）
	gets会给读取的字符串添加'\0'。
参数：
s：指向用来保存读取字符串的buffer
返回值：
读取成功时返回指向s的指针；当发生错误或者没有读取到任何字符的情况下(直接读取到换行符)返回NULL。

注意：gets对读取的字符串的长度没有限制，有可能会导致缓冲区溢出，因此最好用fgets来替代gets
*/


/*
程序功能描述：分析fgets 和 gets函数的使用区别
*/

#define READ_SIZE 128
int main(void)
{
	FILE *fp1;  //指针前面一定加上*
	char ch,ch2;
	char read_buffer[READ_SIZE] = "\0";



	
/*
测试：fgets 和 gets分别从标准输入读取
测试结果：
fgets会保留换行符，需要处理一下，去掉换行符；
fgets对读取的字符串有长度上的限制，读取size-1个字节，并在末尾添加'\0'

gets当读取到换行符时会丢弃换行符，添加‘\0’；
gets对读取的字符串长度没有限制，容易溢出，因此最好用fgets替代
*/

#if 1		

	if (NULL != fgets(read_buffer, READ_SIZE, stdin)) 
	{	
		if ('\n' == read_buffer[strlen(read_buffer)-1])  
		{
			read_buffer[strlen(read_buffer)-1] = '\0';
		}
		
		printf("%s", read_buffer);
	}

	memset(read_buffer, 0, READ_SIZE);
	if (NULL != gets(read_buffer))
		printf("%s", read_buffer);

#endif 
	
	
	return 0;
}
