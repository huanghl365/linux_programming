#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
