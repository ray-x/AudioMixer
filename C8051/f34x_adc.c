#include "main.h"
#include "f34x_adc.h"

static xdata int temperature_offset = 0;

/**
 * get_raw_temperature()
 * 
 * @return result from AD converter
 */
unsigned int get_raw_temperature(){
  set_adc_as_temperature_sensor();
  start_adc();
  polling_adc();
  return get_adc_value();
}

/**
 * get_temperature()
 * 
 * @return ambient temperature in celcius [C] decimal values second byte means digits after colon
 */
int get_temperature(){
  return (((get_raw_temperature() * 322L) - 79700)/ 286) + temperature_offset;
}

/**
 * calibrate
 * 
 * @param temparture ambient temperature in celcius [C] decimal values second byte means digits after colon
 */
void calibrate(int t){
  int t2 = (((get_temperature() * 322L) - 79700)/ 286) ;
  temperature_offset = t - t2;
}
