#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
int main()

{
	unsigned char ch[1024] = {'\0'};
	int in, out;
	int nread = 0;
	in = open("/home/mxc/testfile", O_RDONLY);
	//O_CREAT 创建文件，文件存在则不创建，O_TRUNC清空文件
	//S_IRUSR  S_IWUSR 设置创建文件的权限，与linux系统文件的umask 反值（反值不包括执行权限） 取与
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	//O_APPEND 续写文件
	//out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	//O_CREAT 创建文件， O_EXCL如果文件存在则报错
	out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (out < 0)
	{
		perror("open");
	}
	//read属于系统调用，每次调用都会有用户到内核空间的切换开销，因此加大每次读取字节，减小切换次数减小开销。
	while ((nread = read(in, ch, 1024)) > 0 )
	{
		
		write(out, ch, nread);
	}

	close(in);
	close(out);
	exit(0);
}
