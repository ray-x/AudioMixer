#ifndef _MAIN_H_
#define _MAIN_H_

#include "type.h"

//#define _USB_LOW_SPEED_                      // Change this comment to make Full/Low speed

// SYSCLK frequency in Hz
#ifdef F340_24M
#define SYSCLK    24000000UL
#else
#define SYSCLK    48000000UL
#endif


// USB clock selections (SFR CLKSEL)
#define USB_4X_CLOCK             0x00        // Select 4x clock multiplier, for USB Full Speed
#define USB_INT_OSC_DIV_2        0x10        // See Data Sheet section 13. Oscillators
#define USB_EXT_OSC              0x20
#define USB_EXT_OSC_DIV_2        0x30
#define USB_EXT_OSC_DIV_3        0x40
#define USB_EXT_OSC_DIV_4        0x50

// System clock selections (SFR CLKSEL)
#define SYS_INT_OSC              0x00        // Select to use internal oscillator
#define SYS_4X_MUL               0x03        // Select to use internal oscillator
#define SYS_EXT_OSC              0x01        // Select to use an external oscillator
#define SYS_4X_DIV_2             0x02

extern volatile xdata u32 global_ms;
extern volatile xdata u32 tickcount;

extern volatile unsigned char state;
#define STATE_LOG_ON          0x01
#define STATE_RX_FROM_AD7739  0x02
#define STATE_RX_FROM_DSP     0x04
#define STATE_WRITTEN_LOG     0x08
#define STATE_RW_SERVO_IF     0x10
#define STATE_RW_PITOT_IF     0x20
#define STATE_USB_ACTIVE      0x80

// Define Endpoint Packet Sizes
#ifdef _USB_LOW_SPEED_
// This value can be 8,16,32,64 depending on device speed, see USB spec
#define PACKET_SIZE_EP0     0x40
#else
#define PACKET_SIZE_EP0     0x40
#endif /* _USB_LOW_SPEED_ */ 

// Can range 0 - 1024 depending on data and transfer type
#define PACKET_SIZE_EP1 0x0010
#define PACKET_SIZE_EP2 0x0040
#define PACKET_SIZE_EP3 0x0040

// Define device states
#define DEV_ATTACHED        0x00        // Device is in Attached State
#define DEV_POWERED         0x01        // Device is in Powered State
#define DEV_DEFAULT         0x02        // Device is in Default State
#define DEV_ADDRESS         0x03        // Device is in Addressed State
#define DEV_CONFIGURED      0x04        // Device is in Configured State
#define DEV_SUSPENDED       0x05        // Device is in Suspended State

// Define wIndex bitmaps
#define is_EP_IN(ep_address) ((ep_address) & (0x80))
#define EP_NUMBER(ep_address) ((ep_address) & ~(0x80))
#define IN_EP1              0x81        // Index values used by Set and Clear feature
#define OUT_EP1             0x01        // commands for Endpoint_Halt
#define IN_EP2              0x82
#define OUT_EP2             0x02
#define IN_EP3              0x83
#define OUT_EP3             0x03

// Define wValue bitmaps for Standard Feature Selectors
#define DEVICE_REMOTE_WAKEUP    0x01        // Remote wakeup feature(not used)
#define ENDPOINT_HALT           0x00        // Endpoint_Halt feature selector

// Define Endpoint States
#define EP_IDLE                 0x00        // This signifies Endpoint Idle State
#define EP_TX                   0x01        // Endpoint Transmit State
#define EP_RX                   0x02        // Endpoint Receive State
#define EP_HALT                 0x03        // Endpoint Halt State (return stalls)
#define EP_STALL                0x04        // Endpoint Stall (send procedural stall next status phase)
#define EP_ADDRESS              0x05        // Endpoint Address (change FADDR during next status phase)

#define EP_OWNED_BY_USER        0x80        // Endpoint state owned by user
#define ep_strip_owner(state) ((state) & ~EP_OWNED_BY_USER)

#define CRITICAL_GLOBAL(func) \
{ \
  EA = 0; \
  { \
    func; \
  } \
  EA = 1; \
}

#define ENDLINE   "\r\n"
#define ENDLINE_SGN '\r'

#endif      /* _USB_MAIN_H_ */

