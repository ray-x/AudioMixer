#include <string.h>

#include "MMC.h"
#include "type.h"
#include "diskio.h"
#include "tff.h"

#define RETRY 10

DSTATUS disk_initialize (u8 drive){
  if(drive != 0){return STA_NODISK;}
  MMC_FLASH_Init();
  return RES_OK;
}

DSTATUS disk_status (u8 drive){
  return (drive == 0) ? RES_OK : STA_NODISK;
}

DRESULT disk_read (u8 drive, u8 *buf, u32 start_sector, u8 sectors){
  if(drive != 0){return STA_NODISK;}
  if(sectors == 1){
    //u8 retry = RETRY;
    //while(retry--){
      if(MMC_FLASH_Block_Read(start_sector, buf) == MMC_NORMAL_CODE){
        return RES_OK;
      }
    //}
    MMC_Get_Status();
    return FR_RW_ERROR;
  }else{
    while(sectors--){
      if(MMC_FLASH_Block_Read(start_sector, buf) != MMC_NORMAL_CODE){
        MMC_Get_Status();
        return FR_RW_ERROR;
      }
    }
    return RES_OK;
  }
}

DRESULT disk_ioctl (u8 drive, u8 ctrl, void *buff){
  if(drive != 0){return STA_NODISK;}
  switch(ctrl){
    case CTRL_SYNC :
      if(MMC_Flush() != MMC_NORMAL_CODE){return FR_RW_ERROR;}
      break;
    case GET_SECTOR_COUNT :
      *(u32 *)buff = MMC_physical_sectors;
      break;
    case GET_SECTOR_SIZE :
      *(u16 *)buff = PHYSICAL_BLOCK_SIZE;
      break;
    case GET_SIZE :
      *(u32 *)buff = MMC_physical_size;
      break;
  }
  return RES_OK;
}

#if _READONLY == 0

DRESULT disk_write (u8 drive, const u8 *buf, u32 start_sector, u8 sectors){
  if(drive != 0){return STA_NODISK;}
  if(sectors == 1){
    //u8 retry = RETRY;
    //while(retry--){
      if(MMC_FLASH_Block_Write(start_sector, buf) == MMC_NORMAL_CODE){
        return RES_OK;
      }
    //}
    MMC_Get_Status();
    return FR_RW_ERROR;
  }else{
    while(sectors--){
      if(MMC_FLASH_Block_Write(start_sector, buf) != MMC_NORMAL_CODE){
        MMC_Get_Status();
        return FR_RW_ERROR;
      }
    }
    return RES_OK;
  }
}

#endif



