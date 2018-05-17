#ifndef __PITOT_IF_H__
#define __PITOT_IF_H__

#include "type.h"

#define PITOT_PER_PAGE 4
extern volatile bit rw_pitot_if;

void pitot_if_polling();

#endif
