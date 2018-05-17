//-----------------------------------------------------------------------------
// F34x_MSD_UART0.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file with function prototypes relevant to F34x_UART.c
//
//
// FID:            34X000058
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

#ifndef _UART0_H_
#define _UART0_H_

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void uart0_bauding(unsigned long baudrate);
void uart0_init(void);

#define UART0_TX_BUFFER_SIZE 32
#define UART0_RX_BUFFER_SIZE (0x100 - 32)

#include <c8051F340.h>
#include "fifo.h"

FIFO_SIZE_T uart0_write(char *buf, FIFO_SIZE_T size);
FIFO_SIZE_T uart0_read(char *buf, FIFO_SIZE_T size);
FIFO_SIZE_T uart0_tx_size();
FIFO_SIZE_T uart0_rx_size();

#ifdef MAIN_C

extern xdata fifo_char_t fifo_tx0;
extern xdata fifo_char_t fifo_rx0;

/* Interrupt(TI0 / RI0) */
void interrupt_uart0 (void) interrupt 4 {
  unsigned char c;
  
  if(RI0){
    RI0 = 0;
    /* リングバッファに1バイト書き出し */
    c = SBUF0;
    //if(fifo_char_write(&fifo_rx0, (char *)&c, 1) > 0){
    if(fifo_char_put(&fifo_rx0, (char *)&c)){
      P4 &= ~0x02;
    }else{P4 ^= 0x02;}
  }
  
  if(TI0){
    TI0 = 0;
    /* 書き込むデータがあるか確認 */
    if(fifo_char_get(&fifo_tx0, (char *)&c) > 0){
    	TB80 = 1; // TB80は書込み中フラグとして使う、1(書込み中)に
      SBUF0 = c;
    }else{
      TB80 = 0; // TB80は書込み中フラグとして使う、0(書込みしていない)に
    }
  }
}
#endif

/**
 * For stdio.h
 */
char getchar();
void putchar(char c);

#endif
