#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define READ_SIZE 1024
int main(void)
{
	FILE *fp1;  //指针前面一定加上*
	char ch,ch2;
	char read_buffer[READ_SIZE] = "\0";
	fp1 = fopen("/home/mxc/testfile", "r+");  //更新方式打开，读写
	
	//getc\fgetc\getchar返回读取到的字符，字符为int类型
	//fgetc和getc到达文件尾或读取出错时，返回EOF
	ch = (char)getc(fp1);
	ch2 = (char)fgetc(fp1);
	printf("ch = %c\n", ch);
	printf("ch2 = %c\n", ch2);
	
	ch = (char)getc(stdin);
	ch2 = (char)fgetc(stdin);
	printf("ch = %c  %d\n", ch, ch);
	printf("ch2 = %c  %d\n", ch2, ch2);
	getchar();
	
	//putc\fputc\putchar返回值为写入的字符，字符为int类型
	//putc\fputc\putchar写入失败返回EOF
	putc((int)'Y', fp1);
	fputc((int)'E', fp1);
	putchar((int)'S');
	
	
	//fgets遇到三种情况读取结束：读取到n-1个字符；读取到换行符（保留换行符）；读到文件尾。fgets会给读取的字符串添加'\0'。
	//fgets返回一个指向读取字符串的指针;当读取到文件尾EOF时，返回空指针；出错时，返回空指针并设置errno
	fseek(fp1, 0, SEEK_SET);  
	while(NULL != fgets(read_buffer, READ_SIZE, fp1))  
	{
		printf("%s", read_buffer);
		memset(read_buffer, 0, READ_SIZE);
	}
	
	
	//gets读取到换行符停止读取（丢弃换行符）。gets会给读取的字符串添加'\0'。
	//gets返回一个指向读取字符串的指针;读取出错返回空指针
	//gets对读取字符串长度没有限制，因此最好使用fget替代
	memset(read_buffer, 0, READ_SIZE);
	/*if (NULL != gets(read_buffer))
		printf("%s", read_buffer);*/
	
	if (NULL != fgets(read_buffer, READ_SIZE, stdin))
		printf("%s", read_buffer);
	
	fclose(fp1);
	return 0;  
}