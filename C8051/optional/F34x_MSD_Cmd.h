//-----------------------------------------------------------------------------
// F34x_MSD_Cmd.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_Cmd.c
//
//
// FID:            34X000031
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

#ifndef _CMD_H_
#define _CMD_H_

#define CLS 12
#define BACKSPACE 8
#define CR '\r'
#define LF '\n'
#define SPACE ' '
#define PROMPT '>'

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Cmd_Init(void);
void Cmd_Step(void);

#endif
