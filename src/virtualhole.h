#ifndef LIBVIRTUALHOLE_H
#define LIBVIRTUALHOLE_H

#if defined(LIBFTDI)
#include <ftdi.h>
typedef struct ftdi_context virtualhole_handle; /*!< libftdi typedef for virtual hole access */
#elif defined(LIBFTD2XX)
#ifdef WIN32
#include <windows.h>
#endif
#include <ftd2xx.h>
typedef FT_HANDLE virtualhole_handle; /*!< ftd2xx typedef for virtual hole access */
#else
#error "Either LIBFTDI or FTD2XX (but not both) must be defined to compile libnifalcon"
#endif

/// VID for the Novint Falcon
#define VIRTUALHOLE_VENDOR_ID 0x0403
/// PID for the Novint Falcon
#define VIRTUALHOLE_PRODUCT_ID 0x6001

#ifdef __cplusplus
extern "C" {
#endif

typedef struct virtualhole_device {
	virtualhole_handle device; /*!< FTDI object to access falcon */
	char is_initialized;  /*!< Boolean set to true when device is opened successfully, false when closed/uninitialized */
	char is_open; /*!< Boolean set to true when device is opened successfully, false when closed/uninitialized */
	int status_code; /*!< Status code returned from either libnifalcon or ftdi access library (can also be bytes read/written, etc...) */
	char* status_str;	 /*!< Status string for libnifalcon specific errors and messages */   
} virtualhole_device ;

int virtualhole_init(virtualhole_device* dev);

/** 
 * Counts the number of devices connected to the system
 * 
 * @return Number of falcons connected, -1 on error
 */
int virtualhole_get_count(virtualhole_device* dev);

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
int virtualhole_close(virtualhole_device* dev);

/** 
 * Sets the speed of a certain motor
 *
 * @param dev Device handle to initialize
 * @param motor_index Index of the motor to send control message to
 * @param speed 8-bit speed value to set motor to
 *
 * @return FT_OK on success, FTDI driver error on failure
 */
int virtualhole_set_speed(virtualhole_device* dev, unsigned char motor_index, unsigned char speed);

#ifdef __cplusplus
}
#endif

#endif

