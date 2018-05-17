// SFR declarations
#include <c8051F340.h>

#include "f34x_pseudo_spi.h"

#define _nop_() { \
  _asm \
    nop \
  _endasm; \
}

#define clk_up()     port_manuipulator(PSPI_CLK_UP)
#define clk_down()   port_manuipulator(PSPI_CLK_DOWN)
#define out_up()     port_manuipulator(PSPI_OUT_UP)
#define out_down()   port_manuipulator(PSPI_OUT_DOWN)
#define is_in_up()   port_manuipulator(PSPI_IS_IN_UP)

/**
 * Function sends one byte to pseudo-spi and reads ony byte from pseudo-spi
 * 
 * @param byte value to write
 * @return SPI byte
 */
u8 write_read_pseudo_spi_byte(
    u8 b,
    u8 (*port_manuipulator)(u8 op_code)){

  u8 mask = 0x80;
  u8 temp = 0;
  
  out_down();
  do{
    clk_down();
    (b & mask) ? out_up() : out_down();
    if(is_in_up()) temp |= mask;
    clk_up();
  }while(mask >>= 1);
  out_up();
  return temp;
}
