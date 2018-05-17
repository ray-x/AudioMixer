//-----------------------------------------------------------------------------
// F34x_MSD_Temp_Sensor.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_Temp_Sensor.c
//
//
// FID:            34X000056
// Target:         C8051F34x
// Tool chain:     Keil
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_MSD
//
// Release 1.1
//    -All changes by PKC
//    -09 JUN 2006
//    -No changes; incremented revision number to match project revision
//
// Release 1.0
//    -Initial Release
//

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef __TEMP_SENSOR_H__
#define __TEMP_SENSOR_H__


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Init_Temp_Sensor();
void Calibrate(int temp_value);
int GetTemp();
void Temp_Log_Step(void);
void Temp_Log(char* filename);
void Temp_Sensor_Stop_Logging();
#endif
