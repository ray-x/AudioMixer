#include <c8051F340.h>
#include "main.h"
#include "f34x_i2c.h"
#include "com_manager.h"
#include "dsp.h"

#include <string.h>

#include "servo_if.h"

static u8 f_page_address;
volatile bit rw_servo_if = FALSE;

static void make_packet(packet_t *packet){
  
  u8 i2c_address_wr_flag;
  
  // packetを登録するのに十分なサイズがあるか確認
  if((packet->buf_end - packet->current) < PAGE_SIZE){
    state &= ~STATE_RW_SERVO_IF;
    P4 ^= 0x02;
    return;
  }else{
    state |= STATE_RW_SERVO_IF;
    P4 &= ~0x02;
  }
    
  *((packet->current)++) = 'F';
  *((packet->current)++) = f_page_address;
  *((packet->current)++) = 0;
  *((packet->current)++) = tickcount & 0xFF;
  
  // 時刻の登録
  {
    // LSB first
    *((u32 *)(packet->current)) = global_ms;
    packet->current += 4;
  }
  
  /*
   * [OBS_L(8bits), OBS_H(4bits) + DRV_H(4bits), DRV_L(8bits)] * NUM_OF_SERVOS
   */
  // Servo Read
  for(i2c_address_wr_flag = (f_page_address | 0x01); 
      i2c_address_wr_flag < (NUM_OF_SERVOS << 1);
      i2c_address_wr_flag += (1 << 1), packet->current += 3){
    
    u8 buf[2];
    i2c_read_write(i2c_address_wr_flag, buf, sizeof(buf));
    *(packet->current) = buf[1];
    *(packet->current + 1) = buf[0] << 4;
    i2c_read_write((i2c_address_wr_flag | 0x10), buf, sizeof(buf));
    *(packet->current + 2) = buf[1];
    *(packet->current + 1) |= (buf[0] & 0x0F);
  }
  
  spi_dsp_write(packet->buf_begin, packet->current - packet->buf_begin);
}

void servo_if_polling(){
  
  if(rw_servo_if){
    rw_servo_if = FALSE;
    
#if defined(SERVO_TEST)
    {
      u8 i2c_address_wr_flag;
      // Servo Driver(address:0b0001000-0b0001111の8個) Read/Write
      for(i2c_address_wr_flag = 0x10; 
          i2c_address_wr_flag < (0x10 + (8 << 1));
          i2c_address_wr_flag += (1 << 1)){
        
        u8 buf[2];
        // read
        i2c_read_write((i2c_address_wr_flag | 0x01), buf, sizeof(buf));
        // increment
        if(((++buf[1]) == 0x00) && ((++buf[0]) == 0x10)){
          buf[0] = 0;
        }
        // write
        i2c_read_write(i2c_address_wr_flag, buf, sizeof(buf));
      }
    }
#endif
    
    // サーボ8組(OBSとDRVで1組)ずつ処理する
    for(f_page_address = 0; 
        f_page_address < (((NUM_OF_SERVOS + 7) / 8) << 2); 
        f_page_address += 0x20){
      // Log
      com_manager_assign_page(make_packet);
    }
  }
}
