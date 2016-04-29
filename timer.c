//*****************************************************************************
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
//*****************************************************************************
//
//	Name	 : timer.c
//
//	Project	 : WDT Sample Application Program
//
//	Author	 : Paul DeMetrotion
//
//*****************************************************************************
//
//	  Date		Revision	                Description
//	--------	--------	---------------------------------------------
//	04/09/12	  1.0		Original Release	
//
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "wdt.h"

void init_keyboard(void);
void close_keyboard(void);
int kbhit(void);
int readch(void);

main(int argc, char *argv[])
{
	int timeout, key;

	// check syntax and availability
	if (argc != 3)
	{
		printf("Usage: timer <time> <sec/min>\n");
		printf("Example: timer 100 sec\n");
		exit(1);
	}
	else if(read_wdt() < 0)
	{	
	    fprintf(stderr, "Can't access device WDT - Aborting\n");
	    exit(1);
	}
	else
	{
		printf("Test program WinSystems Watchdog Timer\n");
		timeout = atoi(argv[1]);
		printf("  Settings: %d %s\n", timeout, argv[2]);
		printf("  Hit any key to start program\n");
		init_keyboard();
	}

	// wait for keystroke
	while (!kbhit()) ;
	readch();

	// configure wdt
	if (*argv[2] == 's')
		set_wdt_sec();
	else
		set_wdt_min();

	write_wdt(timeout);

	while(1)
	{
		printf("  ***** Current count = %d *****\n", read_wdt());

		if (kbhit())
		{
			key = readch();
			if (key == 'q')	// exit
			{
				write_wdt(0);	// disable wdt
				close_keyboard();
				exit(0);
			}
			else	// reset wdt
				write_wdt(timeout);
		}

		usleep(1000000);	// wait 1 sec
	}
}
