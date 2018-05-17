//-----------------------------------------------------------------------------
// F34x_MSD_MMC.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// MMC FLASH is used for storing the log entries.  Each entry contains
// the temperature in hundredths of a degree C, the day, hour, minute, and
// second that the reading was taken.  The LogUpdate function stores log
// entries in an external memory buffer and then writes that buffer out to the
// MMC when it is full.  Communication with the MMC is performed through the 
// MMC access functions.  These functions provide transparent MMC access to 
// the higher level functions (Logging functions).  The MMC interface is broken
// into two pieces.  The high level piece consists of the user callable MMC
// access functions (MMC_FLASH_Read, MMC_FLASH_Write, MMC_FLASH_Clear, 
// MMC_FLASH_MassErase).  These functions are called by the user to execute
// data operations on the MMC.  They break down the data operations into MMC
// commands.  The low level piece consists of a single command execution
// function (MMC_Command_Exec) which is called by the MMC data manipulation
// functions.  This function is called every time a command must be sent to the
// MMC.  It handles all of the required SPI traffic between the Silicon 
// Laboratories device and the MMC.
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000043
// Target:         C8051F34x
// Tool chain:     Keil
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_MSD
//
// Release 1.1
//    -All changes by PKC
//    -09 JUN 2006
//    -Replaced SFR definitions file "c8051f320.h" with "c8051f340.h"
//    -Corrected "SPIDAT" to "SPI0DAT" in function spi_write_read_byte
//
// Release 1.0
//    -Initial Release
//


#include "c8051f340.h"                 // SFR declarations
#include <stdio.h>
#include "main.h"					// Has SYSCLK #define'd
#include "MMC.h"
#include "util.h"
#include "type.h"
#include "f34x_spi.h"
//#include <intrins.h>

// Command table value definitions
// Used in the MMC_Command_Exec function to 
// decode and execute MMC command requests
#define     EMPTY  0
#define     YES   1
#define     NO    0
#define     CMD   0
#define     RD    1
#define     WR    2
#define     R1    0
#define     R1b   1
#define     R2    2
#define     R3    3
#define     R7    4

// Start and stop data tokens for single and multiple
// block MMC data operations
#define     START_SBR      0xFE
#define     START_MBR      0xFE
#define     START_SBW      0xFE
#define     START_MBW      0xFC
#define     STOP_MBW       0xFD

// Mask for data response Token after an MMC write
#define     DATA_RESP_MASK 0x1F

// Mask for busy Token in R1b response
#define     BUSY_BIT       0x80

// byte-addressable INT
typedef union{
  int i;
  unsigned char b[2];
} ba_int_t;

// byte-addressable unsigned int
typedef union {
  unsigned int i;
  unsigned char b[2];
} ba_uint_t;

// byte-addressable LONG
typedef union LONG {
  long l;
  unsigned char b[4];
} ba_long_t;

// byte-addressable unsigned long
typedef union {
  unsigned long l;
  unsigned char b[4];
} ba_ulong_t;

// This structure defines entries into the command table;
typedef struct {
  unsigned char command_index;     // OpCode;
  unsigned char arg_required;      // Indicates argument requirement;
  unsigned char CRC;               // Holds CRC for command if necessary;
  unsigned char trans_type;        // Indicates command transfer type;
  unsigned char response;          // Indicates expected response;
  unsigned char var_length;        // Indicates varialble length transfer;
} command_t;

#define ACMD(x) ((x) | 0x40)
#define IS_ACMD(x) ((x) & 0x40)

// Command table for MMC.  This table contains all commands available in SPI
// mode;  Format of command entries is described above in command structure
// definition;
command_t code command_list[] = {
    { 0,NO ,0x95,CMD,R1 ,NO },    // CMD0;  GO_IDLE_STATE: reset card;
    { 1,NO ,0xFF,CMD,R1 ,NO },    // CMD1;  SEND_OP_COND: initialize card;
    { 9,NO ,0xFF,RD ,R1 ,NO },    // CMD9;  SEND_CSD: get card specific data;
    {10,NO ,0xFF,RD ,R1 ,NO },    // CMD10; SEND_CID: get card identifier;
    {12,NO ,0xFF,CMD,R1 ,NO },    // CMD12; STOP_TRANSMISSION: end read;
    {13,NO ,0xFF,CMD,R2 ,NO },    // CMD13; SEND_STATUS: read card status;
    {16,YES,0xFF,CMD,R1 ,NO },    // CMD16; SET_BLOCKLEN: set block size;
    {17,YES,0xFF,RD ,R1 ,NO },    // CMD17; READ_SINGLE_BLOCK: read 1 block;
    {18,YES,0xFF,RD ,R1 ,YES},    // CMD18; READ_MULTIPLE_BLOCK: read > 1;
    {24,YES,0xFF,WR ,R1 ,NO },    // CMD24; WRITE_BLOCK: write 1 block;
    {25,YES,0xFF,WR ,R1 ,YES},    // CMD25; WRITE_MULTIPLE_BLOCK: write > 1;
    {27,NO ,0xFF,CMD,R1 ,NO },    // CMD27; PROGRAM_CSD: program CSD;
    {28,YES,0xFF,CMD,R1b,NO },    // CMD28; SET_WRITE_PROT: set wp for group;
    {29,YES,0xFF,CMD,R1b,NO },    // CMD29; CLR_WRITE_PROT: clear group wp;
    {30,YES,0xFF,CMD,R1 ,NO },    // CMD30; SEND_WRITE_PROT: check wp status;
    {32,YES,0xFF,CMD,R1 ,NO },    // CMD32; TAG_SECTOR_START: tag 1st erase;
    {33,YES,0xFF,CMD,R1 ,NO },    // CMD33; TAG_SECTOR_END: tag end(single);
    {34,YES,0xFF,CMD,R1 ,NO },    // CMD34; UNTAG_SECTOR: deselect for erase;
    {35,YES,0xFF,CMD,R1 ,NO },    // CMD35; TAG_ERASE_GROUP_START;
    {36,YES,0xFF,CMD,R1 ,NO },    // CMD36; TAG_ERASE_GROUP_END;
    {37,YES,0xFF,CMD,R1 ,NO },    // CMD37; UNTAG_ERASE_GROUP;
    {38,YES,0xFF,CMD,R1b,NO },    // CMD38; ERASE: erase all tagged sectors;
    {42,YES,0xFF,CMD,R1b,NO },    // CMD42; LOCK_UNLOCK;
    {55,NO ,0xFF,CMD,R1 ,NO },    // CMD55; APP_CMD;
    {ACMD(41),YES,0xFF,CMD,R1 ,NO },    // ACMD41; APP_SEND_OP_CMD;
    {58,NO ,0xFF,CMD,R3 ,NO },    // CMD58; READ_OCR: read OCR register;
    {59,YES,0xFF,CMD,R1 ,NO },    // CMD59; CRC_ON_OFF: toggles CRC checking;
    { 8,YES,0x87,CMD,R7 ,NO },    // CMD8;  SEND_IF_COND: Sends SD Memory Card interface condition;
  };

// Command Table Index Constants:
// Definitions for each table entry in the command table.
// These allow the MMC_Command_Exec function to be called with a
// meaningful parameter rather than a number.
enum {
  GO_IDLE_STATE,
  SEND_OP_COND,
  SEND_CSD,
  SEND_CID,
  STOP_TRANSMISSION,
  SEND_STATUS,
  SET_BLOCKLEN,
  READ_SINGLE_BLOCK,
  READ_MULTIPLE_BLOCK,
  WRITE_BLOCK,
  WRITE_MULTIPLE_BLOCK,
  PROGRAM_CSD,
  SET_WRITE_PROT,
  CLR_WRITE_PROT,
  SEND_WRITE_PROT,
  TAG_SECTOR_START,
  TAG_SECTOR_END,
  UNTAG_SECTOR,
  TAG_ERASE_GROUP_START,
  TAG_ERASE_GROUP_END,
  UNTAG_ERASE_GROUP,
  ERASE,
  LOCK_UNLOCK,
  APP_CMD,
  APP_SEND_OP_CMD,
  READ_OCR,
  CRC_ON_OFF,
  SEND_IF_COND
};

// MMC size variable;  Set during initialization;
xdata unsigned long MMC_physical_size = 0;

// MMC block number;  Computed during initialization;
xdata unsigned long MMC_physical_sectors = 0;

bit MMC_initialized = FALSE; 
static bit require_busy_check = FALSE;
static bit block_addressing = 0;
static bit sdhc = 0;

#define MMC_WAIT 14 //for wait_10n4clk // 144 / 48 = 3us

#define select_MMC() {NSSMD0 = 0;}
#define deselect_MMC() {NSSMD0 = 1;}

#define raise_exception(code) \
{\
  spi_send_8clock(); \
  deselect_MMC(); \
  spi_send_8clock(); \
  return code; \
}

unsigned char MMC_Flush() {
  if(require_busy_check){
    require_busy_check = FALSE;
    
    // Send buffer SPI clocks to ensure no MMC operations are pending;
    spi_send_8clock();
    select_MMC();
    spi_send_8clock();
    
    /*
     * wait for end of busy signal;
     * 
     * Start SPI transfer to receive busy tokens;
     * When a non-zero Token is returned,
     * card is no longer busy;
     */
    while(spi_write_read_byte(0xFF) == 0x00);
    
    deselect_MMC();
    /*
     * Send 8 more SPI clocks to ensure
     * the card has finished all necessary operations; 
     */
    spi_send_8clock();
  }
  return MMC_NORMAL_CODE;
}

/**
 * This function generates the necessary SPI traffic for all MMC SPI commands.
 * The three parameters are described below:
 * 
 * Cmd:      This parameter is used to index into the command table and read
 *           the desired command.  The Command Table Index Constants allow the
 *           caller to use a meaningful constant name in the Cmd parameter 
 *           instead of a simple index number.  For example, instead of calling 
 *           MMC_Command_Exec (0, argument, pchar) to send the MMC into idle 
 *           state, the user can call 
 *           MMC_Command_Exec (GO_IDLE_STATE, argument, pchar);
 *
 * argument: This parameter is used for MMC commands that require an argument.
 *           MMC arguments are 32-bits long and can be values such as an
 *           an address, a block length setting, or register settings for the
 *           MMC.
 *
 * pchar:    This parameter is a pointer to the local data location for MMC 
 *           data operations.  When a read or write occurs, data will be stored
 *           or retrieved from the location pointed to by pchar.
 *
 * The MMC_Command_Exec function indexes the command table using the Cmd 
 * parameter. It reads the command table entry into memory and uses information
 * from that entry to determine how to proceed.  Returns the 16-bit card 
 * response value;
 */
static unsigned char MMC_Command_Exec (
  unsigned char cmd_index, 
  unsigned long argument,
  unsigned char *pchar
){
  
  // current data block length;
  static xdata unsigned int current_blklen = 512;
  
  unsigned char loopguard;
  
  // Byte counter for multi-byte fields;
  unsigned char counter;
  
  // Local space for the command table entry;
  command_t code *current_command = &command_list[cmd_index]; 
  
  // Temp variable to preserve data block length during temporary changes;
  unsigned int old_blklen;
  
  // Variable for storing card res;
  unsigned char res;
  
  if(IS_ACMD(current_command->command_index)){
    MMC_Command_Exec(APP_CMD,EMPTY,EMPTY);
  }

  // Send buffer SPI clocks to ensure no MMC operations are pending;
  spi_send_8clock();
  select_MMC();
  // Send another byte of SPI clocks;
  spi_send_8clock();
  
  if(require_busy_check){
    require_busy_check = FALSE;
    
    /*
     * wait for end of busy signal;
     * 
     * Start SPI transfer to receive busy tokens;
     * When a non-zero Token is returned,
     * card is no longer busy;
     */
    while(spi_write_read_byte(0xFF) == 0x00);
  }
  
  // Issue command opcode;
  spi_write_read_byte(current_command->command_index | 0x40);
  
  old_blklen = current_blklen;
  switch(current_command->command_index){
    /*
     * If current command changes block length, update block length variable
     * to keep track;
     * 
     * Command byte = 16 means that a set block length command is taking place
     * and block length variable must be set;
     */
    case 16:
      current_blklen = argument;
      break;
    
    /*
     * Command byte = 9 or 10 means that a 16-byte register value is being read
     * from the card, block length must be set to 16 bytes, and restored at the
     * end of the transfer;
     * 
     * Command is a GET_CSD or GET_CID, set block length to 16-bytes;
     */
    case 9:
    case 10:
      current_blklen = 16;
      break;
    
    /*
     * read or write block(s)
     */
    case 17:
    case 18:
    case 24:
    case 25:
      if(!block_addressing){argument *= PHYSICAL_BLOCK_SIZE;}
      break;
  }
  
  /*
   * If an argument is required, transmit one, 
   * otherwise transmit 4 bytes of 0x00; 
   */
  counter = 4;
  if(current_command->arg_required == YES) {
    
    // Union variable for easy byte transfers of the argument;
    ba_ulong_t long_arg; 
    // Make argument byte addressable;
    long_arg.l = argument;
    do {
	    spi_write_read_byte(long_arg.b[--counter]);
    } while(counter);
  } else {
    do{spi_write_read_byte(0x00);}while(--counter);
  }
  
  /*
   * Transmit CRC byte;  In all cases except CMD0,
   * this will be a dummy character;
   */
  spi_write_read_byte(current_command->CRC);
  
    // read the response
  loopguard = 0;
  while((res = spi_write_read_byte(0xFF)) & BUSY_BIT){
    if((++loopguard) == 0){
      raise_exception(res);
    }
  }
  
  /*
   * The command table entry will indicate what type of response to expect for
   * a given command;  The following conditional handles the MMC response;
   */
  switch(current_command->response){
    case R1:
      // Read the R1 response from the card;
      break;
    case R1b:
      // Read the R1b response;
      /*
       * wait for busy signal to end;
       * 
       * When byte from card is non-zero,
       * card is no longer busy;
       */
      loopguard = 0;
      while((res = spi_write_read_byte(0xff)) == 0x00){
        wait_10n4clk(MMC_WAIT);
        if((++loopguard) == 0){
          raise_exception(res);
        }
      }
      break;
    case R2:
      // Read R2 response
      // Read second byte of response;
      spi_write_read_byte(0xFF);
      break;
    case R3:
    case R7:
      // Read R3, R7 response;
      /*
       * Read next three bytes and store them
       * in local memory;  These bytes make up
       * the Operating Conditions Register
       */
      counter = 4;
      while(counter-- > 0){ 
        *(pchar++) = spi_write_read_byte(0xFF); // (OCR);
      }
      break;
    default:
      raise_exception(MMC_ERROR_CODE);
  }
  
  /*
   * This conditional handles all data operations;  The command entry
   * determines what type, if any, data operations need to occur;
   * Read data from the MMC;
   */
  switch(current_command->trans_type){
    case WR:{
        /*
         * Write data to the MMC;
         * Start by sending 8 SPI clocks so the MMC can prepare for the write;
         */
        spi_send_8clock();
        spi_write_read_byte(START_SBW);
        
        spi_write(pchar, current_blklen);  // Write <current_blklen> bytes to MMC;
        
        // Write CRC bytes (don't cares);
        spi_write_read_byte(0xFF);
        spi_write_read_byte(0xFF);
        
        /*
         * Read Data Response from card;
         * 
         * When bit 0 of the MMC response is clear, a valid data response
         * has been received;
         */
        if((spi_write_read_byte(0xFF) & DATA_RESP_MASK) != 0x05){
          raise_exception(MMC_ERROR_CODE);
        }
        spi_send_8clock();
        if(spi_write_read_byte(0xFF) == 0x00){
          require_busy_check = TRUE;
        }
        break;
      } 
    case RD:{
        unsigned char data_res;
        loopguard = 0;
        /*
         * wait for a start read Token from the MMC
         */
        loopguard = tickcount & 0xFF;
        while((data_res = spi_write_read_byte(0xFF)) != START_SBR) {
          u8 loopguard2 = tickcount & 0xFF;
          wait_10n4clk(MMC_WAIT);
          if((data_res != 0xFF) || (((u8)(loopguard2 - loopguard)) >= 0x80)) { 
            raise_exception(MMC_ERROR_CODE);
          }
        }
        
        spi_read(pchar, current_blklen);
      
        /*
         * After all data is read, read the two
         * CRC bytes;  These bytes are not used
         * in this mode, but the placeholders
         * must be read anyway;
         */
        spi_write_read_byte(0xFF);
        spi_write_read_byte(0xFF);
        break;
      }
  }
  
  deselect_MMC();
  /*
   * Send 8 more SPI clocks to ensure
   * the card has finished all necessary operations; 
   */
  spi_send_8clock();
  
  // Restore old block length if needed;
  if((current_command->command_index == 9)||
    (current_command->command_index == 10)) {
    current_blklen = old_blklen;
  }
  return res;
}


/**
 * Xdata pointer for storing MMC
 * register values;
 * Transmit at least 64 SPI clocks
 * before any bus comm occurs.
 */
static xdata unsigned char buffer[16];

/**
 * This function initializes the flash card, configures it to operate in SPI
 * mode, and reads the operating conditions register to ensure that the device
 * has initialized correctly.  It also determines the size of the card by
 * reading the Card Specific Data Register (CSD).
 */
void MMC_FLASH_Init(){
  
  unsigned char loopguard;
  
  unsigned char res;
  unsigned char counter;  // SPI byte counter;

  unsigned int c_size,bl_len;
  unsigned char c_mult;
  
  if(MMC_initialized){return;}
  
  deselect_MMC();
  wait_ms(100);
  
  /*
   * Select the MMC with the CS pin;
   * Send 74 more SPI clocks to ensure proper startup;
   */
  counter = 10;
  while(counter--){spi_write_read_byte(0xFF);}
  
  select_MMC(); 
 	
  wait_ms(1);
  
  /*
   * Send the GO_IDLE_STATE command with CS driven low;
   * This causes the MMC to enter SPI mode;
   */
  MMC_Command_Exec(GO_IDLE_STATE,EMPTY,EMPTY);

  loopguard=0;
  if(MMC_Command_Exec(SEND_IF_COND, 0x01AA, buffer) == 1) {
    /* SDHC */
    sdhc = 1;
    if((buffer[2] == 0x01) && (buffer[3] == 0xAA)){
      /* The card can work at vdd range of 2.7-3.6V */
      /* Wait for leaving idle state (ACMD41 with HCS bit) */
      while(MMC_Command_Exec(APP_SEND_OP_CMD, 0x40000000UL, EMPTY)){
        wait_ms(1);
        if((++loopguard) == 0){return;}
      }
      if(MMC_Command_Exec(READ_OCR, EMPTY, buffer) == 0){
        /* Check CCS bit in the OCR */
        if(buffer[0] & 0x40){block_addressing = 1;}
      }
    }
  }else{
    /* SDSC or MMC */
    unsigned char cmd; 
    if(MMC_Command_Exec(APP_SEND_OP_CMD, EMPTY, EMPTY) <= 1){
      /* SDSC */
      cmd = APP_SEND_OP_CMD;
    }else{
      /* MMC */
      cmd = SEND_OP_COND;
    }
    /* Wait for leaving idle state */
    while(MMC_Command_Exec(cmd, 0, EMPTY)){
      wait_ms(1);
      if((++loopguard) == 0){return;}
    }
    /* Set R/W block length to 512 */
    if(MMC_Command_Exec(SET_BLOCKLEN, (unsigned long)PHYSICAL_BLOCK_SIZE, EMPTY) != 0){
      return;
    }
  }
  
  /*
   * Get the Card Specific Data (CSD)
   * register to determine the size of the MMC;
   */
  res = MMC_Command_Exec(SEND_STATUS,EMPTY,EMPTY);

  res = MMC_Command_Exec(SEND_CSD,EMPTY,buffer);
	
  if(res != 0) {return;}

  switch(buffer[0] & 0xC0){
    case (0x01 << 6):
      bl_len = 512;
      MMC_physical_sectors = ((unsigned long)(buffer[7] & 0x3F) << 16)
          | (buffer[8] << 8) | buffer[9];
      MMC_physical_sectors++;
      MMC_physical_sectors <<= 10;
      break;
    case (0x00 << 6):
      bl_len = 1 << (buffer[5] & 0x0f);
      c_size = ((buffer[6] & 0x03) << 10) | 
          (buffer[7] << 2) | ((buffer[8] & 0xc0) >> 6);
      c_mult = (((buffer[9] & 0x03) << 1) | ((buffer[10] & 0x80) >> 7));
      
      // Determine the number of MMC sectors;
      MMC_physical_sectors = (unsigned long)(c_size+1)*(1 << (c_mult+2));
      break;
    default:
      return;
  }
  /* バイト-ビット対応表
   *  0 127-120
   *  1 119-112
   *  2 111-104
   *  3 110-96
   *  4 95-88
   *  5 87-80
   *  6 79-72
   *  7 71-64
   *  8 63-56
   *  9 55-48
   * 10 47-40
   * 11 39-32
   * 12 31-24
   * 13 23-16
   * 14 15-8
   * 15 7-0
   */
  MMC_physical_size = MMC_physical_sectors * bl_len;

  MMC_initialized = TRUE;
}

/**
 * If you know beforehand that you'll read an entire 512-byte block, then
 * this function has a smaller ROM footprint than MMC_FLASH_Read.
 * 
 * @param address address of block
 * @param pchar pointer to byte
 * @return card status
 */
unsigned char MMC_FLASH_Block_Read(
    unsigned long address, 
    unsigned char *pchar){
  return MMC_Command_Exec(READ_SINGLE_BLOCK, address, pchar);
}

/**
 * If you know beforehand that you'll write an entire 512-byte block, then
 * this function is more RAM-efficient than MMC_FLASH_Write because it
 * doesn't require a 512-byte buffer (and it's faster too, it doesn't
 * require a read operation first). And it has a smaller ROM footprint too.
 * 
 * @param address address of block
 * @param wdata pointer to data
 * @return card status
 */
unsigned char MMC_FLASH_Block_Write(
    unsigned long address, 
    unsigned char *wdata){
  return MMC_Command_Exec(WRITE_BLOCK, address, wdata);
}

/**
 * Function returns the status of MMC card
 * 
 */
unsigned char MMC_Get_Status(){
  return MMC_Command_Exec(SEND_STATUS, EMPTY, EMPTY);	
}
