
# WINSYSTEMS WDT COMeT6-1100 Linux Driver

The WDT Device Driver Package consists of a Linux Device Driver, an application programming interface library, and example application programs. The driver was built and tested on a Linux based Ubuntu 22.04 LTS distribution. This release corresponds to Linux kernel version 6.5. The driver is for use with WinSystems, Inc. COMeT6-1100 (C460) module that utilize our exclusive watchdog timer. The timer is accessed through two hard-coded I/O ports, at address 0x265 and 0x266. Port 0x266 is used to enable the watchdog timer, program the desired time-out count value, and reset the counter. Port 0x265 is used to program the counter for seconds or minutes.  

## Building

To create the device driver Loadable Kernel Module and the sample applications in a command shell, execute: **_make all_**. The device driver Loadable Kernel Module **_wdt.ko_** is created and moved to the appropriate kernel driver directory. The file access permissions are set to allow access by all users and groups; they may be changed manually as desired. The sample program **_timer_** is also built.  

**_make install_** will install the kernel driver to a kernel directory and create dependencies.  
**_make uninstall_** will remove the kernel driver from the kernel directory.  
**_make timer_** will create the timer sample program.  
**_make clean_** will remove objects created by the build.  
**_make spotless_** will forcibly remove all artifacts of the build.  

## Load Driver

The device driver can be loaded with the provided initialization script **_wdt_load_** or manually. In either case modprobe is used to install the driver. Executing: **_modprobe wdt.ko_**  

The **_wdt_load_** script can be added to the /etc/rc.local file to load the driver automatically on boot.  
The **_wdt_unload_** script can be used to unload the driver.  

## Usage

An object file containing the Application Programming Interface utilized by user level programs to access the Kernal Loadable Module device driver driven devices is created as part of the build procedure.  

**_int_** **read_wdt(void)**  
>This function returns the current value of port 0x266 or -1 if the watchdog timer is inaccessible.  

**_int_** **write_wdt(int value)**  
>This function writes the desired value to port 0x266. It returns the value 0 for a successful write or -1 if the watchdog timer is inaccessible. Writing a 0 to this address will disable the watchdog timer. Any other value enables the counter and starts the count-down timer. Subsequent writes reset the timer.  

**_int_** **set_wdt_sec(void)**  
>This function writes the value 0x80 to port 0x265 which selects seconds as the unit for the count-down timer. It returns the value 0 for a successful write or -1 if the watchdog timer is inaccessible.  

**_int_** **set_wdt_min(void)**  
>This function writes the value 0 to port 0x265 which selects minutes as the unit for the count-down timer. It returns the value 0 for a successful write or -1 if the watchdog timer is inaccessible.  

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

|Date||Comment|
|--|-|--|
|02/19/2025|ALH|Initial Commit for COMeT6-1100, update base address|

## License

(C) Copyright 2025 by WINSYSTEMS, Inc.  
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 2 of the License.
