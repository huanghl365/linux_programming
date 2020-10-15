#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
