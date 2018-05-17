/*--------------------------------------------------------------------------/
/  Tiny-FatFs - FAT file system module include file  R0.04    (C)ChaN, 2007
/---------------------------------------------------------------------------/
/ FatFs module is an experimenal project to implement FAT file system to
/ cheap microcontrollers. This is a free software and is opened for education,
/ research and development under license policy of following trems.
/
/  Copyright (C) 2007, ChaN, all right reserved.
/
/ * The FatFs module is a free software and there is no warranty.
/ * You can use, modify and/or redistribute it for personal, non-profit or
/   profit use without any restriction under your responsibility.
/ * Redistributions of source code must retain the above copyright notice.
/
/---------------------------------------------------------------------------*/

#ifndef _FATFS

#define _MCU_ENDIAN 1
/* The _MCU_ENDIAN defines which access method is used to the FAT structure.
/  1: Enable word access.
/  2: Disable word access and use byte-by-byte access instead.
/  When the architectural byte order of the MCU is big-endian and/or address
/  miss-aligned access is prohibited, the _MCU_ENDIAN must be set to 2.
/  If it is not the case, it can be set to 1 for good code efficiency. */

#define _FS_READONLY 0
/* Setting _FS_READONLY to 1 defines read only configuration. This removes
/  writing functions, f_write, f_sync, f_unlink, f_mkdir, f_chmod, f_rename
/  and useless f_getfree. */

#define _FS_MINIMIZE 0
/* The _FS_MINIMIZE option defines minimization level to remove some functions.
/  0: Full function.
/  1: f_stat, f_getfree, f_unlink, f_mkdir, f_chmod and f_rename are removed.
/  2: f_opendir and f_readdir are removed in addition to level 1. */

#define _FAT32 0
/* When enable FAT32 support, set _FAT32 to 1. */

#define  _USE_SJIS
/* When _USE_SJIS is defined, Shift-JIS code transparency is enabled, otherwise
/  only US-ASCII(7bit) code can be accepted as file/directory name. */


#include "type.h"

/* Result type for fatfs application interface */
typedef unsigned char FRESULT;
#if _FAT32 == 0
typedef u16 CLUST;
#else
typedef u32 CLUST;
#endif


/* File system object structure */
typedef struct _FATFS {
  u8   fs_type;    /* FAT type */
  u8   sects_clust;  /* Sectors per cluster */
  u8   n_fats;      /* Number of FAT copies */
  u8   winflag;    /* win[] dirty flag (1:must be written back) */
  u16   id;        /* File system mount ID */
  u16   n_rootdir;    /* Number of root directory entries */
  u32  winsect;    /* Current sector appearing in the win[] */
  u32  fatbase;    /* FAT start sector */
  u32  dirbase;    /* Root directory start sector */
  u32  database;    /* Data start sector */
  CLUST  sects_fat;    /* Sectors per fat */
  CLUST  max_clust;    /* Maximum cluster# + 1 */
  CLUST  last_clust;    /* Last allocated cluster */
  u8   win[512];    /* Disk access window for Directory/FAT/File */
} FATFS;


/* Directory object structure */
typedef struct _DIR {
  FATFS* fs;      /* Pointer to the owner file system object */
  CLUST  sclust;    /* Start cluster */
  CLUST  clust;    /* Current cluster */
  u32  sect;    /* Current sector */
  u16   index;    /* Current index */
  u16   id;      /* Sum of owner file system mount ID */
} DIR;


/* File object structure */
typedef struct _FIL {
  FATFS* fs;        /* Pointer to owner file system */
  u32  fptr;      /* File R/W pointer */
  u32  fsize;      /* File size */
  CLUST  org_clust;    /* File start cluster */
  CLUST  curr_clust;    /* Current cluster */
  u32  curr_sect;    /* Current sector */
#if _FS_READONLY == 0
  u32  dir_sect;    /* Sector containing the directory entry */
  u8*  dir_ptr;    /* Ponter to the directory entry in the window */
#endif
  u16  id;        /* Sum of owner file system mount ID */
  u8  flag;      /* File status flags */
  u8  sect_clust;    /* Left sectors in cluster */
} FIL;


/* File status structure */
typedef struct _FILINFO {
  u32 fsize;      /* Size */
  u16  fdate;        /* Date */
  u16  ftime;        /* Time */
  u8  fattrib;      /* Attribute */
  char fname[8+1+3+1];  /* Name (8.3 format) */
} FILINFO;



/*-----------------------------------------------------*/
/* FatFs module application interface                  */

FRESULT f_mount (u8, FATFS *);            /* Mount/Unmount a logical drive */
FRESULT f_open (FIL*, const char*, u8);      /* Open or create a file */
FRESULT f_read (FIL*, void*, u16, u16*);      /* Read data from a file */
FRESULT f_write (FIL*, const void*, u16, u16*);  /* Write data to a file */
FRESULT f_lseek (FIL*, u32);            /* Move file pointer of a file object */
FRESULT f_close (FIL*);                /* Close an open file object */
FRESULT f_opendir (DIR*, const char*);        /* Open an existing directory */
FRESULT f_readdir (DIR*, FILINFO*);          /* Read a directory item */
FRESULT f_stat (const char*, FILINFO*);        /* Get file status */
FRESULT f_getfree (const char*, u32*, FATFS**);  /* Get number of free clusters on the drive */
FRESULT f_sync (FIL*);                /* Flush cached data of a writing file */
FRESULT f_unlink (const char*);            /* Delete an existing file or directory */
FRESULT f_mkdir (const char*);            /* Create a new directory */
FRESULT f_chmod (const char*, u8, u8);      /* Change file/dir attriburte */
FRESULT f_rename (const char*, const char*);    /* Rename/Move a file or directory */


/* User defined function to give a current time to fatfs module */

/* 31-25: Year(0-127 +1980), 24-21: Month(1-12), 20-16: Day(1-31) */
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
u32 get_fattime (void);

/* File function return code (FRESULT) */

#define FR_OK              0
#define FR_NOT_READY       1
#define FR_NO_FILE         2
#define FR_NO_PATH         3
#define FR_INVALID_NAME    4
#define FR_INVALID_DRIVE   5
#define FR_DENIED          6
#define FR_DISK_FULL       7
#define FR_RW_ERROR        8
#define FR_WRITE_PROTECTED 9
#define FR_NOT_ENABLED     10
#define FR_NO_FILESYSTEM   11
#define FR_INVALID_OBJECT  12


/* File access control and file status flags (FIL.flag) */

#define FA_READ        0x01
#define FA_OPEN_EXISTING  0x00
#if _FS_READONLY == 0
#define FA_WRITE          0x02
#define FA_CREATE_ALWAYS  0x08
#define FA_OPEN_ALWAYS    0x10
#define FA__WRITTEN       0x20
#endif
#define FA__ERROR         0x80


/* FAT type signature (FATFS.fs_type) */

#define FS_FAT12 1
#define FS_FAT16 2
#define FS_FAT32 3


/* File attribute bits for directory entry */

#define AM_RDO 0x01  /* Read only */
#define AM_HID 0x02  /* Hidden */
#define AM_SYS 0x04  /* System */
#define AM_VOL 0x08  /* Volume label */
#define AM_LFN 0x0F  /* LFN entry */
#define AM_DIR 0x10  /* Directory */
#define AM_ARC 0x20  /* Archive */



/* Multi-byte word access macros  */

#if _MCU_ENDIAN == 1  /* Use word access */
#define LD_WORD(ptr)      (u16)(*(u16*)(ptr))
#define LD_DWORD(ptr)     (u32)(*(u32*)(ptr))
#define ST_WORD(ptr,val)  *(u16*)(ptr)=(u16)(val)
#define ST_DWORD(ptr,val) *(u32*)(ptr)=(u32)(val)
#else
#if _MCU_ENDIAN == 2  /* Use byte-by-byte access */
#define LD_WORD(ptr)      (u16)(((u16)*(u8*)((ptr)+1)<<8)|(u16)*(u8*)(ptr))
#define LD_DWORD(ptr)     (u32)(((u32)*(u8*)((ptr)+3)<<24)|((u32)*(u8*)((ptr)+2)<<16)|((u16)*(u8*)((ptr)+1)<<8)|*(u8*)(ptr))
#define ST_WORD(ptr,val)  *(u8*)(ptr)=(u8)(val); *(u8*)((ptr)+1)=(u8)((u16)(val)>>8)
#define ST_DWORD(ptr,val) *(u8*)(ptr)=(u8)(val); *(u8*)((ptr)+1)=(u8)((u16)(val)>>8); *(u8*)((ptr)+2)=(u8)((u32)(val)>>16); *(u8*)((ptr)+3)=(u8)((u32)(val)>>24)
#else
#error "Don't forget to set _MCU_ENDIAN properly!"
#endif
#endif


#define _FATFS
#endif

