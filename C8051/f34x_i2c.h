#ifndef __F34X_I2C_H__
#define __F34X_I2C_H__

#include "type.h"

#define I2C_WRITE(address_rw) (address_rw & ~0x01)
#define I2C_READ(address_rw) (address_rw | 0x01)

void i2c_init();
u8 i2c_read_write(u8 address_wr_flag, u8 *buf, u8 size);

#endif /* __F34X_I2C_H__ */
