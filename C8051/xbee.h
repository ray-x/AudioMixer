#ifndef __XBEE_H__
#define __XBEE_H__

#include "type.h"

u8 xbee_tx(u8 *buf, u8 size);
void xbee_init();
void xbee_polling();

#endif
