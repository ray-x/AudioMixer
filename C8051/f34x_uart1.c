//-----------------------------------------------------------------------------
// F34x_UART1.c
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>

#include "main.h"
#include "f34x_uart1.h"
#include "fifo.h"

#define DEFAULT_BAUDRATE  9600UL          // Baud rate of UART in bps

#define CRITICAL_UART1(func) \
{\
  EIE2 &= ~0x02;\
  func;\
  EIE2 |= 0x02;\
}


/**
 * FIFO
 */
xdata fifo_char_t fifo_tx1;
xdata fifo_char_t fifo_rx1;
static __xdata __at (0x100) 
  char buffer_tx1[UART1_TX_BUFFER_SIZE];
static __xdata __at (0x100 + UART1_TX_BUFFER_SIZE) 
  char buffer_rx1[UART1_RX_BUFFER_SIZE];

/**
 * uart1_bauding
 * 
 */
void uart1_bauding(unsigned long baudrate){
  unsigned long selector = 0x10000UL - (SYSCLK/baudrate/2);
  
  SBCON1 = 0x03; // SB1PS[1:0] = 11;
  
  SBRLH1 = (unsigned char)((selector >> 8) & 0xFF);
  SBRLL1 = (unsigned char)(selector & 0xFF);
  
  SBCON1 |= 0x40; // SB1RUN = 1;
}


/**
 * uart1_init
 * 
 */
void uart1_init() {
  // SCON1: RX enabled
  // ninth bits are zeros
  // clear RI1 and TI1 bits
  SCON1 = 0x10;
  
  // 8-bit, no parity, 1 stop bit
  //SMOD1 = 0x0C; // default
          
  fifo_char_init(&fifo_tx1, buffer_tx1, UART1_TX_BUFFER_SIZE); 
  fifo_char_init(&fifo_rx1, buffer_rx1, UART1_RX_BUFFER_SIZE); 

  uart1_bauding(DEFAULT_BAUDRATE);

  SCON1 &= ~0x08;   // TBX1�͏����ݒ��t���O��0(�����݂��Ă��Ȃ�)
  EIE2 |= 0x02;     // ���荞�ݗL��
  //EIP2 = 0x02;      // �D��x1
}

/**
 * Regist sending data via UART1
 * 
 * @param data pointer to data
 * @param size size of data
 * @return (FIFO_SIZE_T) the size of registed data to buffer
 */
FIFO_SIZE_T uart1_write(char *buf, FIFO_SIZE_T size){
  // TB80�͏����ݒ��t���O�Ƃ��Ďg��
  // 0(�����݂��Ă��Ȃ�)��������蓮���荞�݂�������
  if(size){
    size = fifo_char_write(&fifo_tx1, buf, size);
    CRITICAL_UART1(
      if(!(SCON1 & 0x0A)){ // !TBX1 && !TI1
        SCON1 |= 0x02; // �蓮�Ŋ����݂�������
      }
    );
  }
  return size;
}

/**
 * Return the size of untransimitted data via UART1
 * 
 * @return (FIFO_SIZE_T) the size
 */
FIFO_SIZE_T uart1_tx_size(){
  return fifo_char_size(&fifo_tx1);
}

/**
 * Get the recieved data via UART1
 * 
 * @param buf buffer
 * @param size the size of buffer
 * @return (FIFO_SIZE_T) the real size of grabbed data
 */
FIFO_SIZE_T uart1_read(char *buf, FIFO_SIZE_T size){
  return fifo_char_read(&fifo_rx1, buf, size);
}

/**
 * Return the size of ungrabbed data via UART1
 * 
 * @return (FIFO_SIZE_T) the size
 */
FIFO_SIZE_T uart1_rx_size(){
  return fifo_char_size(&fifo_rx1);
}
