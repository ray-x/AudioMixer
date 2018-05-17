/**
 * HMR3300 support routines.
 *  by fenrir.
 */

#include <c8051f340.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "type.h"
#include "f34x_uart1.h"
#include "com_manager.h"
#include "uart1_device.h"

#include "hmr3300_if.h"

#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

volatile bit rw_hmr3300_if = FALSE;

/*
 * uart1_buf‚ðŽŸ‚Ì‚æ‚¤‚ÉŽg‚¤
 * 'M', 0, 0, tickcount & 0xFF, // 0-3 
 * global_ms, // 4-7
 * [X(MSB,LSB), Y(MSB,LSB), Z(MSB,LSB)] * 4
 */

static void make_packet(packet_t *packet){
  // packet‚Ì“o˜^
  if(packet->current < packet->buf_end){
    
    memcpy(packet->current, uart1_buf, sizeof(uart1_buf));
    
    packet->current += 3;
    *((packet->current)++) = tickcount & 0xFF;
    
    // Žž‚Ì“o˜^
    {
      // LSB first
      *((u32 *)(packet->current)) = global_ms;
      packet->current += 4;
    }
    
    P4 &= ~0x02;
  }else{
    P4 ^= 0x02;
  }
}

void hmr3300_if_init(FIL *f){
  char c;
  uart1_device = UART1_HMR3300;
  
  uart1_bauding(19200UL);
  SCON1 = 0x10; // clear errors esp. buffer overrun
  uart1_write("*S\r\n", 4);
  uart1_write("*M\r\n", 4);
  while(uart1_rx_size()){
    uart1_read(&c, sizeof(c));
  }
  
  uart1_buf[0] = 'M';
  uart1_buf[1] = 0;
  uart1_buf[2] = 0;
}

void hmr3300_if_polling(){
  static xdata u8 * xdata next_buf = uart1_buf + 8;
  u8 i;
  
  if(rw_hmr3300_if){
    rw_hmr3300_if = FALSE;
    
    uart1_write("*Q\r\n", 4);
  }
  
  while((i = uart1_rx_size()) >= 8){
    
    char buf[7];
    char *c = buf;
    
    for(i = min(i, 6); i > 0; --i){
    
      uart1_read(c, 1);
    
      switch(*c){
        case '\r':
          uart1_read(c, 1);  // skip '\n'
        case ',':
          break;
        default:
          c++;
          continue;
      }
      
      break;
    }
    
    *c = '\0';
    *(s16 *)next_buf = (s16)atoi(buf);
    next_buf += 2; 
    
    // Rotate
    if(next_buf >= uart1_buf + sizeof(uart1_buf)){
      
      // Log
      com_manager_assign_page(make_packet);
      
      next_buf = uart1_buf + 8;
    }
  }
}


