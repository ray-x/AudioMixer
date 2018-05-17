//-----------------------------------------------------------------------------
// F34x_MSD_USB_Register.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file for USB firmware. Includes all USB core register
// addresses, register access macros, and register bit masks.
//
// FID:            34X000065
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
// 11/22/02 - DM: 1. Updated function prototypes and added constants
//                to F34x_USB_Main.h with sample interrupt firmware.

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef  _USB_ISR_H_
#define  _USB_ISR_H_

#include <c8051F340.h>
#include <string.h>
#include "main.h"

// Device Request Direction (bmRequestType bit7)
#define DRD_MASK    0x80    // Mask for device request direction
#define DRD_OUT     0x00    // OUT: host to device
#define DRD_IN      0x80    // IN:  device to host

// Device Request Type (bmRequestType bit 6-5)
#define DRT_MASK    0x60    // Mask for device request type
#define DRT_STD     0x00    // Standard device request
#define DRT_CLASS   0x20    // Class specific request
#define DRT_VENDOR  0x40    // Vendor specific request

// Device Request Recipient (bmRequestType bit4-0)
#define DRR_MASK      0x1F    // Mask for device request recipient
#define DRR_DEVICE    0x00    // Device
#define DRR_INTERFACE 0x01    // Interface
#define DRR_ENDPOINT  0x02    // Endpoint

// Define bmRequestType bitmaps
#define OUT_DEVICE    (DRD_OUT | DRT_STD | DRR_DEVICE)
#define IN_DEVICE     (DRD_IN  | DRT_STD | DRR_DEVICE)
#define OUT_INTERFACE (DRD_OUT | DRT_STD | DRR_INTERFACE)
#define IN_INTERFACE  (DRD_IN  | DRT_STD | DRR_INTERFACE)
#define OUT_ENDPOINT  (DRD_OUT | DRT_STD | DRR_ENDPOINT)
#define IN_ENDPOINT   (DRD_IN  | DRT_STD | DRR_ENDPOINT)

#define OUT_CL_INTERFACE  (DRD_OUT | DRT_CLASS | DRR_INTERFACE)
#define IN_CL_INTERFACE   (DRD_IN  | DRT_CLASS | DRR_INTERFACE)

#define OUT_VR_INTERFACE  (DRD_OUT | DRT_VENDOR | DRR_INTERFACE)
#define IN_VR_INTERFACE   (DRD_IN  | DRT_VENDOR | DRR_INTERFACE)

/**
 * Setup Packet Type Definition
 */
typedef struct {
   BYTE bmRequestType;          // Request recipient, type, and direction
   BYTE bRequest;               // Specific standard request number
   WORD wValue;                 // varies according to request
   WORD wIndex;                 // varies according to request
   WORD wLength;                // Number of bytes to transfer
} setup_buffer_t;               // End of Setup Packet Type

// Buffer for current device request information
extern xdata setup_buffer_t usb_setup_buf;

extern BYTE* ep0_Data_Ptr;
extern xdata unsigned int ep0_Data_Size;
extern void xdata (*ep0_callback)();

#define regist_data(target, size) \
{ \
  ep0_Data_Ptr = target; \
  ep0_Data_Size = size; \
  ep0_callback = NULL; \
}
#define reserve_data(target, size, call_back_func) \
{ \
  ep0_Data_Ptr = target; \
  ep0_Data_Size = size; \
  ep0_callback = call_back_func; \
}

extern BYTE xdata usb_ep_stat[7];
#define DIR_IN 0x00
#define DIR_OUT 0x04
#define usb_ep0_status usb_ep_stat[0]
#define usb_ep_status(dir, ep_number) \
usb_ep_stat[((dir == DIR_OUT) ? (3 + ep_number) : (ep_number))]

// Determines current USB device state
extern xdata BYTE usb_state;

extern volatile bit usb_request_completed;

unsigned int usb_write(BYTE* ptr_buf, unsigned int count, unsigned char index);
unsigned int usb_fill(BYTE buf, unsigned int count, unsigned char index);
unsigned int usb_read(BYTE* ptr_buf, unsigned int count, unsigned char index);
unsigned int usb_skip(unsigned int count, unsigned char index);
void usb_flush(unsigned char index);
unsigned int usb_count_ep_out(unsigned char index);

void usb_status_lock(unsigned char dir, unsigned char ep_index);
void usb_status_unlock(unsigned char dir, unsigned char ep_index);
void usb_stall(unsigned char dir, unsigned char ep_index, code void(*)());
void usb_clear_halt(unsigned char dir, unsigned char ep_index);
void usb_bulk_init();

extern volatile bit usb_enable;
extern volatile bit usb_previous_enable;

void usb_polling();

void usb_isr() interrupt 8;

#define CRITICAL_USB0(func) \
{ \
  EIE1 &= ~(0x02); \
  { \
    func; \
  } \
  EIE1 |= (0x02); \
}

#endif /* _USB_ISR_H_ */
