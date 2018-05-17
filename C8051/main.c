//-----------------------------------------------------------------------------
// F34x_MSD_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// File contains the main loop of application. It also contains small delay 
// function and some simple initializations (CPU, Ports)
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000062
// Target:         C8051F34x
// Tool chain:     Keil
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_MSD
//
// REVISIONS:  11/22/02 - DM:  Added support for switches and sample USB
// interrupt application.
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
// Includes
//-----------------------------------------------------------------------------

#define MAIN_C

#include <stdio.h>
#include <c8051f340.h>

#include "main.h"
#include "f34x_USB_Register.h"
#include "diskio.h"
#include "USB_MSD.h"
#include "USB_CDC.h"
#include "f34x_USB_ISR.h"
#include "f34x_uart0.h"
#include "f34x_uart1.h"
#include "f34x_i2c.h"
#include "f34x_spi.h"

#include "servo_if.h"
#include "pitot_if.h"
#include "hmr3300_if.h"
#include "com_manager.h"
#include "gps.h"
#include "dsp.h"
#include "AD7739.h"
#include "util.h"
#include "uart1_device.h"

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
volatile xdata u32 global_ms = 0;
volatile xdata u32 tickcount = 0;
volatile unsigned char state = 0;

char test_data;
idata char test_idata;
const char test_const = 0;
xdata char test_xdata;

char * xdata test_data_ptr;
char * xdata test_idata_ptr;
char * xdata test_const_ptr;
char * xdata test_xdata_ptr;

// Initialization Routines
void Sys_Clk_Init(void);
void Port_Init(void);
void USB0_Init(void);
void Timer_Init(void);

//----------------------------------------------------------------------------
// main
//----------------------------------------------------------------------------
//
// Main loop
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void main(void) {
	PCA0MD &= ~0x40;                       // Disable Watchdog timer

	Sys_Clk_Init();                        // Initialize oscillator
	Port_Init();                           // Initialize crossbar and GPIO

	test_data_ptr = (char *)&test_data;
	test_idata_ptr = (char *)&test_idata;
	test_const_ptr = (char *)&test_const;
	test_xdata_ptr = (char *)&test_xdata;

	uart0_init();
  uart1_init();
  i2c_init();
  spi_init();
	disk_initialize(0);
	Timer_Init();
	USB0_Init();

  // speed up spi, 12MHz.
  spi_clock(12000);
  
  usb_bulk_init();
  ad7739_init();
  com_manager_init();
  dsp_init();
  
  EA = 1;                                // Global Interrupt enable

  gps_init();
  uart1_device_init();
  
  P4 &= ~0x01;
  
	while (1) {
	  gps_polling();
    ad7739_polling();
    dsp_polling();
    com_manager_polling();
		msd_polling();
    cdc_polling();
    servo_if_polling();
    pitot_if_polling();
    uart1_device_polling();
    usb_polling();
	}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Sys_Clk_Init
//----------------------------------------------------------------------------
//
// This routine initilizes the system clock and USB clock
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Sys_Clk_Init(void){
  REF0CN = 0x07;
  
#ifdef _USB_LOW_SPEED_

  OSCICN |= 0x03;                       // Configure internal oscillator for
                                         // its maximum frequency and enable
                                         // missing clock detector

  CLKSEL  = SYS_INT_OSC;                // Select System clock
  CLKSEL |= USB_INT_OSC_DIV_2;          // Select USB clock
#else
  OSCICN |= 0x03;                       // Configure internal oscillator for
                                         // its maximum frequency and enable
                                         // missing clock detector

  CLKMUL  = 0x00;                       // Select internal oscillator as 
                                         // input to clock multiplier

  CLKMUL |= 0x80;                       // Enable clock multiplier
  wait_ms(1);                           // Delay for clock multiplier to begin
  CLKMUL |= 0xC0;                       // Initialize the clock multiplier
  wait_ms(1);

  while(!(CLKMUL & 0x20));                // Wait for multiplier to lock
#ifdef __F340_VER__
#ifdef F340_24M
  CLKSEL = SYS_4X_DIV_2;
#else
  CLKSEL = SYS_4X_MUL;
#endif
#else
#ifdef __F326_VER__
  CLKSEL  = SYS_4X_DIV_2;
#else
  CLKSEL  = SYS_INT_OSC;                // Select system clock  
#endif
#endif
  CLKSEL |= USB_4X_CLOCK;               // Select USB clock
#endif  /* _USB_LOW_SPEED_ */
}


//----------------------------------------------------------------------------
// Port_Init
//----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Port_Init(void) {
  
  // for scl and sda.
  while((P1 & 0x03) != 0x03){
    XBR1 = 0x40;
    P1 &= ~0x02;
    while(P1 & 0x02);
    P1 |=  0x02;
    while(!(P1 & 0x02));
    XBR1 = 0x00;
  }
  
  
	// Default values on reset:
	// P0MDIN=0xFF;		P1MDIN=0xFF;	P2MDIN=0xFF;	P3MDIN=0xFF; 
	// P0MDOUT=0x00;	P1MDOUT=0x00;	P2MDOUT=0x00;	P3MDOUT=0x00;
	// P0SKIP=0x00;		P1SKIP=0x00;	P2SKIP=0x00;
	// XBR0=0x00;
  XBR0 = 0x07;   // UART0, SPI, I2C enabled
  XBR1 = (0x40 | 0x80); // Enable crossbar, disable weak pull-up
  XBR2 = 0x01;   // UART1 enabled
  
  // P0 -- 0-3 => SPI, 4 => TX0, 5=> RX0, 6 => -INT0, 7 => -INT1
  P0MDOUT = 0x1D; // enable TX0,SCK,MOSI as a push-pull
  P0SKIP = 0xC0;  // for -INT0 / -INT1
  IT01CF = 0x76;   // -INT0 => 6 negative, -INT1 => 7 negative 
  
  // P1 -- 0 => SDA, 1 => SCL, 5 => VREF(Analog)
  P1MDIN = 0xDF;
  P1MDOUT = 0xFC;
  P1 = 0x03;
  P1SKIP = 0xFC;
  
  // P2 -- 0-3 => pitot (0, 2, 3(OUT); 1(IN)), 5 => TX1, 6 => RX1
  P2MDIN = 0xFF; 
  P2MDOUT = 0xBD;
  P2 = 0x43;
  P2SKIP = 0x1F;
  
  // P3
  P3MDIN = 0xff;
  P3MDOUT = 0xFF;
  P3 = 0x00;
  
  // P4 -- 0,1,2,3 => LED, 4 => OBC_NSS, 5 => ADC_RX, 6 => ADC_TX, 7 => ADC_CLK  
  P4MDIN = 0xFF;
  P4MDOUT = 0xDF;   // as a push-pull output
  P4 = 0x30;
}


//----------------------------------------------------------------------------
// USB0_Init
//----------------------------------------------------------------------------
//
// USB Initialization
// - Initialize USB0
// - Enable USB0 interrupts
// - Enable USB0 transceiver
// - Enable USB0 with suspend detection
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void USB0_Init(void)
{
  POLL_WRITE_BYTE(POWER,  0x08);          // Force Asynchronous USB Reset
  POLL_WRITE_BYTE(IN1IE,  0x0F);          // Enable Endpoint 0-3 in interrupts
  POLL_WRITE_BYTE(OUT1IE, 0x0F);          // Enable Endpoint 0-3 out interrupts
  POLL_WRITE_BYTE(CMIE,   0x07);          // Enable Reset, Resume, and Suspend interrupts
#ifdef _USB_LOW_SPEED_
  USB0XCN = 0xC0;                         // Enable transceiver; select low speed
  POLL_WRITE_BYTE(CLKREC, 0xA0);          // Enable clock recovery; single-step mode
                                          // disabled; low speed mode enabled
#else                                      
  USB0XCN = 0xE0;                         // Enable transceiver; select full speed
  POLL_WRITE_BYTE(CLKREC, 0x80);          // Enable clock recovery, single-step mode
                                          // disabled
#endif /* _USB_LOW_SPEED_ */
}


//----------------------------------------------------------------------------
// Timer_Init
//----------------------------------------------------------------------------
//
// Timer initialization
// - Timer 3 reload, used to check if switch pressed on overflow and
// used for ADC continuous conversion
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Timer_Init(void)
{
  TMR3CN = 0x00;    // Stop Timer3; Clear TF3;

  CKCON &= ~0xB0;   // Timer3 clocked based on T3XCLK;
  TMR3RL = 0x63C0;  // Re-initialize reload value (100Hz, 10ms)
  TMR3 = 0xffff;    // Set to reload immediately

  EIE1 |= 0x80;     // Enable Timer3 interrupts(ET3)
  TMR3CN |= 0x04;   // Start Timer3(TR3)
}

/**
 * interrupt_timer3()
 * 
 */
void interrupt_timer3(void) interrupt 14 {
  static unsigned char tick_50ms = 0; 
  TMR3CN &= ~0x80; // Clear interrupt
  ad7739_start();
	global_ms += 10;
  tickcount++;
  {
    u8 tickcount_lowest_byte = (u8)tickcount;
    if((tickcount_lowest_byte & 0x01) == 0){ // 50Hz interrupt
      rw_servo_if = TRUE;
    }else{
      rw_pitot_if = TRUE;
    }
    if((tickcount_lowest_byte & 0x0F) == 0){ // 6.25Hz
      rw_hmr3300_if = TRUE;
      cdc_need_line_status_update = TRUE;
    }
  }
  {
    static u8 loop = 0;
    switch(++loop){
      case 5:
        P4 &= ~0x08;
        break;
      case 25:
        if(state & STATE_RX_FROM_AD7739) P4 |= 0x08;
        state &= ~STATE_RX_FROM_AD7739;
        break;
      case 30:
        P4 &= ~0x08;
        break;
      case 50:
        if(state & STATE_RX_FROM_DSP) P4 |= 0x08;
        state &= ~STATE_RX_FROM_DSP;
        break;
      case 55:
        P4 &= ~0x08;
        break;
      case 75:
        if(state & STATE_WRITTEN_LOG) P4 |= 0x08;
        state &= ~STATE_WRITTEN_LOG;
        break;
      case 80:
        P4 &= ~0x08;
        break;  
      case 200:
        P4 |= 0x08;
        if(P1 & 0x04){
          state |= STATE_LOG_ON;
        }else{
          state &= ~STATE_LOG_ON;
        }
        loop = 0;
        break; 
    }
    if(loop % 5 == 0){
      static unsigned char sats = 0;
      if(++tick_50ms == 200){
        tick_50ms = 0;
        if(gps_num_of_sat < MAX_SAT){
          sats = gps_num_of_sat;
        }
      }
      if(sats > 0){
        switch(tick_50ms % 5){
          case 0:
            P4 |= 0x04;
            break;
          case 1:
            sats--;
            P4 &= ~0x04;
            break;
        }
      }
    }
  }
}

//----------------------------------------------------------------------------
// _sdcc_external_startup
//----------------------------------------------------------------------------

unsigned char _sdcc_external_startup(void)
{
	PCA0MD &= ~0x40;                       // Disable Watchdog timer
  return 0;
}
