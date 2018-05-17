//-----------------------------------------------------------------------------
// F34x_MSD_Sect_Serv.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains basic functions for file system control. These low level 
// functions are for FAT file system.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000053
// Target:         C8051F34x
// Tool chain:     Keil
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_MSD
//
// Release 1.1
//    -All changes by PKC
//    -09 JUN 2006
//    -No changes; incremented revision number to match project revision
//
// Release 1.0
//    -Initial Release
//

#include "Sect_Serv.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include "MMC.h"

#define CF_USE 0

#if CF_USE
#include "F34x_MSD_CF_Basic_Functions.h"
#endif

#if CF_USE
static xdata char Is_Compact_Flash; //CF Support
#endif

// Buffer for read/write transfers:
BYTE xdata Scratch[PHYSICAL_BLOCK_SIZE];

typedef struct {
  BYTE checkRoutionOnx86[446];
  struct {
    BYTE bootDescriptor;             /* 0x80: bootable device, 0x00: non-bootable */
    BYTE firstPartitionSector[3];    /* 1st sector number */
    BYTE fileSystemDescriptor;       /* 1:FAT12, 4:FAT16(less than 32MB), 5:Šg’£ DOS ƒp[ƒeƒBƒVƒ‡ƒ“,
                                        6:FAT16(more 32MB), 0xb:FAT32(more 2GB),
                                        0xc:FAT32 Int32h Šg’£, 0xe:FAT16 Int32h Šg’£,
                                        0xf:Šg’£ DOS ƒp[ƒeƒBƒVƒ‡ƒ“‚Ì Int32h Šg’£ */
    BYTE lastPartitionSector[3];
    unsigned firstSectorNumbers[2]; /* first sector number (link to BPB sector) */
    BYTE numberOfSectors[4];
  } partitionTable[4];
  BYTE signature[2];        // 0x55 0xAA
} mbr_t;

typedef struct {
  BYTE jmp[3];
  char oem_name[8];             
  unsigned bytes_per_sector;
  BYTE sectors_per_cluster;
  unsigned reserved_sectors;
  BYTE fat_copies;
  unsigned root_directory_entries;
  unsigned number_of_sectors;
  BYTE media_descriptor;
  unsigned sectors_per_fat;
  unsigned sectors_per_track;
  unsigned heads;
  DWORD hidden_sectors;
  DWORD total_sectors;
  union {
    struct {
      BYTE drive_number;
      BYTE reserved;
      BYTE extended_signature;    // 0x29
      BYTE serial_number[4];
      char volume_label[11];
      char filesystem[8];        // "FAT16   "
      BYTE bootstrap[448];
    } FAT16;
    struct {
      DWORD   bigSectorsPerFAT;       /* sector/FAT for FAT32 */
      BYTE    extFlags[2];            /* use index zero (follows) */
                                      /* bit 7      0: enable FAT mirroring, 1: disable mirroring */
                                      /* bit 0-3    active FAT number (bit 7 is 1) */
      BYTE    FS_Version[2];
      BYTE    rootDirStrtClus[4];     /* root directory cluster */
      BYTE    FSInfoSec[2];           /* 0xffff: no FSINFO, other: FSINFO sector */
      BYTE    bkUpBootSec[2];         /* 0xffff: no back-up, other: back up boot sector number */
      BYTE    reserved[12];
      /* info */
      BYTE    driveNumber;
      BYTE    unused;
      BYTE    extBootSignature;
      BYTE    serialNumber[4];
      BYTE    volumeLabel[11];
      BYTE    filesystem[8];      /* "FAT32   " */
      BYTE    loadProgramCode[420];
    } FAT32;
  } specified;
  BYTE signature[2];        // 0x55 0xAA
} bpb_t;

// This small bootrecord will be in memory as long as the program is running
typedef struct {
  BYTE valid;
  BYTE fat_copies;
  unsigned root_directory_entries;
  unsigned number_of_sectors;
  DWORD sectors_per_fat;
  DWORD total_sectors;
  unsigned reserved_sectors;
  unsigned hidden_sectors;
  BYTE sectors_per_cluster;
} bootrecord_t;

// Permanent copy of important fields from bootrecord:
static bootrecord_t xdata bootrecord;

extern xdata unsigned long PHYSICAL_BLOCKS;
// MMC-specific:
#define HIDDEN_SECTORS 0x00 
//0x20
// Size of an entry in the root directory
#define DIRENTRY_SIZE 0x20


extern void print_scratch();

/**
 * Sect_Validate
 * 
 * Function checks the validate of bootrecord.
 * 
 */
static void Sect_Validate() reentrant {
  unsigned fat_sect = 0;

  const mbr_t *mbr = (mbr_t *)Scratch;
  const bpb_t *bpb = (bpb_t *)Scratch;
  bootrecord.valid = 0;
  bootrecord.hidden_sectors = 0;

  if((memcmp(mbr->signature, "\x55\xAA", 2) != 0)) {
    return;
  }
  if(PHYSICAL_BLOCK_SIZE != ntohs(bpb->bytes_per_sector)){
    if((mbr->partitionTable[0].bootDescriptor & 0x7F) == 0x00){
      fat_sect = mbr->partitionTable[0].firstSectorNumbers[0];
      Sect_Read(fat_sect);
      bootrecord.hidden_sectors = fat_sect;
    }else{
      return;
    }
  }
  
  if(bootrecord.sectors_per_fat = ntohs(bpb->sectors_per_fat)){
    // FAT16
    if(memcmp(bpb->specified.FAT16.filesystem, "FAT16", 5) != 0){return;}
  }else{
    // FAT32
    if(memcmp(bpb->specified.FAT32.filesystem, "FAT32", 5) != 0){return;}
    bootrecord.sectors_per_fat = bpb->specified.FAT32.bigSectorsPerFAT;
  }

	// Make a permanent copy of the important fields of the bpb:
  bootrecord.fat_copies = bpb->fat_copies;
  bootrecord.root_directory_entries = ntohs(bpb->root_directory_entries);
  bootrecord.number_of_sectors = ntohs(bpb->number_of_sectors);
  bootrecord.total_sectors = ntohl(bpb->total_sectors);
  bootrecord.reserved_sectors = ntohs(bpb->reserved_sectors);
  bootrecord.sectors_per_cluster = bpb->sectors_per_cluster;
  bootrecord.valid = 1;
}

/**
 * Sect_Init
 * 
 * Function initializes memory card (Compact Flash or MMC), reads sector 0 and 
 * checks the validate of bootrecord.
 * 
 */
void Sect_Init(){
  unsigned char /*xdata*/ time_out = 0;
#if CF_USE
#ifdef __F340_VER__
  int /*xdata*/ sizel,sizeh;
  Is_Compact_Flash = 0;
  if(Init_CF() != CF_NO_CARD) {
    if(Identify_Drive(Scratch)!= CF_NO_CARD)
    {
      Is_Compact_Flash = 1;
      sizel = (Scratch[115] << 8) | Scratch[114];
      sizeh = (Scratch[117] << 8) | Scratch[116];
      PHYSICAL_BLOCKS = ((unsigned long)sizeh << 16 ) | (sizel&0x0ffff);
    }
  }
  if(!Is_Compact_Flash) {
#else
    Is_Compact_Flash = 0;
#endif
#endif
    MMC_FLASH_Init();
#if CF_USE 
#ifdef __F340_VER__
  }
#endif
#endif
	Sect_Read(0);	
	Sect_Validate();
}

/**
 * Returns number of sectors.
 * 
 */
unsigned long Sect_Sectors(){
  return PHYSICAL_BLOCKS;//bootrecord.number_of_sectors;
}

/**
 * Sect_Print
 * 
 * Dumps some information (size, bootrecord, filesystem, etc.)
 * 
 */
void Sect_Print(){
  mbr_t* mbr = (mbr_t *)Scratch;
  if(!bootrecord.valid){
    //printf("ERROR: Bootrecord invalid." ENDLINE);
    return;
  }

  /*printf("%s size = %lu bytes" ENDLINE,
#if CF_USE
          Is_Compact_Flash ? "CF memory" : "Memory",
#else
          "Memory",
#endif // CF_USE
          (DWORD)Sect_Sectors() * Sect_Block_Size());*/

}

/**
 * Sect_Read
 * 
 * Reads one sector into Scratch buffer
 * 
 * @return error number
 */
unsigned  Sect_Read(unsigned long sector){
  unsigned /*xdata*/ error;
#if CF_USE
#ifdef __F340_VER__
  if(!Is_Compact_Flash) {
#endif // __F340_VER__
#endif // CF_USE
    unsigned char /*xdata*/ loopguard = 0;
    while((error = MMC_FLASH_Block_Read(sector+HIDDEN_SECTORS,Scratch)) != 0) 
    if(!++loopguard) {
      //printf("Unable to Read sector %ld" ENDLINE , sector);
      break;
    }
#if CF_USE
#ifdef __F340_VER__
  } else {
  	_CF_Sector_Address = sector;
    _CF_Sector_Buffer = Scratch;
    error = Read_Sector();
  }
#endif // __F340_VER__
#endif // CF_USE
  return error;
}

/**
 * Sect_Write
 * 
 * It writes one sector from Scratch buffer
 * 
 * @param sector sector's number
 */
void Sect_Write(unsigned long sector){
//  int xdata error;
#if CF_USE
#ifdef __F340_VER__
  if(!Is_Compact_Flash) {
#endif
#endif
    MMC_FLASH_Block_Write(sector+HIDDEN_SECTORS,Scratch); // wozb - 27-09-2005 instead of this write data from uart
#if CF_USE
#ifdef __F340_VER__
  } else {
    /* error = */ 
    	_CF_Sector_Address = sector;
	_CF_Sector_Buffer = Scratch;
	Write_Sector();
  }
#endif
#endif
	// After reformatting by the PC, we must re-init the sector server.
  if(sector==0) {
    Sect_Validate();
  }
}

/**
 * Sect_Write_Multi_Fat
 * 
 * Automatically handle multiple FAT copies
 * 
 * @param sector sector's number
 */
void Sect_Write_Multi_Fat(unsigned long sector){
  if(sector<Sect_Fat1() || sector>=Sect_Root_Dir()) {
		// This is a 'normal' block, not in the FAT:
    Sect_Write(sector);
  } else {
		// Writing to one of the FAT's will automagically write the same block to the 
		// other FAT copies.
    while(sector>=Sect_Fat2()) // Decrement 'sector' to refer to 1st FAT copy.
		  sector-=bootrecord.sectors_per_fat;
    while(sector<Sect_Root_Dir()){ // Write same data to each FAT copy.
      Sect_Write(sector);
      sector+=bootrecord.sectors_per_fat;
    }
  }
}

/**
 * Sect_Root_Dir
 * 
 * Returns number of sector for root directory
 * 
 * @return number of sectors
 */
unsigned Sect_Root_Dir(){
  return bootrecord.hidden_sectors + 
  bootrecord.reserved_sectors + /* Boot record followed by FATs */ 
  bootrecord.fat_copies*bootrecord.sectors_per_fat;
}

/**
 * Sect_Root_Dir_Last
 * 
 * Returns number of last sector for root directory
 * 
 * @return number of last sector
 */
unsigned Sect_Root_Dir_Last(){
  return Sect_Root_Dir() - 1
      + (bootrecord.root_directory_entries*DIRENTRY_SIZE)/PHYSICAL_BLOCK_SIZE;
}

/**
 * Sect_File_Data
 * 
 * Returns first sector of file data
 * 
 * @return number of sector
 */
unsigned Sect_File_Data(){
  return Sect_Root_Dir_Last() + 1 - (bootrecord.sectors_per_cluster*2); // First file data block is called "number 2".
}

/**
 * Sect_Fat1
 * 
 * Returns first sector of 1-st FAT
 * NOTE: Changed this function in a #define, to safe code memory
 * 
 * @return number of sector
 */
unsigned Sect_Fat1(){
  return bootrecord.hidden_sectors + bootrecord.reserved_sectors;
}

/**
 * Sect_Fat2
 * Returns first sector of 2-st FAT
 * NOTE: Changed this function in a #define, to safe code memory
 * 
 * @return number of sector
 */
unsigned Sect_Fat2(){
  return bootrecord.hidden_sectors + bootrecord.reserved_sectors+bootrecord.sectors_per_fat;
}

/**
 * Clear_FATs
 * 
 * Function clears all FAT
 */
static void Clear_FATs(){
  unsigned sector;
  Sect_Read(0);
  Sect_Validate();
  memset(Scratch,0,PHYSICAL_BLOCK_SIZE);
  for(sector = Sect_Fat1()+1;sector < Sect_Root_Dir();sector++)
  {
    Sect_Write(sector);
  }
  Scratch[0] = 0xf8;
  Scratch[1] = 0xff;
  Scratch[2] = 0xff;
  Scratch[3] = 0xff;
  Sect_Write(Sect_Fat1());
}

/**
 * Clear_Dir_Entries
 * 
 * Function clears root directory entries
 */
static void Clear_Dir_Entries(){
  unsigned sector;
  memset(Scratch,0,PHYSICAL_BLOCK_SIZE);
  for(sector=Sect_Root_Dir();sector <= Sect_Root_Dir_Last() ;sector++)
  {
    Sect_Write(sector);
  }
}

/**
 * Format_Disk
 * 
 * Function is used to formatting disk to FAT16
 */
void Format_Disk(){
  Clear_FATs();
  Clear_Dir_Entries();
  return;
}

