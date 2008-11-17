#include "virtualhole.h"
#include <stdio.h>
int main(int argc, char** argv)
{
	virtualhole_device dev;
	virtualhole_info info;
	int count;
	int i, j;
	virtualhole_init(&dev);
	
	count = virtualhole_get_count(&dev);
	if(!count)
	{
		printf("No Virtual Holes Found\n");
		return 1;
	}
	if(virtualhole_open(&dev, 0) != 0)
	{
		printf("Cannot open device!\n");
		return 1;
	}
	printf("Device opened\n");
	for(i = 0; i < 10; ++i)
	{
		info.motor = i;
		info.speed = 63;
		if(virtualhole_set_speed(&dev, info) != 0)
		{
			printf("Cannot write to device!\n");
			break;
		}
	}
	sleep(3);
	for(i = 0; i < 10; ++i)
	{
		info.motor = i;
		info.speed = 0;
		if(virtualhole_set_speed(&dev, info) != 0)
		{
			printf("Cannot write to device!\n");
			break;
		}
	}
	printf("Finished unsetting\n");
	virtualhole_close(&dev);
	return 0;
}
