//-----------------------------------------------------------------------------
// F34x_MSD_USB_Descriptor.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file for USB firmware.  Defines standard
// descriptor structures.
//
//
// FID:            34X000060
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

#ifndef  _USB_DESC_H_
#define  _USB_DESC_H_

#include "type.h"

//#define DEBUG_MSD_ONLY
//#define DEBUG_CDC_ONLY
#define USE_IAD

#ifndef DEBUG_CDC_ONLY
#ifndef DEBUG_MSD_ONLY
#define USB_INTERFACES 3
#else
#define USB_INTERFACES 2
#endif
#else
#define USB_INTERFACES 1
#endif


/**
 * Standard Device Descriptor Type Defintion
 */
typedef struct {
   BYTE bLength;                // Size of this Descriptor in Bytes
   BYTE bDescriptorType;        // Descriptor Type (=1)
   WORD bcdUSB;                 // USB Spec Release Number in BCD
   BYTE bDeviceClass;           // Device Class Code
   BYTE bDeviceSubClass;        // Device Subclass Code	
   BYTE bDeviceProtocol;        // Device Protocol Code
   BYTE bMaxPacketSize0;        // Maximum Packet Size for EP0 
   WORD idVendor;               // Vendor ID 
   WORD idProduct;              // Product ID
   WORD bcdDevice;              // Device Release Number in BCD
   BYTE iManufacturer;          // Index of String Desc for Manufacturer
   BYTE iProduct;               // Index of String Desc for Product
   BYTE iSerialNumber;          // Index of String Desc for SerNo
   BYTE bNumConfigurations;     // Number of possible Configurations
} device_descriptor_t;          // End of Device Descriptor Type

/**
 * Standard Configuration Descriptor Type Definition
 */
typedef struct {
   BYTE bLength;                // Size of this Descriptor in Bytes
   BYTE bDescriptorType;        // Descriptor Type (=2)
   WORD wTotalLength;           // Total Length of Data for this Conf
   BYTE bNumInterfaces;         // No of Interfaces supported by this Conf
   BYTE bConfigurationValue;    // Designator Value for *this* Configuration
   BYTE iConfiguration;         // Index of String Desc for this Conf
   BYTE bmAttributes;           // Configuration Characteristics (see below)
   BYTE bMaxPower;              // Max. Power Consumption in this Conf (*2mA)
} configuration_descriptor_t;   // End of Configuration Descriptor Type

/**
 * Standard Interface Descriptor Type Definition
 */
typedef struct {
   BYTE bLength;                // Size of this Descriptor in Bytes
   BYTE bDescriptorType;        // Descriptor Type (=4)
   BYTE bInterfaceNumber;       // Number of *this* Interface (0..)
   BYTE bAlternateSetting;      // Alternative for this Interface (if any)
   BYTE bNumEndpoints;          // No of EPs used by this IF (excl. EP0)
   BYTE bInterfaceClass;        // Interface Class Code
   BYTE bInterfaceSubClass;     // Interface Subclass Code
   BYTE bInterfaceProtocol;     // Interface Protocol Code
   BYTE iInterface;             // Index of String Desc for this Interface
} interface_descriptor_t;       // End of Interface Descriptor Type

/**
 * Standard Endpoint Descriptor Type Definition
 */
typedef struct {
   BYTE bLength;                // Size of this Descriptor in Bytes
   BYTE bDescriptorType;        // Descriptor Type (=5)
   BYTE bEndpointAddress;       // Endpoint Address (Number + Direction)
   BYTE bmAttributes;           // Endpoint Attributes (Transfer Type)
   WORD wMaxPacketSize;	        // Max. Endpoint Packet Size
   BYTE bInterval;              // Polling Interval (Interrupt) ms
} endpoint_descriptor_t;        // End of Endpoint Descriptor Type

typedef struct {
  BYTE bLength;                // Size of this Descriptor in Bytes
  BYTE bDescriptorType;        // Descriptor Type (=11)
  BYTE bFirstInterface;        // 
  BYTE bInterfaceCount;        // 
  BYTE bFunctionClass;         // Function Class Code
  BYTE bFunctionSubClass;      // Function Subclass Code 
  BYTE bFunctionProtocol;      // Function Protocol Code
  BYTE iInterface;             // Index of String Desc for this function
} interface_association_descriptor_t;

#define config_length_total(n_interfaces, n_endpoints) \
(sizeof(configuration_descriptor_t)  \
   + (sizeof(interface_descriptor_t) * n_interfaces) \
   + (sizeof(endpoint_descriptor_t) * n_endpoints))
#define iad_length() sizeof(interface_association_descriptor_t)

// Standard Descriptor Types  @see USB spec. chapter.9
#define DSC_TYPE_DEVICE     0x01        // Device Descriptor
#define DSC_TYPE_CONFIG     0x02        // Configuration Descriptor
#define DSC_TYPE_STRING     0x03        // String Descriptor
#define DSC_TYPE_INTERFACE  0x04        // Interface Descriptor
#define DSC_TYPE_ENDPOINT   0x05        // Endpoint Descriptor
#define DSC_TYPE_IASSOC     0x0B        // Interface Association Descriptor

// Endpoint transfer type
#define DSC_EP_CONTROL      0x00
#define DSC_EP_ISOC         0x01
#define DSC_EP_BULK         0x02
#define DSC_EP_INTERRUPT    0x03

// HID Descriptor Types
#define DSC_HID         0x21   // HID Class Descriptor
#define DSC_HID_REPORT  0x22   // HID Report Descriptor

// HID Request Codes
#define GET_REPORT      0x01  // Code for Get Report
#define GET_IDLE        0x02   // Code for Get Idle
#define GET_PROTOCOL    0x03   // Code for Get Protocol
#define SET_REPORT      0x09   // Code for Set Report
#define SET_IDLE        0x0A   // Code for Set Idle
#define SET_PROTOCOL    0x0B   // Code for Set Protocol

// These are created in F34x_USB_Descriptor.c
extern const device_descriptor_t DESC_DEVICE;
extern const configuration_descriptor_t DESC_CONFIG;
extern code BYTE * code DESC_STRINGs[4];

#if (!defined(DEBUG_MSD_ONLY)) && (!defined(DEBUG_CDC_ONLY))
extern const device_descriptor_t DESC2_DEVICE;
extern const configuration_descriptor_t DESC2_CONFIG;

#endif

#endif  /* _USB_DESC_H_ */


