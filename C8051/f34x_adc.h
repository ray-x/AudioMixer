//-----------------------------------------------------------------------------
// f34x_adc.h
//-----------------------------------------------------------------------------
#ifndef __F34X_ADC_H__
#define __F34X_ADC_H__

#include <c8051F340.h>
#include "type.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

unsigned int get_raw_temperature();
int get_temperature();
void calibrate(int temp_value);

#define start_adc() {AD0BUSY = 1;}
#define polling_adc() { \
  while(!AD0INT); \
  AD0INT = 0; \
}
#define get_adc_value() (((u16)ADC0H << 8) | (ADC0L))
#define set_adc_register(amx0p, amx0n, adc0cn, adc0cf) { \
  AMX0P = amx0p; \
  AMX0N = amx0n; \
  ADC0CN = adc0cn; \
  ADC0CF = adc0cf; \
}
#define set_adc_as_temperature_sensor() set_adc_register(0x1E, 0x1F, 0x80, 0x48)
// AMX0P = 0x1E(Temp), AMX0N = 0x1F(GND)
// ADC0CN = 0x80, ADC0CF = 0b01001000 (CLKSAR = 4.8MHz => SA@Tabel.5.1 = 0.25)

#endif /* __F34X_ADC_H__ */
