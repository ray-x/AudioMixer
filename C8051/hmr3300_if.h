#ifndef __HMR3300_IF_H__
#define __HMR3300_IF_H__

#include "type.h"
#include "tff.h"

extern volatile bit rw_hmr3300_if;

void hmr3300_if_init(FIL *f);
void hmr3300_if_polling();

#endif
