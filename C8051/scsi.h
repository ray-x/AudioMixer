//-----------------------------------------------------------------------------
// F34x_MSD_Scsi.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_Scsi.c
//
//
// FID:            34X000052
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

#ifndef _SCSI_H_
#define _SCSI_H_

#include "type.h"

#define SCSI_PASSED 		    0
#define SCSI_FAILED 		    1
#define SCSI_PHASE_ERROR 	2
#define SCSI_PENDING       0xFF

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void scsi_setup();
void scsi_ex();

extern BYTE xdata scsi_Status;
extern u32 xdata scsi_Residue;

extern u8 xdata scsi_lun;
extern u16 xdata scsi_block_size;

#endif
