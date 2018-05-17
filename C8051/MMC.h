//-----------------------------------------------------------------------------
// F34x_MSD_MMC.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_MMC.c
//
//
// FID:            34X000044
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

#ifndef _MMC_H_
#define _MMC_H_

// MMC_FLASH Functions

#define MMC_NORMAL_CODE 0x00
#define MMC_ERROR_CODE  0xFF

// Physical size in bytes of one MMC FLASH sector
#define PHYSICAL_BLOCK_SIZE     512 

extern bit MMC_initialized;
extern xdata unsigned long MMC_physical_size;
extern xdata unsigned long MMC_physical_sectors;

// Initializes MMC and configures it to accept SPI commands;
void MMC_FLASH_Init();
unsigned char MMC_Flush();

unsigned char MMC_FLASH_Block_Read(unsigned long address, unsigned char *pchar);
unsigned char MMC_FLASH_Block_Write(unsigned long address, unsigned char *wdata);
unsigned char MMC_Get_Status();

#endif

