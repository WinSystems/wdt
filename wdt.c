/*
 * wdt.c: Watchdog Timer Linux Device Driver
 *
 * (C) Copyright 2012, 2016 by WinSystems, Inc.
 * Author: Paul DeMetrotion <pdemetrotion@winsystems.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

// #define DEBUG 1

/* Helper to format our pr_* functions */
#define pr_fmt(__fmt) KBUILD_MODNAME ": " __fmt

#include <linux/module.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "wdt.h"

#define MOD_DESC "WinSystems, Inc. Watchdog Timer Driver"
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(MOD_DESC);
MODULE_AUTHOR("Paul DeMetrotion");

// Driver major number
static int cdev_major;      // 0 = allocate dynamically
module_param(cdev_major, uint, S_IRUGO);
MODULE_PARM_DESC(cdev_major, "Set the major number for wdt device (0 = auto, default)");
static int wdt_major;

// wdt char device structure
static struct cdev wdt_cdev;

// This holds the base addresses of the wdt
static unsigned base_port = 0x564;
module_param(base_port, uint, S_IRUGO);
MODULE_PARM_DESC(base_port, "Set the base port for the wdt device (default is 0x564)");

// mutex & spinlock
static DEFINE_MUTEX(mtx);

//**********************************************************************
//			DEVICE OPEN
//**********************************************************************
// called whenever a process attempts to open the device file
static int device_open(struct inode *inode, struct file *file)
{
	if (base_port == 0) {
		pr_warning("**** OPEN ATTEMPT on uninitialized port *****\n");
		return -1;
	}

	pr_devel("device_open(%p)\n", file);

	return SUCCESS;
}

//**********************************************************************
//			DEVICE CLOSE
//**********************************************************************
static int device_release(struct inode *inode, struct file *file)
{
	pr_devel("device_release(%p,%p)\n", inode, file);

	return 0;
}

//**********************************************************************
//			DEVICE IOCTL
//**********************************************************************
static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int ret_val;

	pr_devel("IOCTL call Code %04X\n", ioctl_num);

	// Switch according to the ioctl called
	switch (ioctl_num) {
	case IOCTL_READ_WDT:
		ret_val = inb(base_port + 2);

		pr_devel("Reading WDT Port %04x -> %02x\n", base_port + 2, ret_val);

		return ret_val;

	case IOCTL_WRITE_WDT:
		// obtain lock before writing
		mutex_lock_interruptible(&mtx);

		ret_val = ioctl_param & 0xff;

		outb(ret_val, base_port + 2);

		//release lock
		mutex_unlock(&mtx);

		pr_devel("Writing to WDT Port %04x <- %02x\n", base_port + 2, ret_val);

		return SUCCESS;

	case IOCTL_SET_WDT_SEC:
		// obtain lock before writing
		mutex_lock_interruptible(&mtx);

		outb(0x80, base_port + 1);

		//release lock
		mutex_unlock(&mtx);

		pr_devel("Setting WDT to Seconds\n");

		return SUCCESS;

	case IOCTL_SET_WDT_MIN:
		// obtain lock before writing
		mutex_lock_interruptible(&mtx);

		outb(0, base_port + 1);

		//release lock
		mutex_unlock(&mtx);

		pr_devel("Setting WDT to Minutes\n");

		return SUCCESS;

	// Catch all return
	default:
		return(-EINVAL);
	}

	return SUCCESS;
}

//**********************************************************************
//			Module Declarations
// This structure will hold the functions to be called
// when a process does something to the device
//**********************************************************************
static struct file_operations wdt_fops = {
	owner:		THIS_MODULE,
	unlocked_ioctl:	device_ioctl,
	open:		device_open,
	release:	device_release,
};

//**********************************************************************
//			INIT MODULE
//**********************************************************************
// register the character device
int init_module()
{
	int ret_val;
	dev_t devno;

	// Sign-on
	pr_info(MOD_DESC " loading\n");

	// register the character device
	if (cdev_major) {
		wdt_major = cdev_major;
		devno = MKDEV(wdt_major, 0);
		ret_val = register_chrdev_region(devno, 1, KBUILD_MODNAME);
	} else {
		ret_val = alloc_chrdev_region(&devno, 0, 1, KBUILD_MODNAME);
		wdt_major = MAJOR(devno);
	}

	if (ret_val < 0) {
		pr_err("Cannot obtain major number %d\n", wdt_major);
		return -ENODEV;
	} else {
		pr_info("Major number %d assigned\n", wdt_major);
	}

	// add character device
	cdev_init(&wdt_cdev, &wdt_fops);
	wdt_cdev.owner = THIS_MODULE;
	wdt_cdev.ops = &wdt_fops;
	ret_val = cdev_add(&wdt_cdev, MKDEV(wdt_major, 0), 1);

	if (!ret_val) {
		pr_info("Added character device\n");
	} else {
		pr_err("Error %d adding character device\n", ret_val);
		goto exit_majnum_delete;
	}

	// check and map our I/O region requests
	if (request_region(base_port, 4, KBUILD_MODNAME) == NULL) {
		pr_err("Unable to use I/O Address %04X\n", base_port);
		goto exit_cdev_delete;
	} else {
		pr_info("Base I/O Address = %04X\n", base_port);
	}

	return SUCCESS;

exit_cdev_delete:
	cdev_del(&wdt_cdev);
exit_majnum_delete:
	unregister_chrdev_region(devno, 1);
	wdt_major = 0;
	return -ENODEV;
}

//**********************************************************************
//			CLEANUP MODULE
//**********************************************************************
// unregister the appropriate file from /proc
void cleanup_module()
{
	// unregister I/O port usage
	release_region(base_port, 4);

	// remove and unregister the device
	cdev_del(&wdt_cdev);

	unregister_chrdev_region(MKDEV(wdt_major, 0), 1);
	wdt_major = 0;
}
