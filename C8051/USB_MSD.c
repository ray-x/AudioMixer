//-----------------------------------------------------------------------------
// F34x_MSD_MSD.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This module contains the crank function. This function is used to checking
// the request and preparing the response action for its. The fact that it's 
// crank function means that in one cycle this function can make only one step.
// That means it can't check and response in this same cycle. It works more 
// like state machine. One step can only repose for actual state and change 
// this state to another. The response for other state occurs in next step.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000046
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>

#include "main.h"
#include "USB_MSD.h"
#include "scsi.h"
#include "util.h"
#include "f34x_USB_ISR.h"
#include "diskio.h"

#define DIRECTION_IN	  0x80
#define DIRECTION_OUT	0x00

#define CBW_SIGNATURE 0x43425355
#define CSW_SIGNATURE 0x53425355

typedef enum {
  MSD_READY,
  MSD_COMMAND_TRANSPORT,
  MSD_DATA,
  MSD_ZERO_PADDING,
  MSD_DO_STALL,
  MSD_STALLING,
  MSD_MAKE_CSW,
  MSD_STATUS_TRANSPORT,
  MSD_DO_RESET
} msd_state_t;

static msd_state_t xdata msd_State;

msd_cbw_t xdata msd_cbw;
msd_csw_t xdata msd_csw;

static volatile bit discard_state_transition;

static volatile bit ep_in_stall = FALSE;
static volatile bit ep_out_stall = FALSE;

void usb_MSD_init(){
  if(ep_in_stall){
    usb_clear_halt(DIR_IN, MSD_EP_IN);
    usb_status_unlock(DIR_IN, MSD_EP_IN);
  }
  if(ep_out_stall){
    usb_clear_halt(DIR_OUT, MSD_EP_OUT);
    usb_status_unlock(DIR_OUT, MSD_EP_OUT);
  }
  msd_State = MSD_DO_RESET;
  discard_state_transition = TRUE;
}

static void usb_MSD_reset(){
  // Parse this class-specific request
  if((usb_setup_buf.bmRequestType == 0x21) 
      && (usb_setup_buf.wLength.i == 0x00)) {
    
    usb_status_unlock(DIR_IN, MSD_EP_IN);
    usb_status_unlock(DIR_OUT, MSD_EP_OUT);
    
    msd_State = MSD_DO_RESET;
    discard_state_transition = TRUE;
    
    usb_request_completed = TRUE;
  }
}

static void usb_MSD_Get_MaxLUN(){ 
  // Parse this class-specific request
  if((usb_setup_buf.bmRequestType == 0xA1) 
      && (usb_setup_buf.wLength.i == 0x01)
      && (usb_setup_buf.wValue.i == 0x00)) {
    // Return max lun to host:
    static code unsigned char maxlun[] = {0x00}; // Only 1 LUN supported
    regist_data(maxlun, 1);
    
    // put endpoint in transmit mode
    usb_ep0_status = EP_TX;
    usb_request_completed = TRUE;
  }
}

void usb_MSD_req(){
  switch(usb_setup_buf.bRequest){
    case MSD_GET_MAX_LUN:
      usb_MSD_Get_MaxLUN();
      break;
    case MSD_RESET:
      usb_MSD_reset();
      break;
  }
}

u8 xdata msd_action;

/**
 * This is a polling function. 
 * It checks if something is received and calls the responding functions (USB).
 * 
 */
void msd_polling(){
  
  msd_state_t current;
  
  if(!usb_enable){return;}
  if(!usb_previous_enable){
    msd_State = MSD_DO_RESET;
    return;
  }
  
  CRITICAL_USB0(
    current = msd_State;
    discard_state_transition = FALSE;
  );
  
  switch(current) {
    case MSD_READY: {
      unsigned int ready_bytes = usb_count_ep_out(MSD_EP_OUT); 
      if(!ready_bytes){return;}
      
      // Look for a "valid" and "meaningful" CBW, 
      // as defined in the spec: Check size
      if(ready_bytes == sizeof(msd_cbw_t)) {
        
        usb_read((BYTE*)&msd_cbw, sizeof(msd_cbw_t), MSD_EP_OUT);
        
        // Check signature, reserved bits & LUN
        if((CBW_SIGNATURE == le_u32(msd_cbw.dCBWSignature.i)) 
            && (msd_cbw.bCBWLUN <= 0x0F)
            && (msd_cbw.bCBWCBLength && (msd_cbw.bCBWCBLength <= 0x10))
            && (!(msd_cbw.bmCBWFlags & (~0x80)))){
          
          scsi_Residue = le_u32(msd_cbw.dCBWDataTransferLength.i);
          scsi_Status = SCSI_FAILED;
          
          scsi_lun = msd_cbw.bCBWLUN;
          disk_ioctl(scsi_lun, GET_SECTOR_SIZE, (void *)&scsi_block_size);
          
          if(scsi_Residue){
            msd_action 
                = (msd_cbw.bmCBWFlags & 0x80) ? MSD_HOST_RX : MSD_HOST_TX;
          }else{
            msd_action = MSD_HOST_NO_DATA;
          }
          
          scsi_setup();
          
          current = MSD_DATA;
          ep_in_stall = FALSE;
          ep_out_stall = FALSE;
          
          switch(msd_action){
            // case(2),(3)
            case (MSD_HOST_NO_DATA | MSD_DEVICE_RX):
            case (MSD_HOST_NO_DATA | MSD_DEVICE_TX):
              current = MSD_MAKE_CSW;
              scsi_Status = SCSI_PHASE_ERROR;
              break;
            
            // case(8)
            case (MSD_HOST_TX | MSD_DEVICE_TX):
              scsi_Status = SCSI_PHASE_ERROR;
              current = MSD_DO_STALL;
              ep_in_stall = TRUE;
              break;
            
            // case(10)
            case (MSD_HOST_RX | MSD_DEVICE_RX):
              scsi_Status = SCSI_PHASE_ERROR;
              current = MSD_DO_STALL;
              ep_out_stall = TRUE;
              break;
          }
          
          break;
        }
      }else{
        usb_flush(MSD_EP_OUT);
      }
      
      ep_in_stall = TRUE;
      ep_out_stall = TRUE;
      usb_status_lock(DIR_IN, MSD_EP_IN);
      usb_status_lock(DIR_OUT, MSD_EP_OUT);
      current = MSD_DO_STALL;
      break;
    }

    case MSD_DATA:
      scsi_ex();
      if(scsi_Status != SCSI_PENDING){
        if(scsi_Residue > 0){
          
          // Case (4) and Case (5)?
          if(MSD_HOST_SIDE(msd_action) == MSD_HOST_RX){
            msd_csw.dCSWDataResidue.i = le_u32(scsi_Residue);
            current = MSD_ZERO_PADDING;
            break;
          }
          // Case (9) and Case (11)?
          else if(MSD_HOST_SIDE(msd_action) == MSD_HOST_TX){
            ep_out_stall = TRUE;
            current = MSD_DO_STALL;
            break;
          }
        }
        current = MSD_MAKE_CSW;
      }
      break;
    
    case MSD_ZERO_PADDING:
      {
        u32 fill_length = le_u32(msd_csw.dCSWDataResidue.i);
        fill_length -= usb_fill(0, fill_length, MSD_EP_IN);
        msd_csw.dCSWDataResidue.i = le_u32(fill_length);
        if(!fill_length){current = MSD_MAKE_CSW;}
        break;
      }
    
    case MSD_DO_STALL:
      if(ep_in_stall){usb_stall(DIR_IN, MSD_EP_IN, NULL);}
      if(ep_out_stall){usb_stall(DIR_OUT, MSD_EP_OUT, NULL);}
      current = MSD_STALLING;
    
    case MSD_STALLING:
      CRITICAL_USB0(
        ep_in_stall = (usb_ep_status(DIR_IN, MSD_EP_IN) != EP_IDLE);
        ep_out_stall = (usb_ep_status(DIR_OUT, MSD_EP_OUT) != EP_IDLE);
      );
      if(ep_in_stall || ep_out_stall){break;}
    
    case MSD_MAKE_CSW:
      // Reply with a CSW:
      msd_csw.dCSWSignature.i = le_u32(CSW_SIGNATURE);
      msd_csw.dCSWTag.i = msd_cbw.dCBWTag.i;
      msd_csw.bCSWStatus = scsi_Status;
      msd_csw.dCSWDataResidue.i = le_u32(scsi_Residue);
      current = MSD_STATUS_TRANSPORT;

		case MSD_STATUS_TRANSPORT:
			if(usb_write((BYTE*)&msd_csw, sizeof(msd_csw_t), MSD_EP_IN)){
        current = MSD_READY;
      }
      break;

    case MSD_DO_RESET:
    default:
      current = MSD_READY;
      break;
  }
  
  CRITICAL_USB0(
    if(!discard_state_transition){
      msd_State = current;
    }
  );
}
