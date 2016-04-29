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
//	Name	 : wdt.h
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

#ifndef CHARDEV_H
  #define CHARDEV_H

#include <linux/ioctl.h> 

#define IOCTL_NUM 'd'

#define SUCCESS 0

// Read the WDT
#define IOCTL_READ_WDT _IOWR(IOCTL_NUM, 1, int)

// Write to the WDT
#define IOCTL_WRITE_WDT _IOWR(IOCTL_NUM, 2, int)

// Set seconds
#define IOCTL_SET_WDT_SEC _IOWR(IOCTL_NUM, 3, int)

// Set minutes
#define IOCTL_SET_WDT_MIN _IOWR(IOCTL_NUM, 4, int)

#endif
