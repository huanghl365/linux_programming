#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int i = 0, val = 0;

	printf("RAND_MAX = %d.\n", RAND_MAX);		// 2147483647

	
	
	while(1)
	{
		//srand使用time函数来播种子产生新的序列，缺陷是如果频率太快超过1HZ，则产生的序列可能会重复
		srand(time(NULL));
		for (i=0; i<6; i++)
		{
			//rand返回一个0 - RAND_MAX范围内的伪随机数
			val = rand();
			printf("%d ", (val % 6));
		}
		printf("\n");
		sleep(2);

	}
	printf("\n");
	
	return 0;
}


