#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


int main(int argc, char **argv)
{
	struct timespec time1 = {0, 0}; 
	struct timespec time2 = {0, 0};
	
	float temp;
 
	clock_gettime(CLOCK_REALTIME, &time1);		
	sleep(2);
	clock_gettime(CLOCK_REALTIME, &time2);	 
	temp = (time2.tv_nsec - time1.tv_nsec) / 1000000;
	printf("time = %f ms\n", temp);
	return 0;
}

