//-----------------------------------------------------------------------------
// F34x_MSD_Util.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file for F34x_Util.h. It contains functions and variables 
// prototypes.
//
//
// FID:            34X000068
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

#ifndef _UTIL_H_
#define _UTIL_H_

#include "main.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void wait_10n4clk(unsigned char i);
void wait_us(unsigned int count);
void wait_ms(unsigned int count);

#ifdef ENDIAN_CORRECT_BY_FUNC
u32 swap_u32(u32 dw);
u16 swap_u16(u16 w);
#else
#define swap_u32(dw) \
( (u32)(((u32)dw & 0x000000FF) << 24) \
  | (((u32)dw & 0x0000FF00) << 8) \
  | (((u32)dw & 0x00FF0000) >> 8) \
  | (((u32)dw & 0xFF000000) >> 24) \
)
#define swap_u16(w) \
( (u16)(((u16)w & 0x00FF) << 8) \
  | (((u16)w & 0xFF00) >> 8) \
)
#endif


#ifdef __SDCC__
#define le_u32(dw) (dw)
#define le_u16(w) (w)
#define be_u32(dw) swap_u32(dw)
#define be_u16(w) swap_u16(w)
#else
#define le_u32(dw) swap_u32(dw)
#define le_u16(w) swap_u16(w)
#define be_u32(dw) (dw)
#define be_u16(w) (w)

#endif

//-----------------------------------------------------------------------------
// Variable Prototype
//-----------------------------------------------------------------------------

char* Str_Token(char* str);

//-----------------------------------------------------------------------------
// Macros Prototypes
//-----------------------------------------------------------------------------

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

u16 crc16(u8 *buf, u8 size, u16 crc);

#endif
