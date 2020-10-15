#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

/*
程序功能描述：对比测试时间相关的系统调用
*/
int main()
{

	time_t t;
	time_t *pt = &t;//注意指针要指向一个对象
	char *pc;
	struct tm *pgm;
	char strfbuf[100];
	struct timeval tv;

	t = time(NULL);
	printf("time得到的时间：%ld秒\n", t);
	time(pt);
	printf("time得到的时间：%ld秒\n", *pt);


	pc = ctime(pt);
	if(pc != NULL)
		printf("ctime得到的时间字符串：%s", pc);


	pgm = gmtime(pt);
	if(pgm != NULL)
	{
		printf("gmtime得到的时间：国际时间\n");
		printf("%d年（1900年起）%d月（0起）%d日（1起）\n", pgm->tm_year, pgm->tm_mon, pgm->tm_mday);
		printf("%d时（0起）%d分（0起）%d秒（0起）\n", pgm->tm_hour, pgm->tm_min, pgm->tm_sec);
	}
	
	pgm = localtime(pt);
	if(pgm != NULL)
	{
		printf("localtime得到的时间：北京时间\n");
		printf("%d年（1900年起）%d月（0起）%d日（1起）\n", pgm->tm_year, pgm->tm_mon, pgm->tm_mday);
		printf("%d时（0起）%d分（0起）%d秒（0起）\n", pgm->tm_hour, pgm->tm_min, pgm->tm_sec);
	}


	pc = asctime(pgm);
	if(pc != NULL)
	{
		printf("asctime得到的时间字符串：%s", pc);
	}


	memset(strfbuf, 0 ,sizeof(strfbuf));
	strftime(strfbuf, sizeof(strfbuf), "%Y-%M-%d  %T", pgm);
	printf("strftime自定义时间格式：[%s]\n", strfbuf);


	t = mktime(pgm);
	printf("mktime设置的时间为：%ld秒\n", t);


	gettimeofday(&tv, NULL);
	printf("gettimeofday得到的时间：\n");
	printf("秒数：%lu  微妙：%lu\n", tv.tv_sec, tv.tv_usec);

	return 0;

}
