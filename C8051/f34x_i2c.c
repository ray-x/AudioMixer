#include <c8051F340.h>    // SFR declarations

#include "f34x_i2c.h"
#include "type.h"

/**
 * Configure i2c with Timer 0
 * 
 */
void i2c_init(){
  // timer 0 setup => mode 2, 8bit w autoreload
  TMOD &= ~0x0F;
  TMOD |=  0x02;
  
  CKCON |= 0x04; // => use SYSCLK
  TH0 = TL0 = 0xF0; // => freq: 48M / 16 = 3M 
  //TH0 = TL0 = 0x00; // => freq: 48M / 256 
  TR0 = 1;
  
  // enable, inhibit slave, ex s/h time, use timer 0 overflow 
  SMB0CF = (0x40 | 0x10);
  
  SMB0CF |= 0x80;
}

/**
 * Read or Write via i2c
 * 
 * @return remain bytes
 */
u8 i2c_read_write(u8 address_wr_flag, u8 *buf, u8 size){
  
  // start bit
  STA = 1;
  while(!(SMB0CN & 0x05));
  STA = 0;
  
  if((SMB0CN & 0xDC) == 0xC0){
    // address + R/W flag
    SMB0DAT = address_wr_flag;
    SI = 0;
    while(!(SMB0CN & 0x05));
    
    if((SMB0CN & 0xFE) == 0xC2){
      // Read / Write
      if(address_wr_flag & 0x01){ 
        while(1){
          SI = 0;
          while(!(SMB0CN & 0x05));
          if((SMB0CN & 0xFC) == 0x88){
            *(buf++) = SMB0DAT;
            if(--size){
              ACK = 1;
            }else{
              ACK = 0;
              break;
            }
          }else{break;}
        }
      }else{
        while(size--){
          SMB0DAT = *(buf++);
          SI = 0;
          while(!(SMB0CN & 0x05));
          if((SMB0CN & 0xFE) != 0xC2){break;}
        }
      }
    }
  }
  STO = 1;
  SI = 0;
  
  return size;
}
