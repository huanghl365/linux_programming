#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#if 0
int main(int argc, char *argv[])
{

#if 0
	//system运行以字符串参数传递给它的命令并等待命令执行完成才返回，如果不
	//想等待命令执行完成才返回，那么可以给命令加上& 后台运行。
	//system执行命令的过程相当bash -c command, 即相当于打开一个shell来运行命
	//令，创建一个子进程，命令执行后主函数会继续运行不会退出。
	system("ps -l &");
	printf("done");
#else
	//可以使用excel将当前进程替换为另一个进程，并且当新的进程执行后会退出当前进程，因此不会看到printf打印
	execl("/bin/ps", "ps", "-l", NULL);
	printf("done");
#endif
}

#else
int main(int argc, char *argv[])
{

	//1、编译出toupper.out,用来将小写字母翻译成大写
#if 0 
	int ch;
	while ((ch = getchar()) != EOF)
	{
		putchar(toupper(ch));
	}
#endif 

	//2、将文件流重定向到标准输入，使用execl调用toupper.out
#if 1
	FILE *fptr = NULL;
	if (argc != 2)
	{
		printf("usage: ./a.out filename");
		exit(1);
	}

	fptr = freopen(argv[1], "r", stdin); //打开文件，并且重新打开标准输入，将文件流绑定到标准输入
	if (fptr == NULL)
	{
		perror("freopen:");
		exit(2);
	}
	else
	{
		//已经打开的文件在执行execl之后会保留下来,因此文件流会通过标准输入重定向给toupper.out
		execl("./toupper.out", "toupper.out", NULL);
		printf("done");
		exit(3);
	}
#endif 

	return 0;

}
#endif

