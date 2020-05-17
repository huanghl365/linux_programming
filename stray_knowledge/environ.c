#include <stdio.h>
extern char **environ;

//读取进程中的环境变量，environ指针指向环境变量的指针数组
int main(int argc, char *argv[])
{
	int i = 0;
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i++]);
	}
	return 0;
}

