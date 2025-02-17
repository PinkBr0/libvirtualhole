/*
 * Implementation file for NovInt Falcon User Space Driver - Bootloader and Base Functionality
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#include "virtualhole.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEVICES 128

unsigned char command[100];

int virtualhole_init(virtualhole_device* dev)
{
	return FT_OK;
}

int virtualhole_set_speed(virtualhole_device* dev, virtualhole_info info)
{
	int bytes_written, bytes_read;
	command[0] = 0x10 + info.motor;
	command[1] = info.speed;
	FT_Write((dev->device), command, 2, &bytes_written);
 	FT_Read((dev->device), command, 1, &bytes_read);
	return 0;
}

int virtualhole_set_speeds(virtualhole_device* dev, virtualhole_info* info, unsigned int command_count)
{
	unsigned char command[100];
	int bytes_written, bytes_read;
	int i;
	for(i = 0; i < command_count; ++i)
	{
		command[i * 2] = 0x10 + info[i].motor;
		command[(i * 2) + 1] = info[i].speed;
	}
	FT_Write((dev->device), command, 2*command_count, &bytes_written);
 	FT_Read((dev->device), command, 1, &bytes_read);
}

int virtualhole_get_count(virtualhole_device* dev)
{
    unsigned int falcon_count;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	int i;
	FT_STATUS ftStatus;

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
#ifndef WIN32
	FT_SetVIDPID(VIRTUALHOLE_VENDOR_ID, VIRTUALHOLE_PRODUCT_ID);
#endif
	if((ftStatus = FT_ListDevices(pcBufLD, &falcon_count, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER)) != FT_OK) return -1;
	for(i = 0; ( (i <MAX_DEVICES) && (i < falcon_count) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}
	return falcon_count;
}

int virtualhole_open(virtualhole_device *dev, unsigned int device_index)
{
	unsigned int falcon_count, i;
	char* pcBufLD[MAX_DEVICES + 1];
	char cBufLD[MAX_DEVICES][64];
	char test[10];
	FT_STATUS ftStatus;
	int bytes_written, bytes_read;	
	//If we're not using windows, we can set PID/VID to filter on. I have no idea why this isn't provided in the windows drivers.
#ifndef WIN32
	FT_SetVIDPID(VIRTUALHOLE_VENDOR_ID, VIRTUALHOLE_PRODUCT_ID);
#endif

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	if((ftStatus = FT_ListDevices(pcBufLD, &falcon_count, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER)) != FT_OK) return ftStatus;
	if(device_index > falcon_count)	return ftStatus;
	for(i = 0; ( (i <MAX_DEVICES) && (i < falcon_count) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}

	//Open and reset device
	if((ftStatus = FT_OpenEx(cBufLD[0], FT_OPEN_BY_SERIAL_NUMBER, &(dev->device))) != FT_OK) return ftStatus;
	if((ftStatus = FT_ResetDevice((dev->device))) != FT_OK) return ftStatus;

	//Set to:
	// 9600 baud
	// 8n1
	// No Flow Control
	// RTS Low
	// DTR High	
	if((ftStatus = FT_SetBaudRate((dev->device), 9600)) != FT_OK) return ftStatus;
	if((ftStatus = FT_SetDataCharacteristics((dev->device), FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)) != FT_OK) return ftStatus;
	if((ftStatus = FT_SetFlowControl((dev->device), FT_FLOW_NONE, 0, 0)) != FT_OK) return ftStatus;
//	if((ftStatus = FT_ClrRts((dev->device))) != FT_OK) return ftStatus;
//	if((ftStatus = FT_ClrDtr((dev->device))) != FT_OK) return ftStatus;
//	if((ftStatus = FT_SetDtr((dev->device))) != FT_OK) return ftStatus;

 	FT_Read(dev, test, 10, &bytes_read);

	return FT_OK;
}

int virtualhole_close(virtualhole_device* dev)
{
	if(!dev) return -1;
	FT_Close((dev->device));
	return 0;
}
