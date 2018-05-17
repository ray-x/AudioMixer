#include <c8051f340.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "type.h"
#include "f34x_uart1.h"
#include "com_manager.h"
#include "hmr3300_if.h"
#include "xbee.h"
#include "dsp.h"

#include "uart1_device.h"

xdata enum uart1_device_list uart1_device;
xdata u8 uart1_buf[PAGE_SIZE];

void uart1_device_init(){
  
  uart1_device = UART1_NONE;
  
  // �ݒ�t�@�C��������ΗL����
  com_manager_load_config("hmr3300.dat", hmr3300_if_init);
  if(uart1_device != UART1_NONE){
    return;
  }
  
  // �f�t�H���g�ł�XBee��I��
  uart1_device = UART1_XBEE;
  xbee_init();
}

void uart1_device_polling(){
  
  switch(uart1_device){
    case UART1_XBEE: {
      xbee_polling();
      break;
    }
    case UART1_HMR3300: {
      hmr3300_if_polling();
      break;
    }
  }
}
