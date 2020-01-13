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

	time_t t;
	time_t *pt = &t;//注意指针要指向一个对象
	char *pc;
	struct tm *pgm;
	char strfbuf[100];
	struct timeval tv;
	/*
	time_t time(time_t *t)
	描述：
	time能得到一个当前时间距离标准起点时间1970-01-01 00:00:00 +0000(UTC)过去了多少秒
	参数：
	t：输入型参数，系统将距离标准起点时间的秒数写入到t中，可以设置为NULL。
	返回值：
	距离标准起点时间的秒数。
	*/
	t = time(NULL);
	printf("time得到的时间：%ld秒\n", t);
	time(pt);
	printf("time得到的时间：%ld秒\n", *pt);


	/*
	char *ctime(const time_t *timep);
	描述：ctime可以从time_t出发得到一个容易观察的字符串格式的当前时间
	参数：
	timep：通过time调用获取time_t类型的指针作为输入型参数
	返回值：
	一个容易观察的字符串格式的时间,失败返回NULL
	*/
	pc = ctime(pt);
	if(pc != NULL)
		printf("ctime得到的时间字符串：%s", pc);

	/*
	struct tm *gmtime(const time_t *timep);
	描述：获取以struct tm结构体表示的时间，gmtime获取的时间中年份是以1900为基准的差
	值，月份是0表示1月，小时数是以UTC时间的0时区为标准的小时数（英国伦敦）
	参数：
	timep：通过time调用获取time_t类型的指针作为输入型参数
	返回值：
	struct tm类型的指针，失败返回NULL
	*/

	pgm = gmtime(pt);
	if(pgm != NULL)
	{
		printf("gmtime得到的时间：国际时间\n");
		printf("%d年（1900年起）%d月（0起）%d日（1起）\n", pgm->tm_year, pgm->tm_mon, pgm->tm_mday);
		printf("%d时（0起）%d分（0起）%d秒（0起）\n", pgm->tm_hour, pgm->tm_min, pgm->tm_sec);
	}
	
	//localtime和gmtime的唯一区别就是localtime以当前计算机中设置的时区为小时的时间基准，其余一样。
	pgm = localtime(pt);
	if(pgm != NULL)
	{
		printf("localtime得到的时间：北京时间\n");
		printf("%d年（1900年起）%d月（0起）%d日（1起）\n", pgm->tm_year, pgm->tm_mon, pgm->tm_mday);
		printf("%d时（0起）%d分（0起）%d秒（0起）\n", pgm->tm_hour, pgm->tm_min, pgm->tm_sec);
	}


	/*
	char *asctime(const struct tm *tm);
	描述：从struct tm类型的时间出发想得到字符串格式的时间，与ctime返回的时间一样
	参数：
	tm：struct tm类型的指针作为输入型参数，通过gmtime或者localtime获取
	返回值：
	一个容易观察的字符串格式的时间,失败返回NULL
	*/
	pc = asctime(pgm);
	if(pc != NULL)
	{
		printf("asctime得到的时间字符串：%s", pc);
	}

	/*
	size_t strftime(char *s, size_t max, const char *format,const struct tm *tm);
	描述：如果想从struct tm类型的时间出发想得到自定义字符串格式的时间，可以用strftime。
	参数：
	s：指向字符数组，作为输出型参数保存时间字符串
	max：字符数组的大小
	format：自定义输出格式
	tm：struct tm类型的指针作为输入型参数，通过gmtime或者localtime获取
	返回值：
	返回时间字符串的长度，不包含'\0',长度超过max返回0
	*/
	memset(strfbuf, 0 ,sizeof(strfbuf));
	strftime(strfbuf, sizeof(strfbuf), "%Y-%M-%d  %T", pgm);
	printf("strftime自定义时间格式：[%s]\n", strfbuf);

	/*
	time_t mktime(struct tm *tm);
	描述：从OS中读取时间时用不到mktime的，这个mktime是用来向操作系统设置时间时用的。
	参数：struct tm类型的指针作为输入型参数，通过gmtime或者localtime获取
	返回值：返回设置的时间，单位为秒
	*/
	t = mktime(pgm);
	printf("mktime设置的时间为：%ld秒\n", t);


	/*
	int gettimeofday(struct timeval *tv, struct timezone *tz);
	描述：有时候我们程序希望得到非常精确的时间（譬如以us为单位），这时候就只能通过gettimeofday来实现了。
	参数：
	tv：输出型参数，       保存获取的时间，精确到微妙
	struct timeval 
	{
       time_t      tv_sec;     //seconds
       suseconds_t tv_usec;    //microseconds 
    };
	tz：输出型参数，一般设置为NULL
	返回值：
	成功返回0，失败返回-1
	*/
	gettimeofday(&tv, NULL);
	printf("gettimeofday得到的时间：\n");
	printf("秒数：%lu  微妙：%lu\n", tv.tv_sec, tv.tv_usec);

	return 0;

}
