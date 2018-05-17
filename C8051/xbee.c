/**
 * XBee support routines by fenrir.
 */

#include <c8051f340.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "type.h"
#include "util.h"
#include "f34x_uart1.h"
#include "com_manager.h"
#include "uart1_device.h"
#include "dsp.h"

#include "xbee.h"

#define PACKET_HEADER_SIZE 6

static void make_packet(packet_t *packet){
  
  // packet��o�^����̂ɏ\���ȃT�C�Y�����邩�m�F
  if((packet->buf_end - packet->current) < PAGE_SIZE){
    P4 ^= 0x02;
    return;
  }else{
    P4 &= ~0x02;
  }
  
  memcpy(packet->current, uart1_buf, sizeof(uart1_buf));
  
  (packet->current)++;  // �w�b�_ 'C'
  *((packet->current)++) = tickcount & 0xFF;
  
  // �����̓o�^
  {
    // LSB first
    *((u32 *)(packet->current)) = global_ms;
    packet->current += 4;
  }  
}

u8 xbee_tx(u8 *buf, u8 size){
  return (uart1_device == UART1_XBEE)
      ? uart1_write(buf, size)
      : 0;
}

void xbee_init(){
  // XBee�p�̏�����
  uart1_buf[0] = 'C';
}

void xbee_polling(){
  
#ifdef XBEE_TX_TEST
  // ���M�e�X�g�p
  uart1_write("The quick brown fox jumps over the lazy dog. 1234567890", 55);
#else
  static xdata u8 * xdata next_buf = uart1_buf + PACKET_HEADER_SIZE;
  
  u8 rx_size;
  char buf[PAGE_SIZE + 1];
  
  buf[0] = 'C';
  
  // ��M�f�[�^�͋L�^���Ƃ�]������
  while(rx_size = min(sizeof(buf) - 1, uart1_rx_size())){
    char *buf_content = buf + 1;
    u8 uart1_buf_remain = uart1_buf + sizeof(uart1_buf) - next_buf;
    
    uart1_read(buf_content, rx_size);

#ifdef XBEE_LOOPBACK
    // XBee Loopback�e�X�g�p
    uart1_write(buf_content, rx_size);
#endif
    
    // DSP�֑��M
    spi_dsp_write(buf, rx_size + 1);
    
    // �L�^���Ƃ�
    while(rx_size >= uart1_buf_remain){
      memcpy(next_buf, buf_content, uart1_buf_remain);
      com_manager_assign_page(make_packet);
      next_buf = uart1_buf + PACKET_HEADER_SIZE;
      rx_size -= uart1_buf_remain;
      buf_content += uart1_buf_remain;
    }
    memcpy(next_buf, buf_content, rx_size);
    next_buf += rx_size;
  }
#endif
}
