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
	char ch = '\0';
	char ch1 = '\0';
	char ch2 = '\0';
	int num = 9678;
	double num2 = 12.48568;
	printf("%010d\n",num); //字段限定长度为10，小于10以0填充，如果长度大于10，不截断
	printf("%010.4lf\n", num2); //字段限定长度为10（包括.），小数限定为4位（超过4位四舍五入），如果长度小于10以0填充，如果长度大于10，不截断
	
	//scanf("%c,%d,%lf", &ch, &num, &num2);  //空格作为分隔符，输入时要添加空格作为分隔符
	//scanf("%c %d %lf", &ch, &num, &num2);  //,作为分隔符，输入时要添加,作为分隔符
	scanf("helloworld:%c %d %lf", &ch, &num, &num2);   //转换空字符中如果加入了字符串，那么输入的时候也要输入字符串进行匹配
	printf("%c\n",ch);
	printf("%d\n",num);
	printf("%lf\n",num2);
	
	
	scanf("%c %c %c", &ch, &ch1, &ch2);  //第一个字符可以是空格，第二个开始不能是空格，因为空格被作为分隔符
	//scanf("%c%c%c", &ch, &ch1, &ch2); 	   //每个字符都能是空格
	//scanf函数并不够灵活并且有漏铜，可以使用fread和fgets，读取输入后对字符串进行分解
	printf("ch=%c\n",ch);
	printf("ch1=%c\n",ch1);
	printf("ch2=%c\n",ch2);
	return 0;
}
