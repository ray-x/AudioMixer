#include <c8051F340.h>
#include "main.h"
#include "f34x_pseudo_spi.h"
#include "com_manager.h"
#include "dsp.h"

#include <string.h>

#include "pitot_if.h"

#define PITOT_8BYTES_MODE

/*
 * [v(MSB,LSB), a(MSB,LSB), b(MSB,LSB)] * PITOT_PER_PAGE
 */
static xdata u8 pitot_data[6 * PITOT_PER_PAGE];
volatile bit rw_pitot_if = FALSE;

/*
 * Pitot Tube
 *
 * === Connection ===
 * C8051               Pitot
 *  P2.0(OUT)      <=>  SCLK
 *  P2.1(IN)       <=>  DOUT
 *  P2.2(OUT)      <=>  DIN
 *  P2.3(OUT)      <=>  CS
 */

#define clk_up()     (P2 |= 0x01)
#define clk_down()   (P2 &= ~0x01)
#define out_up()     (P2 |= 0x04)
#define out_down()   (P2 &= ~0x04)
#define cs_up()      (P2 |= 0x08)
#define cs_down()    (P2 &= ~0x08)
#define is_in_up()   (P2 & 0x02)

static u8 port_manuipulator(u8 op_code){
  switch(op_code){
    case PSPI_CLK_UP:   return clk_up();
    case PSPI_CLK_DOWN: return clk_down();
    case PSPI_OUT_UP:   return out_up();   
    case PSPI_OUT_DOWN: return out_down();
    default:  return (is_in_up() ? TRUE : FALSE);
  }
}

static void make_packet(packet_t *packet){
  
  // packetを登録するのに十分なサイズがあるか確認
  if((packet->buf_end - packet->current) < PAGE_SIZE){
    state &= ~STATE_RW_PITOT_IF;
    P4 ^= 0x02;
    return;
  }else{
    state |= STATE_RW_PITOT_IF;
    P4 &= ~0x02;
  }
    
  *((packet->current)++) = 'P';
  *((packet->current)++) = 0;
  *((packet->current)++) = 0;
  *((packet->current)++) = tickcount & 0xFF;
  
  // 時刻の登録
  {
    // LSB first
    *((u32 *)(packet->current)) = global_ms;
    packet->current += 4;
  }
  
  memcpy(packet->current, pitot_data, sizeof(pitot_data));
  packet->current += sizeof(pitot_data);
  
  spi_dsp_write(packet->buf_begin, packet->current - packet->buf_begin);
}

void pitot_if_polling(){
  
  static xdata u8 * xdata next_buf = pitot_data;
  u8 i;
  
  if(rw_pitot_if){
    rw_pitot_if = FALSE;
    
    cs_down();
    write_read_pseudo_spi_byte(0xA5, port_manuipulator);  // 先頭1バイトは無視する
#ifdef PITOT_8BYTES_MODE
    for(i = 0; i < 8; i++){
#else
    for(i = 0; i < 6; i++){
#endif
      *(next_buf++) = write_read_pseudo_spi_byte(0x00, port_manuipulator);
    }
    cs_up();
    
    // Rotate
    if(next_buf >= pitot_data + sizeof(pitot_data)){
      
      // Log
      com_manager_assign_page(make_packet);
      
      next_buf = pitot_data;
    }
  }
}
