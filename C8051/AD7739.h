#ifndef __AD7739_H__
#define __AD7739_H__

#define AD7739_ADDR_COM    0x00
#define AD7739_ADDR_IO     0x01
#define AD7739_ADDR_REV    0x02
#define AD7739_ADDR_TEST   0x03
#define AD7739_ADDR_STATUS 0x04
#define AD7739_ADDR_CHKSUM 0x05
#define AD7739_ADDR_ZS_CAL 0x06
#define AD7739_ADDR_FS_CAL 0x07
#define AD7739_ADDR_C_DATA 0x08
#define AD7739_ADDR_C_ZS   0x10
#define AD7739_ADDR_C_FS   0x18
#define AD7739_ADDR_C_STAT 0x20
#define AD7739_ADDR_C_SET  0x28
#define AD7739_ADDR_C_CT   0x30
#define AD7739_ADDR_MODE   0x38

#define AD7739_READ_OP(addr)     (0x40 | addr)
#define AD7739_WRITE_OP(addr)    (addr)
#define AD7739_CHANNEL(addr, ch) (addr | ch)

#include "type.h"

void ad7739_init();
void ad7739_polling();

void ad7739_start();
void ad7739_stop();

void ad7739_out_write(u8);
u8 ad7739_in_read();

#define CRITICAL_AD7739(func) \
{ \
  EX1 = 0; \
  { \
    func; \
  } \
  EX1 = 1; \
}

#ifdef MAIN_C
void interrupt_int1(void) interrupt 2
{

  //IE1 = 0; // Auto Clear TCON.1(IE1) Flag

  ad7739_stop();
}
#endif

#endif /* __AD7739_H__ */
