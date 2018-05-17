#include <c8051F340.h>
#include "main.h"
#include "AD7739.h"
#include "com_manager.h"
#include "f34x_adc.h"
#include "dsp.h"

/**
 * AD7739
 *
 * === Connection ===
 * C8051               AD7739
 *  P4.7(OUT)      <=>  SCLK
 *  P4.5(IN)       <=>  DOUT
 *  P4.6(OUT)      <=>  DIN
 *  P0.7/-INT1(IN) <=>  -RDY
 */

#define clk_up()     (P4 |= 0x80)
#define clk_down()   (P4 &= ~0x80)
#define out_up()     (P4 |= 0x40)
#define out_down()   (P4 &= ~0x40)
#define is_in_up()   (P4 & 0x20)
#define is_int1_up() (P0 & 0x80)

/*
static u8 port_manuipulator(u8 op_code){
  switch(op_code){
    case PSPI_CLK_UP:   return clk_up();
    case PSPI_CLK_DOWN: return clk_down();
    case PSPI_OUT_UP:   return out_up();   
    case PSPI_OUT_DOWN: return out_down();
    default:  return is_in_up();
  }
}

void ad7739_out_write(u8 b){
  write_read_pseudo_spi_byte(b, port_manuipulator);
}

u8 ad7739_in_read(){
  return write_read_pseudo_spi_byte(0x00, port_manuipulator);
}
*/

#define _nop_() { \
  _asm \
    nop \
  _endasm; \
}

void ad7739_out_write(u8 b)
{
	u8 mask = 0x80;
	do{
		clk_down();
		(b & mask) ? out_up() : out_down();
		clk_up();
	}while(mask >>= 1);
	out_up();
}

u8 ad7739_in_read(void)
{
	u8 temp = 0;
	u8 mask = 0x80;
	out_down();
	do{
		clk_down();
    _nop_();
    if(is_in_up()) temp |= mask;
		clk_up();
	}while(mask >>= 1);
	out_up();
	return temp;
}

static volatile bit ad7739_done;

void ad7739_init(void)
{
  // Power on Reset
  {
    u8 i;
    ad7739_out_write(0x00);
    for(i = 0; i < 4; i++) ad7739_out_write(0xFF); 
  }
  
  // wait
  {
    u16 i;
    for(i = 0; i < 10000; i++);
  }
	

	// Caliblation
	ad7739_out_write(AD7739_WRITE_OP(AD7739_ADDR_MODE));
	ad7739_out_write(0x80);  // ADC Zero-Scale 
	while(is_int1_up());
	ad7739_out_write(AD7739_WRITE_OP(AD7739_ADDR_MODE));
	ad7739_out_write(0xA0);  // ADC Full-Scale
	while(is_int1_up());

	// continuous
  {
    u8 ch;
  	for(ch = 0; ch < 8; ch++){
  		ad7739_out_write(AD7739_WRITE_OP(AD7739_CHANNEL(AD7739_ADDR_C_SET, ch)));
  		ad7739_out_write(0x0C);
    }
  	ad7739_out_write(AD7739_WRITE_OP(AD7739_ADDR_IO));
  	ad7739_out_write(0x38);
  }
  
  IT1 = 1;    // Edge sense
  //PX1 = 1;    // Proiority High
  EX1 = 1;    // Enable
  
  ad7739_done = FALSE;
	
	clk_up();
	out_up();
}

void ad7739_start(void)
{
  if(!ad7739_done){
  	// single
  	/*
  	ad7739_out_write(AD7739_WRITE_OP(AD7739_CHANNEL(AD7739_ADDR_MODE, 0)));
  	ad7739_out_write(0x42);
  	*/
  
  	// continuous
  	ad7739_out_write(AD7739_WRITE_OP(AD7739_ADDR_MODE));
  	ad7739_out_write(0x26);
  }
}

void ad7739_stop(void)
{
	// single
	/* return; */

	// continous
	ad7739_out_write(AD7739_WRITE_OP(AD7739_ADDR_MODE));
	ad7739_out_write(0x02);
  ad7739_done = TRUE;
}

static void make_packet(packet_t *packet){
  
  // packetが壊れていないか、確認用
  bit valid_packet = TRUE;
  
  // packetを登録するのに十分なサイズがあるか確認
  if((packet->buf_end - packet->current) < PAGE_SIZE){
    state &= ~STATE_RX_FROM_AD7739;
    P4 ^= 0x02;
    return;
  }else{
    P4 &= ~0x02;
  }
  
  *((packet->current)++) = 'A';
  *((packet->current)++) = tickcount & 0xFF;
  
  // 時刻の登録
  {
    // LSB first
    *((u32 *)(packet->current)) = global_ms;
    packet->current += 4;
  }
  
  {
    u8 ch, i, received, is_valid;
    for(ch = 0; ch < 8; ch++){
      ad7739_out_write(AD7739_READ_OP(AD7739_CHANNEL(AD7739_ADDR_C_DATA, ch)));
      is_valid = FALSE;
      for(i = 0 ; i < 3; i++, (packet->current)++){
        received = *(packet->current) = ad7739_in_read();
        if(!is_valid && (received != (i == 0 ? 0x80 : 0x00))){is_valid = TRUE;}
      }
      if(!is_valid){valid_packet = FALSE;}
    }
  }
  
  polling_adc();
  *((u16 *)packet->current) = get_adc_value();
  packet->current += 2;
  
  spi_dsp_write(packet->buf_begin, (packet->current - packet->buf_begin));
  
  if(valid_packet){
    __critical {
      state |= STATE_RX_FROM_AD7739;
    }
  }
}

void ad7739_polling(){
  
  if(ad7739_done){
    set_adc_as_temperature_sensor();  // 温度センサ
    start_adc();
    com_manager_assign_page(make_packet);
    ad7739_done = FALSE;
  }
}
