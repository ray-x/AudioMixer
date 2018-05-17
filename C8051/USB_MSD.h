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
// FID:            34X000047
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

#ifndef _MSD_H_
#define _MSD_H_

#include "type.h"

typedef struct {
	DWORD dCBWSignature;
	DWORD dCBWTag;
	DWORD dCBWDataTransferLength;
	BYTE  bmCBWFlags;
	BYTE  bCBWLUN;
	BYTE  bCBWCBLength;
	BYTE  CBWCB[16];
} msd_cbw_t;

typedef struct {
	DWORD dCSWSignature;
	DWORD dCSWTag;
	DWORD dCSWDataResidue;
	BYTE  bCSWStatus;
} msd_csw_t;

extern msd_cbw_t xdata msd_cbw;
extern msd_csw_t xdata msd_csw;

#define MSD_EP_IN  3
#define MSD_EP_OUT 3

#define CONCAT2_(a,b) (a ## b)
#define CONCAT2(a,b) CONCAT2_(a, b) 

#define MSD_EP_IN_PACKET_SIZE  (CONCAT2(PACKET_SIZE_EP, MSD_EP_IN))
#define MSD_EP_OUT_PACKET_SIZE  (CONCAT2(PACKET_SIZE_EP, MSD_EP_OUT))

#define MSD_RESET           0xFF    // Mass-storage device Reset
#define MSD_GET_MAX_LUN     0xFE    // Mass-storage device Get Max LUN

void msd_polling();

void usb_MSD_init();
void usb_MSD_req();

extern u8 xdata msd_action;
#define MSD_HOST_NO_DATA      0x01
#define MSD_HOST_TX           0x02
#define MSD_HOST_RX           0x04
#define MSD_DEVICE_NO_DATA    0x10
#define MSD_DEVICE_RX         0x20
#define MSD_DEVICE_TX         0x40

#define MSD_HOST_SIDE(action) (action & 0x0F)
#define MSD_DEVICE_SIDE(action) (action & 0xF0)

#endif
