//-----------------------------------------------------------------------------
// F34x_UART1.h
//-----------------------------------------------------------------------------

#ifndef _UART1_H_
#define _UART1_H_

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void uart1_bauding(unsigned long baudrate);
void uart1_init(void);

#define UART1_TX_BUFFER_SIZE 64
#define UART1_RX_BUFFER_SIZE 64

#include <c8051F340.h>
#include "fifo.h"

FIFO_SIZE_T uart1_write(char *buf, FIFO_SIZE_T size);
FIFO_SIZE_T uart1_read(char *buf, FIFO_SIZE_T size);
FIFO_SIZE_T uart1_tx_size();
FIFO_SIZE_T uart1_rx_size();

#ifdef MAIN_C

extern xdata fifo_char_t fifo_tx1;
extern xdata fifo_char_t fifo_rx1;

/* Interrupt(TI1 / RI1) */
void interrupt_uart1 (void) interrupt 16 {
  unsigned char c;
  
  if(SCON1 & 0x01){   // RI1
    SCON1 &= ~0x01;
    /* �����O�o�b�t�@��1�o�C�g�����o�� */
    c = SBUF1;
    if(fifo_char_put(&fifo_rx1, (char *)&c)){
      P4 &= ~0x02;
    }else{P4 ^= 0x02;}
  }
  
  if(SCON1 & 0x02){   // TI1
    SCON1 &= ~0x02;
    /* �������ރf�[�^�����邩�m�F */
    if(fifo_char_get(&fifo_tx1, (char *)&c) > 0){
    	SCON1 |= 0x08;   // TBX1�͏����ݒ��t���O�Ƃ��Ďg���A1(�����ݒ�)��
      SBUF1 = c;
    }else{
      SCON1 &= ~0x08;  // TBX1�͏����ݒ��t���O�Ƃ��Ďg���A0(�����݂��Ă��Ȃ�)��
    }
  }
}
#endif

#endif
