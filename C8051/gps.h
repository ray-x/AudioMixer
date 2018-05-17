#ifndef __GPS_H__
#define __GPS_H__

/*
 * u-block GPS => Little Endian!!
 * sdcc => Little Endian
 * 
 */

#include "type.h"

#define MAX_SAT 16

void gps_init(void);
void gps_polling();
void gps_write(char *buf, int size);

extern volatile bit gps_ms_modified;
extern u8 gps_num_of_sat;
extern s32 gps_ms;

#define CRITICAL_GPS_1PPS(func) \
{ \
  IE0 = 0; \
  { \
    func; \
  } \
  IE0 = 1; \
}

#ifdef MAIN_C
void interrupt_int0(void) interrupt 0
{
  P4 ^= 0x01;
  if(gps_ms_modified){
    gps_ms_modified = FALSE;
    global_ms = gps_ms;
  }
}
#endif

#endif /* __GPS_H__ */
