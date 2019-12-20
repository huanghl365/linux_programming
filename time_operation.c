#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

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
