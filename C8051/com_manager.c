#include <stdio.h>
#include <string.h>

#include "main.h"
#include "com_manager.h"
#include "util.h"
#include "USB_CDC.h"
#include "f34x_USB_ISR.h"
#include "tff.h"
#include "diskio.h"

static xdata packet_t packet;

static void packet_init(
    packet_t *p, 
    PAYLOAD_T xdata *buf, 
    PAYLOAD_SIZE_T max_size){
  
  p->buf_begin = p->current = buf;
  p->buf_end = p->current + max_size;
}

#define BUFFER_SIZE (PAGE_SIZE * 16 * 2) // �_�u���o�b�t�@
#define PAGES (BUFFER_SIZE / PAGE_SIZE)

static xdata PAYLOAD_T payload_buf[BUFFER_SIZE];

static volatile xdata PAYLOAD_T * xdata free_page 
    = payload_buf;
static volatile xdata PAYLOAD_T * xdata locked_page
    = payload_buf;

PAYLOAD_SIZE_T com_manager_assign_page(void (* packet_maker)(packet_t *)){
  xdata PAYLOAD_T * next_free_page = free_page + PAGE_SIZE;
  if(next_free_page >= (xdata PAYLOAD_T *)(payload_buf + sizeof(payload_buf))){
    next_free_page -= sizeof(payload_buf);
  }
  
  if(next_free_page == locked_page){return 0;}
  
  packet_init(&packet, free_page, PAGE_SIZE);
  packet_maker(&packet);
  if(packet.current > packet.buf_begin){
    free_page = next_free_page;
  }
  
  return PAGE_SIZE;
}

static xdata FIL dat_file;
__xdata __at (0x01E2) FATFS fs;

volatile bit log_file_opened;

#define log_file_open() \
if(!log_file_opened){ \
	if(f_mount(0, &fs) == FR_OK){ \
  	if(f_open(&dat_file, "log.dat", (FA_OPEN_ALWAYS | FA_WRITE)) == FR_OK){ \
    	f_lseek(&dat_file, dat_file.fsize); \
    	log_file_opened = TRUE; \
  	} \
	} \
}
#define log_file_close() \
if(log_file_opened){ \
  log_file_opened = FALSE; \
  f_close(&dat_file); \
} \
f_mount(0, NULL)
#define log_file_write(buf, size, result) \
f_write(&dat_file, buf, size, result)
#define log_file_sync() \
f_sync(&dat_file)

void com_manager_load_config(char *fname, void (* func)(FIL *)){
  if(f_mount(0, &fs) == FR_OK){
    if(f_open(&dat_file, fname, (FA_OPEN_EXISTING | FA_READ)) == FR_OK){
      func(&dat_file);
      f_close(&dat_file);
    }
    f_mount(0, NULL);
  }
}

void com_manager_init(){
  log_file_opened = FALSE;
}

u16 log_block_size; // PAGE_SIZE�̔{���ł��邱�Ƃ�����

static u16 log_to_file(){
  u16 accepted_bytes;
  
  //P4 ^= 0x02;
  log_file_write( 
    locked_page,
    log_block_size, &accepted_bytes);
  {
    static xdata u8 loop = 0;
    if((++loop) == 64){
      loop = 0;
      log_file_sync();
    }
  }
  
  return accepted_bytes;
}

static u16 log_to_host(){
  static xdata u8 header[] = {0xF7, 0xE0}; 
  static xdata u16 sequence_num = 0;
  u16 crc = crc16(locked_page, log_block_size, 
      crc16((u8 *)&(++sequence_num), sizeof(sequence_num), 0));
  if(!(cdc_tx(header, sizeof(header))
      && cdc_tx((u8 *)&sequence_num, sizeof(sequence_num))
      && (cdc_tx(locked_page, log_block_size) == log_block_size)
      && cdc_tx((u8 *)&crc, sizeof(crc)))){
    return 0;
    //P4 ^= 0x02;
  }
  return log_block_size;
}

void com_manager_polling() {
  
  code u16 (* log_func)() = NULL;
  
  if(usb_enable){
    // USB Disbale => Enable
    if(!usb_previous_enable){
      log_file_close();
      free_page = locked_page = payload_buf;
      return;
    }
    log_block_size = PAGE_SIZE;
    log_func = log_to_host;
  }else{
    // USB Enable => Disable
    if(usb_previous_enable){
      log_file_open();
      free_page = locked_page = payload_buf;
      return;
    }
    disk_ioctl(0, GET_SECTOR_SIZE, (void *)&log_block_size);
    if((tickcount >= 500) && (state & STATE_LOG_ON)){
      log_func = log_to_file;
    }
  }
    
  // �f�[�^�����E�ɒB�����ꍇ�A��������
  while(TRUE){
    xdata PAYLOAD_T * next_locked_page = locked_page + log_block_size;
    if(next_locked_page >= (xdata PAYLOAD_T *)(payload_buf + sizeof(payload_buf))){
      next_locked_page -= sizeof(payload_buf);
    }
    
    if((next_locked_page > locked_page) 
        ? ((free_page >= locked_page) && (free_page < next_locked_page))
        : !((free_page >= next_locked_page) && (free_page < locked_page))){
      break;
    }
    
    if(log_func){
      if(log_func()){
        __critical {
          state |= STATE_WRITTEN_LOG;
        }
      }else{break;}
    }
    
    locked_page = next_locked_page;
  }
}
