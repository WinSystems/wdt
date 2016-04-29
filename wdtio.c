//****************************************************************************
//	
//	Copyright 2012 by WinSystems Inc.
//
//	Permission is hereby granted to the purchaser of WinSystems CPU products 
//	to distribute any binary file or files compiled using this source code 
//	directly or in any work derived by the user from this file. In no case 
//	may the source code, original or derived from this file, be distributed 
//	to any third party except by explicit permission of WinSystems. This file 
//	is distributed on an "As-is" basis and no warranty as to performance or 
//	fitness of purposes is expressed or implied. In no case shall WinSystems 
//	be liable for any direct or indirect loss or damage, real or consequential 
//	resulting from the usage of this source code. It is the user's sole respon-
//	sibility to determine fitness for any considered purpose.
//
//****************************************************************************
//
//	Name	 : wdtio.c
//
//	Project	 : Watchdog Timer Linux Device Driver
//
//	Author	 : Paul DeMetrotion
//
//****************************************************************************
//
//	  Date		Revision	                Description
//	--------	--------	---------------------------------------------
//	04/09/12	  1.0		Original Release	
//
//****************************************************************************

#include <stdio.h>
#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */

// Include the WinSystems WDT definitions
#include "wdt.h"    

//**************************************************************************
//		USER LIBRARY FUNCTIONS
//**************************************************************************

// device handles
int handle = 0;

// the names of our device nodes
char *device_id={"/dev/wdt"};

//
//------------------------------------------------------------------------
//
// read_wdt - Read the watchdog timer
//
// Description:		This function will read the address 0x566 and return
//					the current watchdog timer count
//
// Arguments:
//
// Returns:
//			-1		If the handle is invalid
//					The result of the read
//
//------------------------------------------------------------------------
//
int read_wdt(void)
{
	int c;

    if(check_handle())
		return -1;

    c = ioctl(handle, IOCTL_READ_WDT);
    
    return c;
}

//
//------------------------------------------------------------------------
//
// write_wdt - Write to the watchdog timer
//
// Description:		This function will write the address 0x566 and reset
//					the watchdog timer
//
// Arguments:
//			val		The value to write to the register
//
// Returns:
//			-1		If the handle is invalid
//			 0		The write is completed
//
//------------------------------------------------------------------------
//
int write_wdt(int value)
{
	int c;

    if(check_handle())
		return -1;

    c = ioctl(handle, IOCTL_WRITE_WDT, value);

    return c;
}

//
//------------------------------------------------------------------------
//
// set_wdt_sec - Sets the watchdog timer units to seconds
//
// Description:		This function will write 0x80 to the address 0x565 
//					and set the watchdog timer units to seconds
//
// Arguments:
//
// Returns:
//			-1		If the handle is invalid
//			 0		The units are set to seconds
//
//------------------------------------------------------------------------
//
int set_wdt_sec(void)
{
	int c;

    if(check_handle())
		return -1;

    c = ioctl(handle, IOCTL_SET_WDT_SEC);

    return c;
}

//
//------------------------------------------------------------------------
//
// set_wdt_min - Sets the watchdog timer units to minutes
//
// Description:		This function will write 0 to the address 0x565 
//					and set the watchdog timer units to minutes
//
// Arguments:
//
// Returns:
//			-1		If the handle is invalid
//			 0		The units are set to minutes
//
//------------------------------------------------------------------------
//
int set_wdt_min(void)
{
	int c;

    if(check_handle())
		return -1;

    c = ioctl(handle, IOCTL_SET_WDT_MIN);

    return c;
}

//
//------------------------------------------------------------------------
//
// check_handle - Checks that a handle to the appropriate device file
//					exists. If it does not a file open is performed.
//
// Description:
//
// Arguments:
//			chip_number	The 1 based index of the chip
//
// Returns:
//			0		if handle is valid
//			-1		If the chip does not exist or it's handle is invalid
//
//------------------------------------------------------------------------
//
int check_handle(void)
{
    if(handle > 0)	// If it's already a valid handle
		return 0;

    if(handle == -1)	// If it's already been tried
		return -1;

	// Try opening the device file, in case it hasn't been opened yet
    handle = open(device_id, O_RDWR);

    if(handle > 0)	// If it's now a valid open handle
		return 0;
    
    handle = -1;
		return -1;
}
