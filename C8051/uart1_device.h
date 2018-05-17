#ifndef __UART1_DEVICE_H__
#define __UART1_DEVICE_H__

enum uart1_device_list {
  UART1_NONE = 0,
  UART1_XBEE = 1,
  UART1_HMR3300 = 2
};

extern xdata enum uart1_device_list uart1_device;
extern xdata u8 uart1_buf[PAGE_SIZE];

void uart1_device_init();
void uart1_device_polling();

#endif /* __UART1_DEVICE_H__ */
