//-----------------------------------------------------------------------------
// F34x_MSD_Get_Char.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains function for receiving data.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000039
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
#include "F34x_MSD_Get_Char.h"
#include "c8051f340.h"
//#include <stdio.h>

//----------------------------------------------------------------------------
// getchar
//----------------------------------------------------------------------------
//
// This function waits for input interrupt flag and receives the byte.
//
// Parameters   :
// Return Value : recieved byte
//----------------------------------------------------------------------------

char getchar (void)
{
  unsigned char r;
  
  do {
    if (RI0 && !RB80)
      RI0 = 0;
  } while (!RI0);

  r = SBUF0;
  RI0 = 0;
  return r;
}


