//-----------------------------------------------------------------------------
// F34x_MSD_Log.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// File contains functions for creating, updating and closing log file.
// In addition it contains the crank function which uses all function 
// and builds whole logging module to boot.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000041
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
#include "F34x_MSD_File_System.h"
#include <stdio.h>

static xdata BYTE Logging=0;

FILE xdata Log_File;

sbit at 0xa0 Sw1;
sbit at 0xa1 Sw2;                	// SWx='0' means switch pressed

static xdata BYTE Sw1_Prev,Sw2_Prev;

//----------------------------------------------------------------------------
// Stop_Logging
//----------------------------------------------------------------------------
//
// This function stops logging and closes the Log file.
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Stop_Logging()
{
	if(Logging) {
		printf("Logging done."ENDLINE);
	    fclose(&Log_File);
	    Logging=0;
	    Sw1_Prev=Sw2_Prev=5; // Force triggering the first time
	}
}

//----------------------------------------------------------------------------
// Log
//----------------------------------------------------------------------------
//
// Function opens the Log file and starts writing.
//
// Parameters   : filename - pointer to file name
// Return Value :
//----------------------------------------------------------------------------

void Log(char* filename)
{
  if(Logging) {
  	Stop_Logging();
  } else {
    //"file without name" fix (masw):
    if (filename == 0 ) {
      printf("No file name."ENDLINE);
      return;
    }
    //===============================
    if(fopen(&Log_File,filename,"a")) { // Append mode
      printf("Logging buttons. Type 'log' again to stop."ENDLINE);
      Logging=1;
      tickcount=0;
    } else {
      printf("Cannot write."ENDLINE);
    }
  }
}

//----------------------------------------------------------------------------
// Log_Step
//----------------------------------------------------------------------------
//
// Crank function which checks the Log file and creates the logging module.
// This function also adds the line to Log file
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Log_Step(void)
{
  /*xdata*/ char buf[16];
  
  if(Logging) {
    if(Sw1_Prev!=Sw1 || Sw2_Prev!=Sw2) {
      Sw1_Prev=Sw1;Sw2_Prev=Sw2;
      sprintf(buf,"%10lu ms ",tickcount);
      tickcount=0;

      fwrite(&Log_File,buf,14);
      fwrite(&Log_File,Sw1_Prev?"OFF ":"ON  ",4);
      //fwrite(&Log_File,Sw2_Prev?"OFF"ENDLINE :"ON "ENDLINE,4);
      fwrite(&Log_File,Sw2_Prev?"OFF\r\n":"ON \r\n",5);	
    }
  }
}

