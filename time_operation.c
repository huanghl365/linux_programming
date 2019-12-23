#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
/*
time_t time(time_t *t);
系统调用 time 可以获取系统时间（秒数）。
执行系统调用time，可以获取系统时间，系统时间将写入t指针指向的内存处，并将系统时间作为返回值。
*/
int main()
{
	time_t timeval;
	struct tm *tmptr;
	time(&timeval);
	printf("timeval = %ld\n", timeval);
	printf("%s", ctime(&timeval));
	tmptr = localtime(&timeval);
	printf("%d-%d-%d  %d-%d-%d\n", tmptr->tm_year%100+2000,tmptr->tm_mon+1, tmptr->tm_mday, tmptr->tm_hour,tmptr->tm_min, tmptr->tm_sec);
	
	strftime(buf, 256, "%Y-%m-%d %A %H:%M:%S", tmpstr);
        printf("%s\n", buf);
	return 0;
}
