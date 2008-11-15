/*
 * Declaration file for NovInt Falcon User Space Driver - Bootloader and Base Functionality
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/libnifalcon
 *
 * This library is covered by the MIT License, read LICENSE for details.
 */

#ifndef LIBVIRTUALHOLE_H
#define LIBVIRTUALHOLE_H

#ifdef WIN32
#include <windows.h>
#endif
#include <ftd2xx.h>

/// Typedef over the FTDI driver handle
typedef FT_HANDLE virtualhole_device;

/// VID for the Novint Falcon
#define VIRTUAL_HOLE_VENDOR_ID 0x6001
/// PID for the Novint Falcon
#define VIRTUAL_HOLE_PRODUCT_ID 0x0403

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Counts the number of devices connected to the system
 * 
 * @return Number of falcons connected, -1 on error
 */
int virtualhole_get_count();

/** 
 * Opens the device
 * 
 * @param dev Pointer to store opened device handle
 * @param device_index Index of the device to open (for multiple devices)
 * 
 * @return FT_OK on success, FTDI driver error on failure
 */
int virtualhole_open(virtualhole_device *dev, unsigned int device_index);

/** 
 * Closes device passed to it
 * 
 * @param dev Device handle to close
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
int virtualhole_close(virtualhole_device dev);

/** 
 * Wrapper for FTDI read functions to do non-blocking, timeout capable read
 *
 * @param dev Device handle to initialize
 * @param str Data to send to device
 * @param size Size of data to send to device
 * @param timeout_ms Send timeout, in milliseconds
 * @param bytes_read Pointer to an int, stores the number of bytes read (in case time out is reached)
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
FT_STATUS virtualhole_set_speed(virtualhole_device dev, char motor_index, char speed);

#ifdef __cplusplus
}
#endif

#endif

