#ifndef __F34X_PSEUDO_SPI_H__
#define __F34X_PSEUDO_SPI_H__

#include "type.h"

#define PSPI_CLK_UP     0x01
#define PSPI_CLK_DOWN   0x10
#define PSPI_OUT_UP     0x02
#define PSPI_OUT_DOWN   0x20
#define PSPI_IS_IN_UP   0x08

u8 write_read_pseudo_spi_byte(u8 b,
    u8 (*port_manuipulator)(u8 op_code));

#endif
