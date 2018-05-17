//-----------------------------------------------------------------------------
// F34x_MSD_Basic_Functions.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Module contains basic functions for Compact Flash card.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000028
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
// Includes
//-----------------------------------------------------------------------------

#include "F34x_MSD_Physical_Settings.h"
#include "F34x_MSD_CF_Basic_Functions.h"
#include <stdio.h>

sbit at 0xa2 Led1;
sbit at 0xa3 Led2;
void CF_Port_Init();

//----------------------------------------------------------------------------
// Init_CF
//----------------------------------------------------------------------------
//
// This routine initializes the Compact Flash memory card
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

char Init_CF()
{
  /*xdata*/ char result = Dtct_Card(); 
  /*xdata*/ unsigned int loop = 0;
  /*xdata*/ unsigned reinit = 0;
  CF_OE = 0;
  for(loop=0;loop<5000;i++) Delay();
  CF_OE = 1;
  if (result == 1) return (CF_NO_CARD); 
REINIT:
  CF_RST = 1;
  while(++loop);
  CF_WAIT_STATE;
  CF_WAIT_STATE;
  CF_WAIT_STATE;
  CF_WAIT_STATE;
  CF_RST = 0;
  while(!CF_RDY) {
    if(++loop == 0) {
      CF_GET_REGISTER_VALUE(0x01);
      goto REINIT;
    }
  }
  while(++loop);
  if((CF_GET_REGISTER_VALUE(0x01) & 0xf0) != 0)
    goto REINIT;
  return 0;
}


//----------------------------------------------------------------------------
// Wait_Drq
//----------------------------------------------------------------------------
//
// This function waits until data reqeust bit in staus register is clear
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Wait_Drq()
{
  /*xdata*/ char wait, wait_drq;
  /*xdata*/ int loopguard = 0;
  do {
    #ifndef MACRO_CF_VERSIONS
    wait = CF_GET_REGISTER_VALUE(0x07);	// Read status register
    #else
    CF_GET_REGISTER_VALUE(0x07,wait);	// Read status register
    #endif
    if(wait & 0x01) {
      wait_drq = CF_GET_REGISTER_VALUE(0x01);
      return;
    }
    wait_drq = wait & 0x08;		// Wait for DRQ to SET
  } while (wait_drq == (char)0x00);
}


//----------------------------------------------------------------------------
// Wait_Busy
//----------------------------------------------------------------------------
//
// This function waits until busy bit is set
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Wait_Busy()
{
  /*xdata*/ char wait, wait_bsy;
  do {
    #ifndef MACRO_CF_VERSIONS
    wait = CF_GET_REGISTER_VALUE(0x07);	// Read status register
    #else
    CF_GET_REGISTER_VALUE(0x07,wait);	// Read status register
    #endif
    wait_bsy = wait & 0x80;		// Wait for DRQ to SET
  } while (wait_bsy == (char)0x80);
}


//----------------------------------------------------------------------------
// Dtct_Card
//----------------------------------------------------------------------------
//
// Function detcts if compact flash card is in slot
//
// Parameters   :
// Return Value : 0 card is on , 1 off
//----------------------------------------------------------------------------

char Dtct_Card(void)
{
  /*xdata*/ char card_detect;
  card_detect = CF_CD1; 
  return (card_detect); // 0 card is on , 1 off
}

#ifndef MACRO_CF_VERSIONS

//----------------------------------------------------------------------------
// CF_GET_REGISTER_VALUE
//----------------------------------------------------------------------------
//
// Function returns value from register pointed by addr
//
// Parameters   : addr - address value of CF register
// Return Value : value stroed in register
//----------------------------------------------------------------------------

char CF_GET_REGISTER_VALUE(char addr)
{
  /*xdata*/ char ret = 0;
  CF_SET_ADDR_LINE(addr);
  CF_READ;
  CF_WAIT_STATE;
  ret = CF_READ_DATA;
  CF_READWRITE_IDLE;
  return ret;
}


//----------------------------------------------------------------------------
// CF_SET_REGISTER_VALUE
//----------------------------------------------------------------------------
//
// Function sets value of register
//
// Parameters   : addr - register address
//				  value - value to set
// Return Value : 
//----------------------------------------------------------------------------

void CF_SET_REGISTER_VALUE(char addr, char value)
{
  CF_SET_ADDR_LINE(addr);
  CF_WR_DATA(value);
  CF_WRITE;
  CF_WAIT_STATE;
  CF_READWRITE_IDLE;
}
#endif

//----------------------------------------------------------------------------
// Write_Sector
//----------------------------------------------------------------------------
//
// Function writes buffer into address
//
// Parameters   : address - LBA address
//                buffer - buffer with data to write
// Return Value : return error status or 0 if everything was OK
//----------------------------------------------------------------------------

char Write_Sector(void)
{
  /*xdata*/ char cylinder_low, cylinder_high,sector,head;
  /*xdata*/ char result, error;
  /*xdata*/ int i;
  result = Dtct_Card(); 
  if (result == 1) return (CF_NO_CARD); 
	
  Led2=1;
  sector = _CF_Sector_Address & 0xff;
  cylinder_low = (_CF_Sector_Address >> 8 ) & 0x0FF; 
  cylinder_high = (_CF_Sector_Address >> 16) & 0xff; 
  head = (_CF_Sector_Address >> 24) & 0x0f;

	
  CF_SET_REGISTER_VALUE(2,0x01);
  CF_SET_REGISTER_VALUE(3,sector);
  CF_SET_REGISTER_VALUE(4,cylinder_low);
  CF_SET_REGISTER_VALUE(5,cylinder_high);
  CF_SET_REGISTER_VALUE(6,(0xE0 | head));
  CF_SET_REGISTER_VALUE(7,0x30);

  error = CF_GET_REGISTER_VALUE(1);

  Wait_Drq(); // Wait for DRQ
  Wait_Busy(); 

  for(i=0;i<PHYSICAL_BLOCK_SIZE;i++) {
    CF_SET_REGISTER_VALUE(0,_CF_Sector_Buffer[i]);
  }

  Wait_Busy(); 
  #ifndef MACRO_CF_VERSIONS
  error = CF_GET_REGISTER_VALUE(1);
  #else
  CF_GET_REGISTER_VALUE(1,error);
  #endif
  Led2=0;
  return(error); 
}


//----------------------------------------------------------------------------
// Read_Sector
//----------------------------------------------------------------------------
//
// Function reads buffer from address
//
// Parameters   : address - LBA address
//                buffer - buffer with data to write
// Return Value : return error status or 0 if everything was OK
//----------------------------------------------------------------------------

char Read_Sector(void)
{
  /*xdata*/ char cylinder_low, cylinder_high,sector,head;
  /*xdata*/ char result, error;
  /*xdata*/ int i;
  result = Dtct_Card(); 
  if (result == 1) return (CF_NO_CARD); 
  Led1 = 1;
  sector = _CF_Sector_Address & 0xff;
  cylinder_low = (_CF_Sector_Address >> 8 ) & 0x0FF; 
  cylinder_high = (_CF_Sector_Address >> 16) & 0xff; 
  head = (_CF_Sector_Address >> 24) & 0x0f;

  error = CF_GET_REGISTER_VALUE(1);
	
  CF_SET_REGISTER_VALUE(2,0x01);
  CF_SET_REGISTER_VALUE(3,sector);
  CF_SET_REGISTER_VALUE(4,cylinder_low);
  CF_SET_REGISTER_VALUE(5,cylinder_high);
  CF_SET_REGISTER_VALUE(6,(0xE0 | head));
  CF_SET_REGISTER_VALUE(7,0x20);

  error = CF_GET_REGISTER_VALUE(1);

  Wait_Drq(); // Wait for DRQ

  #ifndef MACRO_CF_VERSIONS
  error = CF_GET_REGISTER_VALUE(1);
  #else
  CF_GET_REGISTER_VALUE(1,error);
  #endif

		

  for(i=0;i<PHYSICAL_BLOCK_SIZE;i++) {
#ifndef MACRO_CF_VERSIONS
    _CF_Sector_Buffer[i] = CF_GET_REGISTER_VALUE(0);
#else
    CF_GET_REGISTER_VALUE(0,_CF_Sector_Buffer[i]);
#endif
  }
  Wait_Busy();
  #ifndef MACRO_CF_VERSIONS
  error = CF_GET_REGISTER_VALUE(1);
  #else
  CF_GET_REGISTER_VALUE(1,error);
  #endif
  Led1 = 0;
  return(error); 
}

//----------------------------------------------------------------------------
// Identify_Drive
//----------------------------------------------------------------------------
//
// This function reads identifier from card
//
// Parameters   : buffer - buffer on data from CF
// Return Value : error number if occurs 
//----------------------------------------------------------------------------

char Identify_Drive(char* buffer)
{
  /*xdata*/ char error,result;
  /*xdata*/ int i;
	
  result = Dtct_Card();
  if (result == 1) return (CF_NO_CARD); 

  while(((error = CF_GET_REGISTER_VALUE(1)) & 0xF0) != 0);
			

  CF_SET_REGISTER_VALUE(6,0);
  CF_SET_REGISTER_VALUE(7,0xEC);

  error = CF_GET_REGISTER_VALUE(1);

	
  Wait_Drq();
  for (i = 0; i < PHYSICAL_BLOCK_SIZE; i++) {
    #ifndef MACRO_CF_VERSIONS
    *(buffer + i) = CF_GET_REGISTER_VALUE(0);
    #else
    CF_GET_REGISTER_VALUE(0,*(buffer + i));
    #endif
  }
  #ifndef MACRO_CF_VERSIONS
  error = CF_GET_REGISTER_VALUE(1);
  #else
  CF_GET_REGISTER_VALUE(1,error);
  #endif
  return error; 
}

//----------------------------------------------------------------------------
// Just Nop
//----------------------------------------------------------------------------
void _nop_(void) _naked
{
_asm
	nop
_endasm;
}
