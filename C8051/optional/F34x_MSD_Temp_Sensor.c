//-----------------------------------------------------------------------------
// F34x_MSD_Temp_Sensor.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains a functions for temperature sensor. These functions are
// used to reading the temperature value from AD converter and recalculate it
// to the value in celcius [C] to boot. In addition some of them are used to 
// creating and updating the Log file.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000055
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

#include "c8051f340.h"
#include "F34x_MSD_Temp_Sensor.h"
#include "F34x_MSD_File_System.h"
#include <stdio.h>

static FILE xdata Log_File;

static xdata int Adc_Offset = 0;
static unsigned int Get_Temp_Value();
static xdata char Logging = 0;

//----------------------------------------------------------------------------
// Init_Temp_Sensor
//----------------------------------------------------------------------------
//
// Function inits temperature sensor
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Init_Temp_Sensor()
{
  AMX0P = 0x1E;
  AMX0N = 0x1F;
  ADC0CN = 0x80;
  REF0CN = 0x0c;
  Adc_Offset = 0;
}

//----------------------------------------------------------------------------
// Calibrate
//----------------------------------------------------------------------------
//
// Function calibratea temperature sensor
//
// Parameters   : temp_value - ambient temperature in celcius [C] decimal 
//                           values second byte means digits after colon
// Return Value :
//----------------------------------------------------------------------------

void Calibrate(int temp_value)
{
  unsigned int /*xdata*/ tmpv = Get_Temp_Value();
  tmpv = (((tmpv * 322L) - 79700)/ 286) ;
  Adc_Offset = tmpv - temp_value;
}

//----------------------------------------------------------------------------
// Get_Temp_Value
//----------------------------------------------------------------------------
//
// Function returns value from AD converter. This value represents the 
// temperature.
//
// Parameters   :
// Return Value : result from AD converter
//----------------------------------------------------------------------------

unsigned int Get_Temp_Value()
{
  unsigned int /*xdata*/ retV = 0;
  AD0BUSY = 1;
  while(!AD0INT);
  retV = (ADC0H << 8) | (ADC0L);
  AD0INT = 0;
  return retV;
}

//----------------------------------------------------------------------------
// Get_Temp
//----------------------------------------------------------------------------
//
// Function returns temperature returned by temperature sensor
//
// Parameters   :
// Return Value : ambient temperature in celcius [C] decimal values second 
//                byte means digits after colon
//----------------------------------------------------------------------------

int Get_Temp()
{
  int /*xdata*/ ret = Get_Temp_Value();
  ret = (((ret * 322L) - 79700)/ 286) ;
  ret -= Adc_Offset;
  return ret;
}

//----------------------------------------------------------------------------
// Temp_Sensor_Stop_Logging
//----------------------------------------------------------------------------
//
// This function close temperature Log File
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Temp_Sensor_Stop_Logging()
{
	if(Logging) {
		printf("Logging temperature done."ENDLINE);
    	fclose(&Log_File);
    	Logging=0;
	}
}

//----------------------------------------------------------------------------
// Temp_Log
//----------------------------------------------------------------------------
//
// Function creates temperature Log File
//
// Parameters   : filename - name of Log file
// Return Value :
//----------------------------------------------------------------------------

void Temp_Log(char* filename)
{
  if(Logging) {
  	Temp_Sensor_Stop_Logging();
  } else {
    //"file without name" fix (masw):
    if (filename == 0 ) {
      printf("No file name."ENDLINE);
      return;
    }
    //===============================
    if(fopen(&Log_File,filename,"a")) { // Append mode
      printf("Logging temperature. Type 'templog' again to stop."ENDLINE);
      Logging=1;
      tickcount=0;
    } else {
      printf("Cannot write."ENDLINE);
    }
  }
}

//----------------------------------------------------------------------------
// Temp_Log_Step
//----------------------------------------------------------------------------
//
// Function writes another line to Log File
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Temp_Log_Step(void) {
  /*xdata*/ char buf[18];
  int /*xdata*/ retV ;
  if(Logging) {
    if(tickcount >=1000) {
		  retV = Get_Temp();
		  sprintf(buf,"%02d.%02d after 1sec" /*ENDLINE*/ "\r\n",(retV - (retV/100)*100),(retV / 100 ));
		  tickcount=0;
		  fwrite(&Log_File,buf,18);
	  }
  }
}
