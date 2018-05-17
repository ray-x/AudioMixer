//-----------------------------------------------------------------------------
// F34x_MSD_F34x_Temp_Sensor.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains the functions used by the USB MSD to check if USB cable 
// is switched on. If so UART transmision is stopped
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000069
// Target:         C8051F34x
// Tool chain:     Keil
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_MSD
//
// Release 1.1
//    -All changes by PKC
//    -09 JUN 2006
//    -Replaced SFR definitions file "c8051f320.h" with "c8051f340.h"
//
// Release 1.0
//    -Initial Release
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F34x_MSD_Definitions.h"
#include "c8051f340.h"
#include "F34x_MSD_VBUS_Functions.h"
#include "F34x_MSD_File_System.h"
#include "F34x_MSD_Cmd.h"
#include "F34x_MSD_Log.h"
#include "F34x_MSD_Temp_Sensor.h"
#include <stdio.h>


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// check_USB
//-----------------------------------------------------------------------------
//
// This function checks if USB connection is set (USB cable is switched on)
//-----------------------------------------------------------------------------

void check_USB()
{
	static unsigned usb_enabled = 0;
	if(REG0CN & 0x40){	
		if(!usb_enabled){
			usb_enabled = 1;
			Temp_Sensor_Stop_Logging();
			Stop_Logging();
			printf(ENDLINE"USB ACTIVE"ENDLINE);
		}
	}else{
		if(usb_enabled){
			usb_enabled = 0;
			printf(ENDLINE"USB INACTIVE"ENDLINE);
			write_current_dir();
		  putchar(PROMPT);
		}
	}
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
