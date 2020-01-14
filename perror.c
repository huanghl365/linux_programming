#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
	int  out;
	char *pstr = NULL;
	out = open("test.txt", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
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
	return 0;
}
