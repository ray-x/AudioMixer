//-----------------------------------------------------------------------------
// F34x_MSD_CF_Basic_Functions.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_CF_Basic_Functions.c
//
//
// FID:            34X000029
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
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef __CF_BASIC_H__
#define __CF_BASIC_H__

//#include <intrins.h>
#include "c8051f340.h"

#define		CF_NO_CARD		0x01
sbit at 0x90 CF_RST;
sbit at 0x91 CF_OE;
sbit at 0x92 CF_CE1;
sbit at 0x93 CF_CD1;
sbit at 0x94 CF_RDY;
sbit at 0x96 CF_WE;


//-----------------------------------------------------------------------------
// Macro Prototypes
//-----------------------------------------------------------------------------

#define CF_SET_ADDR_LINE(value) {\
	P3 &= ~0xe0;\
	P3 |= (value << 5) & 0xe0;\
}

#define CF_WR_DATA(value) {\
	P4MDOUT = 0xFF;\
	P4 = value;\
}

#define CF_READ_DATA	P4

#define CF_WAIT_STATE {\
	_nop_ ();\
	_nop_ ();\
	_nop_ ();\
}

#define CF_READWRITE_IDLE {\
	P4MDOUT = 0x00;\
	P4 =0xff;\
	CF_OE = 1;\
	CF_WE = 1;\
}

#define CF_WRITE {\
	CF_OE = 1;\
	CF_WE = 0;\
}

#define CF_READ {\
	P4MDOUT = 0x00;\
	P4 =0xff;\
	CF_WE = 1;\
	CF_OE = 0;\
}


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

char Dtct_Card(void);
void Wait_Drq();
void Wait_Busy();
char Identify_Drive(char* buffer);
char Init_CF();
//char Write_Sector(unsigned long address, char* buffer);
char Write_Sector(void);
char Read_Sector(void);

void _nop_(void) _naked;

xdata unsigned long _CF_Sector_Address;
xdata char* xdata _CF_Sector_Buffer;

//-----------------------------------------------------------------------------
// Macro Prototypes
//-----------------------------------------------------------------------------

//#define MACRO_CF_VERSIONS
#ifndef MACRO_CF_VERSIONS

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

char CF_GET_REGISTER_VALUE(char addr);
void CF_SET_REGISTER_VALUE(char addr, char value);

#else
#define CF_GET_REGISTER_VALUE(addr,value) {\
	CF_SET_ADDR_LINE(addr);\
	CF_READ;\
	CF_WAIT_STATE;\
	value = CF_READ_DATA;\
	CF_READWRITE_IDLE;\
}
#define CF_SET_REGISTER_VALUE(addr,value) {\
	CF_SET_ADDR_LINE(addr);\
	CF_WR_DATA(value);\
	CF_WRITE;\
	CF_WAIT_STATE;\
	CF_READWRITE_IDLE;\
}
#endif

#endif
