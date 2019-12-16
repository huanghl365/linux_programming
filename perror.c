#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main()
{
	
	int  out;
	char *pstr = NULL;
	out = open("/home/mxc/file.out", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (out < 0)
	{
		//根据error打印错误信息，可以自己加上提示字符串
		perror("open");
		
		//根据error返回错误信息
		pstr = strerror(errno);
		if (pstr != NULL)
		{
			printf("%s\n",pstr);
		}
	}
	
	close(out);
	exit(0);
}
