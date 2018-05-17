// SFR declarations
#include <c8051F340.h>
#include <string.h>

#include "f34x_spi.h"
#include "com_manager.h"
#include "util.h"
#include "dsp.h"
#include "f34x_USB_ISR.h"

// DSP������݂����Ӌ@��
#include "gps.h"
#include "f34x_i2c.h"
#include "xbee.h"

#define spi_select_DSP() {P4 &= ~(0x10);}
#define spi_deselect_DSP() {P4 |= 0x10;}

static xdata unsigned char dsp_ready_pages = 0;
static bit is_sync_frame;

/**
 * DSP�����M
 * �v���g�R���� ([] = 32bit�t���[���A�����SPI��4��쓮)
 *  MOSI [0x80 length 0x55 0x55] [0x55 ...] ... 
 *  MISO [NAQ ...] [NAQ ...] ... [0xAA 0xAA 0xAA tx_len] [p0 p1 p2 p3] ...
 * �t���[�����͑S����
 *  (length ? ((tx_len + 3) / 4) : 0) + 2 
 *   => length = 0�̂Ƃ����̎�M�t���[���̃o�C�g���̌���
 * �ő��M�o�C�g����length�Őݒ�
 * 
 * @param buf ��M�f�[�^�i�[�p�o�b�t�@
 * @param length �ő��M�o�C�g���A0�̂Ƃ��͎��̎�M�t���[���̃o�C�g���̌���
 * @return ��M�����o�C�g���A���邢�͎��̎�M�t���[���̃o�C�g��
 */
unsigned char spi_dsp_read(unsigned char *buf, unsigned char length){
  unsigned char frame_count = 0;
  unsigned char frames = 2;
  unsigned char sync_frame_checker = 8;  // num of retry
  unsigned char true_length = 0;
  while(frame_count < frames){
    unsigned char i, c;
    spi_select_DSP();
    is_sync_frame = TRUE;
    for(i = 0; i < 4; i++){
      c = 0x55;
      if(frame_count == 0){
        switch(i){
          case 0: c = 0x80; break;
          case 1: c = length; break;
        }
      }
      c = spi_write_read_byte(c);
      
      if(frame_count > 1){
        if(length){
          *(buf++) = c;
          length--;
        }
      }else if(sync_frame_checker){
        if(i < 3){
          if(c != 0xAA){
            is_sync_frame = FALSE;
          }
        }else{
          if(is_sync_frame){
            frames = ((c + 3) / 4) + 2;
            true_length = length = min(c, length);
            sync_frame_checker = 0;
          }else{
            frame_count = 0;
            sync_frame_checker--;
          }
        }
      }
    }
    spi_deselect_DSP();
    if((frame_count++) == 0){
      wait_us(10);
    }
  }
  return true_length;
}

/**
 * DSP�ɑ��M
 * �v���g�R���� ([] = 32bit�t���[���A�����SPI��4��쓮)
 *  MOSI [header1 p0 p1 p2] [header2 p3 p4 p5] [header2 p6 p7 p8] ...
 *    header1 = 1 (valid_bytes_in_this_frame 2bit) (remain_frames 5bit)
 *    header2 = 0 (valid_bytes_in_this_frame 2bit) (remain_frames 5bit)
 *  MISO [NAQ ...] ... [0x55 0x55 0x55 tx_ready_pages] [NAQ ...] ...
 * �t���[�����͑S���� ((length + 2) / 3) 
 * 
 * @param buf ���M�f�[�^
 * @param length ���M�o�C�g��
 * @return ���M�����o�C�g��
 */
unsigned char spi_dsp_write(unsigned char *buf, unsigned char length){
  unsigned char frame_count = 0;
  unsigned char remain_frames = ((length + 2) / 3);
  while(remain_frames){
    unsigned char i, c;
    spi_select_DSP();
    is_sync_frame = TRUE;
    for(i = 0; i < 4; i++){
      if(i == 0){ // Header
        c = remain_frames--;
        if(frame_count == 0){c |= 0x80;}
        c |= ((remain_frames ? 3 : length) << 5);
      }else{ // Payload
        if(length){
          length--;
          c = *(buf++);
        }else{
          c = 0x00;
        }
      }
      c = spi_write_read_byte(c);
      
      // notify check
      if(i < 3){
        if(c != 0x55){is_sync_frame = FALSE;}
      }else{
        if(is_sync_frame){dsp_ready_pages = c;}
      }
    }
    spi_deselect_DSP();
    if((frame_count++) == 0){
      wait_us(20);
    }
  } 
  return length;
}

static void send_dsp_config(FIL *f){
  u16 accepted_bytes;
  u8 buf[PAGE_SIZE];
  u32 read_count = 0;
  while(read_count < f->fsize){
    if(f_read(f, buf, PAGE_SIZE, &accepted_bytes) != FR_OK){break;}
    spi_dsp_write(buf, (u8)accepted_bytes);
    read_count += accepted_bytes;
    if(f_lseek(f, read_count) != FR_OK){break;}
  }
}

void dsp_init(){
  // DSP�������҂�
  wait_ms(1000);
  
  // �ݒ�t�@�C����DSP�ɑ��M
  com_manager_load_config("config.dat", send_dsp_config);
}

static void make_dsp_packet(packet_t *packet){
  // packet�̓o�^
  u8 rx_size;
  u8 buf[PAGE_SIZE * 2];  // �ő��M�T�C�Y
   
  if(rx_size = spi_dsp_read(buf, sizeof(buf))){
    state |= STATE_RX_FROM_DSP;
    switch(buf[0]){
      case 'N': // Navigation (�q�@���)
        rx_size = min(rx_size, (packet->buf_end - packet->current));
        memcpy(packet->current, buf, rx_size);
        packet->current += rx_size;
        return;
      case 'C': // Command (���Ӌ@��Ɏw�߂𑗂�)
        switch(buf[1]){
          case 'F': { // i2c�f�o�C�X(���FPGA)�ւ̎w��
            u8 buf_index = 2;
            while(buf_index < rx_size){
              // [address_wr] [size] [contents(0)] [contents(1)] ... �̌J��Ԃ�
              i2c_read_write(
                  buf[buf_index], &buf[buf_index + 2], buf[buf_index + 1]);
              buf_index += (buf[buf_index + 1] + 2);
            }
            break;
          }
          case 'G': // GPS�ւ̎w��
            gps_write(&buf[2], rx_size - 2);
            break;
        }
        break;
      case 'T': // Telemetory (Xbee�_�E�������N)
        xbee_tx(&(buf[1]), (rx_size - 1));
        break;
      default:
        return;
    }
  }
  
  // USB���L���ȏꍇ�͂��ׂĂ��_���v����
  if(usb_enable && usb_previous_enable){
    rx_size = min(rx_size, (packet->buf_end - packet->current));
    memcpy(packet->current, buf, rx_size);
    packet->current += rx_size;
  }
}

void dsp_polling(){
  // DSP�f�[�^�̃`�F�b�N
  for(; dsp_ready_pages; dsp_ready_pages--){
    if(!com_manager_assign_page(make_dsp_packet)){break;}
  }
}
