#ifndef __SERVO_IF_H__
#define __SERVO_IF_H__

#include "type.h"

#define NUM_OF_SERVOS 8
extern xdata u8 servo_data[3 * NUM_OF_SERVOS];
extern volatile bit rw_servo_if;

void servo_if_polling();

#endif
