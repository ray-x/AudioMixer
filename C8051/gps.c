/**
 * GPS (LEA-4T) support routines.
 *  by fenrir.
 */

#include <c8051f340.h>
#include <string.h>

#include "f34x_uart0.h"
#include "com_manager.h"
#include "gps.h"
#include "util.h"
#include "dsp.h"

void gps_write(char *buf, int size){
  int written = 0;
  while(TRUE){
    written = uart0_write(buf, size);
    size -= written;
    if(size == 0){break;}
    buf += written;
    P4 ^= 0x01;
  }
}

static void checksum(unsigned char *ck_a, 
                        unsigned char *ck_b, 
                        unsigned char *packet, 
                        int size){
  u8 a, b;
  a = b = 0x00;
  while(size-- > 0){
    a += *(packet++);
    b += a;
  }
  *ck_a = a;
  *ck_b = b;
}

#define swap(x) (\
(((x) & 0x80) ? 0x01 : 0x00) | \
(((x) & 0x40) ? 0x02 : 0x00) | \
(((x) & 0x20) ? 0x04 : 0x00) | \
(((x) & 0x10) ? 0x08 : 0x00) | \
(((x) & 0x08) ? 0x10 : 0x00) | \
(((x) & 0x04) ? 0x20 : 0x00) | \
(((x) & 0x02) ? 0x40 : 0x00) | \
(((x) & 0x01) ? 0x80 : 0x00) \
)

#define UBX_CFG_RATE_PS   (u16)6
#define UBX_CFG_RATE_MEAS (u16)250
#define UBX_CFG_RATE_NAV  (u16)1
#define UBX_CFG_RAET_TIME (u16)0

static void set_ubx_cfg_rate()
{
  unsigned char packet[14];
  packet[0] = 0xB5;
  packet[1] = 0x62;
  packet[2] = 0x06;
  packet[3] = 0x08;
  packet[4 + 0]  = (UBX_CFG_RATE_PS & 0xFF);
  packet[4 + 1]  = (UBX_CFG_RATE_PS >> 8) & 0xFF;
  packet[6 + 0]  = (UBX_CFG_RATE_MEAS & 0xFF);
  packet[6 + 1]  = (UBX_CFG_RATE_MEAS >> 8) & 0xFF;
  packet[6 + 2]  = (UBX_CFG_RATE_NAV & 0xFF);
  packet[6 + 3]  = (UBX_CFG_RATE_NAV >> 8) & 0xFF;
  packet[6 + 4]  = (UBX_CFG_RAET_TIME & 0xFF);
  packet[6 + 5]  = (UBX_CFG_RAET_TIME >> 8) & 0xFF;
  checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
  gps_write(packet, sizeof(packet));
}

#define UBX_CFG_TP_PS          (u16)20
#define UBX_CFG_TP_INTERVAL    (u32)1000000
#define UBX_CFG_TP_LENGTH      (u32)1000
#define UBX_CFG_TP_STATUS      (s8)-1
#define UBX_CFG_TP_TIMEREF     (u8)0
#define UBX_CFG_TP_CABLE_DELAY (u16)50
#define UBX_CFG_TP_RF_DELAY    (u16)820
#define UBX_CFG_TP_USER_DELAY  (u32)0

static void set_ubx_cfg_tp()
{
  unsigned char packet[28];
  packet[0] = 0xB5;
  packet[1] = 0x62;
  packet[2] = 0x06;
  packet[3] = 0x07;
  packet[4 + 0]  = (UBX_CFG_TP_PS & 0xFF);
  packet[4 + 1]  = (UBX_CFG_TP_PS >> 8) & 0xFF;
  packet[6 + 0]  = (UBX_CFG_TP_INTERVAL & 0xFF);
  packet[6 + 1]  = (UBX_CFG_TP_INTERVAL >> 8) & 0xFF;
  packet[6 + 2]  = (UBX_CFG_TP_INTERVAL >> 16) & 0xFF;
  packet[6 + 3]  = (UBX_CFG_TP_INTERVAL >> 24) & 0xFF;
  packet[6 + 4]  = (UBX_CFG_TP_LENGTH & 0xFF);
  packet[6 + 5]  = (UBX_CFG_TP_LENGTH >> 8) & 0xFF;
  packet[6 + 6]  = (UBX_CFG_TP_LENGTH >> 16) & 0xFF;
  packet[6 + 7]  = (UBX_CFG_TP_LENGTH >> 24) & 0xFF;
  packet[6 + 8]  = (UBX_CFG_TP_STATUS & 0xFF);
  packet[6 + 9]  = (UBX_CFG_TP_TIMEREF & 0xFF);
  packet[6 + 10] = 0x00;
  packet[6 + 11] = 0x00;
  packet[6 + 12] = (UBX_CFG_TP_CABLE_DELAY & 0xFF);
  packet[6 + 13] = (UBX_CFG_TP_CABLE_DELAY >> 8) & 0xFF;
  packet[6 + 14] = (UBX_CFG_TP_RF_DELAY & 0xFF);
  packet[6 + 15] = (UBX_CFG_TP_RF_DELAY >> 8) & 0xFF;
  packet[6 + 16] = (UBX_CFG_TP_USER_DELAY & 0xFF);
  packet[6 + 17] = (UBX_CFG_TP_USER_DELAY >> 8) & 0xFF;
  packet[6 + 18] = (UBX_CFG_TP_USER_DELAY >> 16) & 0xFF;
  packet[6 + 19] = (UBX_CFG_TP_USER_DELAY >> 24) & 0xFF;
  checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
  gps_write(packet, sizeof(packet));
}

#define UBX_CFG_PRT_PS        (u16)20
#define UBX_CFG_PRT_BAUDRATE  (u32)115200

static void set_ubx_cfg_prt()
{
  // UBX
  unsigned char packet[UBX_CFG_PRT_PS + 8];
  packet[0] = 0xB5;
  packet[1] = 0x62;
  packet[2] = 0x06;
  packet[3] = 0x00;
  packet[4 + 0]  = (UBX_CFG_PRT_PS & 0xFF);
  packet[4 + 1]  = (UBX_CFG_PRT_PS >> 8) & 0xFF;
  packet[6 + 0]  = 0x00; // Port.NO
  packet[6 + 1]  = 0x00; // Res0
  packet[6 + 2]  = 0x00; // Res1
  packet[6 + 3]  = 0x00;
  packet[6 + 4]  = 0xC0; // (M)11000000(L)
  packet[6 + 5]  = 0x08; // (M)00001000(L)
  packet[6 + 6]  = 0x00; // (M)00000000(L)
  packet[6 + 7]  = 0x00; //
  packet[6 + 8]  = (UBX_CFG_PRT_BAUDRATE & 0xFF);
  packet[6 + 9]  = ((UBX_CFG_PRT_BAUDRATE >> 8) & 0xFF);
  packet[6 + 10] = ((UBX_CFG_PRT_BAUDRATE >> 16) & 0xFF);
  packet[6 + 11] = ((UBX_CFG_PRT_BAUDRATE >> 24) & 0xFF);
  packet[6 + 12] = 0x07; // in - UBX,NMEA,RAW
  packet[6 + 13] = 0x00;
  packet[6 + 14] = 0x01; // out - UBX
  packet[6 + 15] = 0x00;
  packet[6 + 16] = 0x00;
  packet[6 + 17] = 0x00;
  packet[6 + 18] = 0x00;
  packet[6 + 19] = 0x00;
  
  //checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
  //gps_write(packet, sizeof(packet));
  
  {
    u8 i;
    for(i = 0; i < 3; i++){
      packet[6 + 0]  = i;
      checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
      gps_write(packet, sizeof(packet));
    }
  }
  
  // NMEA
  /*const char code *str1 = "$PUBX,41,0,0007,0003,115200,1*1A\r\n";
  const char code *str2 = "$PUBX,41,1,0007,0003,115200,1*1B\r\n";
  const char code *str3 = "$PUBX,41,2,0007,0003,115200,1*18\r\n";  
  gps_write(str1, strlen(str1));
  gps_write(str2, strlen(str2));  
  gps_write(str3, strlen(str3));*/
}

#define UBX_CFG_MSG_PS    (u16)6

static void set_ubx_cfg_msg(u8 _class, u8 id, u8 rate1)
{
  unsigned char packet[14];
  packet[0] = 0xB5;
  packet[1] = 0x62;
  packet[2] = 0x06;
  packet[3] = 0x01;
  packet[4 + 0]  = (UBX_CFG_MSG_PS & 0xFF);
  packet[4 + 1]  = (UBX_CFG_MSG_PS >> 8) & 0xFF;
  packet[6 + 0]  = _class;
  packet[6 + 1]  = id;
  packet[6 + 2]  = rate1;//(u8)0;
  packet[6 + 3]  = rate1;
  packet[6 + 4]  = rate1;//(u8)0;
  packet[6 + 5]  = (u8)0;
  checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
  gps_write(packet, sizeof(packet));
}

void gps_init(void){
  
  // init wait
  wait_ms(100);
  
  // set U-blox configuration
  set_ubx_cfg_prt();              // baudrate change
  while(uart0_tx_size() > 0);
  uart0_bauding(UBX_CFG_PRT_BAUDRATE);
  
  // baudrate change wait
  wait_ms(100);
  
  // clear buffer
  {
    char c;
    while(uart0_read(&c, 1) > 0);
  }
  
  set_ubx_cfg_rate();
  set_ubx_cfg_tp();
  
  set_ubx_cfg_msg(0x01, 0x02, 1);  // NAV-POSLLH  // 28 + 8 = 36 bytes
  set_ubx_cfg_msg(0x01, 0x03, 4);  // NAV-STATUS  // 16 + 8 = 24 bytes
  set_ubx_cfg_msg(0x01, 0x04, 4);  // NAV-DOP     // 18 + 8 = 26 bytes
  set_ubx_cfg_msg(0x01, 0x06, 1);  // NAV-SOL     // 52 + 8 = 60 bytes
  set_ubx_cfg_msg(0x01, 0x12, 1);  // NAV-VELNED  // 36 + 8 = 44 bytes
  set_ubx_cfg_msg(0x01, 0x30, 8);  // NAV-SVINFO  // (8 + 12 * x) + 8 = 112 bytes (@8)
  set_ubx_cfg_msg(0x02, 0x10, 1);  // RXM-RAW     // (8 + 24 * x) + 8 = 208 bytes (@8)
  //set_ubx_cfg_msg(0x02, 0x11, 8);  // RXM-SFRB    // 42 + 8 = 50 bytes
  
  // NMEA-GGA
  // NMEA-GLL
  // NMEA-GSA
  // NMEA-GSV
  // NMEA-RMC
  // NMEA-VTG
  // NMEA-ZDA
  
	// Time Pulse Interrupt config (-INT0)
  IT0 = 1;    // Edge sense
  //PX0 = 1;    // Proiority High
  EX0 = 1;    // Enable
}

volatile bit gps_ms_modified = FALSE;
u8 gps_num_of_sat = 0;
s32 gps_ms = 0;

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#define UBX_POLL_RXM_EPH    (u16)1
#define UBX_POLL_RXM_FREQ   16

static void poll_rxm_eph(u8 svid){
  unsigned char packet[9];
  packet[0] = 0xB5;
  packet[1] = 0x62;
  packet[2] = 0x02;
  packet[3] = 0x31;
  packet[4 + 0]  = (UBX_POLL_RXM_EPH & 0xFF);
  packet[4 + 1]  = (UBX_POLL_RXM_EPH >> 8) & 0xFF;
  packet[6 + 0]  = svid;
  checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
  gps_write(packet, sizeof(packet));
}

#define UBX_POLL_AID_HUI    (u16)0

static void poll_aid_hui(){
  unsigned char packet[8];
  packet[0] = 0xB5;
  packet[1] = 0x62;
  packet[2] = 0x0B;
  packet[3] = 0x02;
  packet[4 + 0]  = (UBX_POLL_AID_HUI & 0xFF);
  packet[4 + 1]  = (UBX_POLL_AID_HUI >> 8) & 0xFF;
  checksum(&(packet[sizeof(packet) - 2]), &(packet[sizeof(packet) - 1]), &(packet[2]), sizeof(packet) - 4);
  gps_write(packet, sizeof(packet));
}

#define UBX_SAT_MAX_ID 32
static void make_packet(packet_t *packet){
  unsigned char size;
  
  // Is gps_packet available
  if(packet->current >= packet->buf_end){return;}
  
  *((packet->current)++) = 'G';
    
  // packet‚Ö‚Ì“o˜^
  size = uart0_read(packet->current, packet->buf_end - packet->current);
    
  // GPSŽž‚ÌŽæ“¾, ‰ÂŽ‹‰q¯‚É‘Î‚·‚éƒGƒtƒFƒƒŠƒX‚Ì—v‹
  while(size-- > 0){
    u8 c = *((packet->current)++);
    static xdata u8 ubx_index = 0;
    switch(ubx_index){
      case 0: if(c == 0xB5){ubx_index = 1;} break;
      case 1: ubx_index = (c == 0x62) ? 2 : 0; break;
      case 2: ubx_index = (c == 0x02) ? 3 : 0; break;
      case 3: ubx_index = (c == 0x10) ? 4 : 0; break;
      default: {
        switch(++ubx_index){
          case 7:
            gps_ms_modified = FALSE;
            *((u8 *)((u8 *)&gps_ms)) = c;  
            break;
          case 8:
          case 9:
            *((u8 *)(((u8 *)&gps_ms) + (ubx_index - 7))) = c;  
            break;
          case 10:
            *((u8 *)(((u8 *)&gps_ms) + 3)) = c;
            if(gps_ms > 0) gps_ms = ((gps_ms / 1000) + 1) * 1000;
            break;
          case 13:
            gps_num_of_sat = c;
            gps_ms_modified = TRUE;
            {
              static xdata u8 epf_timing = UBX_POLL_RXM_FREQ;
              static xdata u8 sv_eph_selector = 0;
              if(--epf_timing == 0){
                epf_timing = UBX_POLL_RXM_FREQ;
                poll_rxm_eph(++sv_eph_selector);
                if(sv_eph_selector == UBX_SAT_MAX_ID){
                  poll_aid_hui();
                  sv_eph_selector = 0;
                }
              }
            }
            ubx_index = 0;
            break;
        }
      }
    }
  }
  
  spi_dsp_write(packet->buf_begin, (packet->current - packet->buf_begin));
}

void gps_polling(){
  u8 buf_size = uart0_rx_size();
  for(; buf_size >= (PAGE_SIZE - 1); buf_size -= (PAGE_SIZE - 1)){
    if(!com_manager_assign_page(make_packet)){break;}
  }
}
