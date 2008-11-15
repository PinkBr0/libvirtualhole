#include "virtualhole.h"
#include <stdio.h>
int main(int argc, char** argv)
{
	virtualhole_device dev;
	int count;
	int i, j;
	count = virtualhole_get_count();
	if(!count)
	{
		printf("No Virtual Holes Found\n");
		return 1;
	}
	if(virtualhole_open(&dev, 0) != FT_OK)
	{
		printf("Cannot open device!\n");
		return 1;
	}
	printf("Device opened\n");
	for(j = 0; j < 100;  ++j)
	{
		printf("setting\n");
	for(i = 0; i < 10; ++i)
	{
		if(virtualhole_set_speed(dev, i, 255) != FT_OK)
		{
			printf("Cannot write to device!\n");
			break;
		}
	}
	printf("Finished setting\n");
	//sleep(500);
	for(i = 0; i < 10; ++i)
	{
		if(virtualhole_set_speed(dev, i, 255) != FT_OK)
		{
			printf("Cannot write to device!\n");
			break;
		}
	}
	printf("Finished unsetting\n");
	sleep(500);
}
	virtualhole_close(dev);
	return 0;
}
