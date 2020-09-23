#include <stdio.h>
#include <stdlib.h>

void fun1(void)
{
	printf("process over, call the fun1\n");
}
void fun2(void)
{
	printf("process over, call the fun2\n");
}


int main(void)
{
	/*测试：atexit注册进程终止处理函数,先注册的后执行,当在main中执行return或者程序使用了exit调用时，会执行回调函数
	测试结果：先打印hello world再执行进程终止函数*/
	atexit(fun1);
	atexit(fun2);
	printf("hello world\n");
	return 0;
}

