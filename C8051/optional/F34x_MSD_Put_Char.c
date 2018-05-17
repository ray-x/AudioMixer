//-----------------------------------------------------------------------------
// F34x_MSD_Put_Char.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains function which sends one byte via UART.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000049
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
#include "F34x_MSD_Put_Char.h"
#include "c8051f340.h"
//#include <stdio.h>
//#include <string.h>


//----------------------------------------------------------------------------
// putchar
//----------------------------------------------------------------------------
//
// Function waits for empty output buffer and sends one byte
//
// Parameters   : c - byte to send
// Return Value : TRUE when byte writing's finished
//----------------------------------------------------------------------------

//char putchar (char c)
void putchar (char c)
{
   while (!TI0);
   TI0 = 0;
   /*return*/ (SBUF0 = c);
}

