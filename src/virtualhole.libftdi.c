#include "virtualhole.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEVICES 128

//Ripped from libftdi, so we can standardize how we return errors
#define virtualhole_error_return(code, str) do {	   \
		dev->virtualhole_status_str = str;			   \
        dev->virtualhole_status_code = -code;			   \
        return -code;							   \
   } while(0);

static int s_is_initialized = 0;

int virtualhole_init(virtualhole_device* dev)
{
	if((dev->status_code = ftdi_init(&(dev->device))) < 0) return dev->status_code;
	dev->is_open = 0;
	dev->status_str = "";
	dev->is_initialized = 1;
	return dev->status_code;
}

//Just wraps the FT_Read function in a QueueStatus call so we don't block on reads
int virtualhole_set_speed(virtualhole_device* dev, unsigned char motor_index, unsigned char speed)
{
	unsigned char command[100];
	int bytes_written, bytes_read;
	command[0] = 0x10 + motor_index;
	command[1] = speed;

	ftdi_write_data(&(dev->device), command, 2);
	ftdi_read_data(&(dev->device), command, 1);
	return 0;
}

int virtualhole_get_count(virtualhole_device* dev)
{
	int count;
	struct ftdi_device_list* dev_list[128];
	if(!dev->is_initialized)
	{
		printf("Not initialized\n");
		return -1;
	}
	count = ftdi_usb_find_all(&(dev->device), dev_list, VIRTUALHOLE_VENDOR_ID, VIRTUALHOLE_PRODUCT_ID);
	printf("counted %d\n", count);
	ftdi_list_free(dev_list);
	return count;
}

int virtualhole_open(virtualhole_device *dev, unsigned int device_index)
{
	unsigned int count, i, status;
	struct ftdi_device_list *dev_list, *current;

	count = ftdi_usb_find_all(&(dev->device), &dev_list, VIRTUALHOLE_VENDOR_ID, VIRTUALHOLE_PRODUCT_ID);
	if(count <= 0 || device_index > count)
	{
		ftdi_list_free(&dev_list);
		if(count == 0)
		{
			printf("no devices connected to system\n");
		}
		else
		{
			printf("device out of range\n");
		}
		return;
	}
	for(i = 0, current = dev_list; current != NULL && i < device_index; current = dev_list->next, ++i);
	if((ftdi_usb_open_dev(&(dev->device), current->dev)) < 0) return -1;
	ftdi_list_free(&dev_list);
	ftdi_usb_reset(&(dev->device));
	if((dev->status_code = ftdi_set_baudrate(&(dev->device), 9600)) < 0) return dev->status_code;
	if((dev->status_code = ftdi_set_line_property(&(dev->device), BITS_8, STOP_BIT_1, NONE)) < 0) return dev->status_code;
	if((dev->status_code = ftdi_setflowctrl(&(dev->device), SIO_DISABLE_FLOW_CTRL)) < 0) return dev->status_code;

	//VERY IMPORTANT
	//If we do not reset latency to 1ms, then we either have to fill the FTDI butter (64bytes) or wait 16ms
	//to get any data back. This is what was causing massive slowness in pre-1.0 releases
//	if(ftdi_set_latency_timer(*dev, 1) < 0) return -1;

	return 0;
}

int virtualhole_close(virtualhole_device* dev)
{
	if(!dev) return -1;
	ftdi_usb_close(&(dev->device));
	return 0;
}
