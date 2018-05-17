//-----------------------------------------------------------------------------
// F34x_MSD_Dir_Commands.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_Dir_Commands.c
//
//
// FID:            34X000034
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


#ifndef __DIRECTORIES_COMMANDS_H__
#define __DIRECTORIES_COMMANDS_H__

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// See F34x_Dir_Commands.c for prototypes of functions with file scope


void Remove_Direcory(char* dir_name);
void Make_Directory(char* dir_name);
int Change_Directory(char* dir_name);

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define __DIRECTORIES_COMMANDS_H__

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
