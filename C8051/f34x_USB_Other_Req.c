//-----------------------------------------------------------------------------
// F34x_MSD_USB_Std_Req.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This source file contains the subroutines used to handle incoming setup 
// packets. These are called by handle_setup in F34x_USB_ISR.c and used for
// USB chapter 9 compliance.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000066
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

#include "c8051f340.h"
#include "f34x_USB_Register.h"
#include "main.h"
#include "f34x_USB_ISR.h"
#include "f34x_USB_Other_Req.h"
#include "USB_CDC.h"
#include "USB_MSD.h"
#include "f34x_USB_Descriptor.h"

void usb_Class_init(){
  usb_MSD_init();
}

void usb_Class_Req(){
  switch(usb_setup_buf.wIndex.i){
    case 0:
#ifndef DEBUG_MSD_ONLY
#ifndef CDC_IS_REPLACED_BY_FTDI
      usb_CDC_req();
      break;
    case 2:
#else
    case 1:
#endif
#endif
      usb_MSD_req();
      break;
  }
}

void usb_Vendor_Req(){
#ifdef CDC_IS_REPLACED_BY_FTDI 
  usb_CDC_req();
#endif
}
