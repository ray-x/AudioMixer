//-----------------------------------------------------------------------------
// F34x_MSD_Cmd.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Module contains a functions for communication via UART. These functions are 
// mainly used to initializing, communicating, executing commands used by 
// UART interface.
//
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000030
// Target:         C8051F34x
// Tool chain:     Keil
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_MSD
//
// Release 1.1
//    -All changes by PKC
//    -09 JUN 2006
//    -Replaced SFR definitions file "c8051f320.h" with "c8051f340.h"
//
// Release 1.0
//    -Initial Release
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "F34x_MSD_Definitions.h"
#include "c8051f340.h"
#include "F34x_MSD_Util.h"
#include "F34x_MSD_UART0.h"
#include "F34x_MSD_Cmd.h"
#include "F34x_MSD_Sect_Serv.h"
#include "F34x_MSD_File_System.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "F34x_MSD_Log.h"
#include "F34x_MSD_Temp_Sensor.h"
#include "F34x_MSD_Dir_Commands.h"
#include "F34x_MSD_Format_Disk.h"

#define MAX_LEN 32
static char xdata Cmd[MAX_LEN];
static char* xdata Token;
static BYTE xdata Cmd_Ptr;

unsigned char format_flag = 0;
//bootrecord_large* xdata bootrecord=Scratch;	//
long xdata n_of_sect = 0;

#ifdef __F340_VER__
extern void Get_Status_MMC();
#ifdef __SDCC__
extern bit Is_Initialized;
#else
extern bdata bit Is_Initialized;
#endif
#endif

//----------------------------------------------------------------------------
// Cmd_Init
//----------------------------------------------------------------------------
//
// Initialization of communication via UART interface. This function ends on 
// prompt sign. 
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Cmd_Init() 
{
  putchar(CLS);
  Cmd_Ptr=0;
  Cmd[0]=Cmd[1]='\0';
  Token=0;
  Sect_Print();
  printf("MMC Shell version 1.0" ENDLINE);
  write_current_dir();
  putchar(PROMPT);
}

//----------------------------------------------------------------------------
// Cmd_Match
//----------------------------------------------------------------------------
//
// This function checks the syntax of command and converts all leters to 
// lowercase if it's uppercase.  
//
// Parameters   : str - command string
// Return Value : TRUE if command string and received string are similar 
//----------------------------------------------------------------------------

BYTE Cmd_Match(char* str) 
{
#ifdef __SDCC__
  /*xdata*/ char* /*xdata*/ s = (/*xdata*/ char*)Token;
#else
  xdata char* xdata s=Token;
#endif
  for(;*s;s++) *s=tolower(*s);
  return !strncmp(Token,str,MAX_LEN);
}

//----------------------------------------------------------------------------
// Cmd_Dir
//----------------------------------------------------------------------------
//
// Function sends list of all directories and files in actual directory.
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Cmd_Dir(void) 
{
  /*xdata*/ BYTE i;
  find_info /*xdata*/ findinfo;
  if(!findfirst(&findinfo,0)) {
    printf("."ENDLINE);	
    return;
  }
  do {
    for(i=0;i<8;i++) { 
      putchar(tolower(findinfo.direntry->sfn.name[i]));
    }
    if(findinfo.direntry->sfn.name[i]>' ') 
      putchar('.');
    for(i=8;i<11;i++) { 
      putchar(tolower(findinfo.direntry->sfn.name[i])); 
    }

    if(!(findinfo.direntry->sfn.attrib & (ATTRIB_LABEL|ATTRIB_SUBDIR))) {
      printf("\t%lu bytes",ntohl(findinfo.direntry->sfn.filesize));
    }

    if(findinfo.direntry->sfn.attrib & ATTRIB_LABEL) 
      printf("\t<LABEL>");
    if(findinfo.direntry->sfn.attrib & ATTRIB_SUBDIR) 
      printf("\t<DIR>");
	printf(ENDLINE);

  } while(findnext(&findinfo));
}

//----------------------------------------------------------------------------
// Cmd_Type
//----------------------------------------------------------------------------
//
// This function sends via UART the content of file which is defiened with 
// "type" command.
//
// Parameters   : filename - pointer to file name
// Return Value :
//----------------------------------------------------------------------------

void Cmd_Type(char* filename) 
{
  FILE /*xdata*/ f;
  unsigned /*xdata*/ bytesread,i;

  if(!fopen(&f,filename,"r")) { 
    printf("File not found"ENDLINE);
    return; 
  }  

  while(bytesread=fread(&f,Scratch,Sect_Block_Size())) {
    if(key_available()) { 
      break; 
    }
    for(i=0;i<bytesread;i++) {
      putchar(Scratch[i]);
    }
  }

  fclose(&f);


  printf(ENDLINE);
}

//----------------------------------------------------------------------------
// Cmd_Help
//----------------------------------------------------------------------------
//
// Function sends via UART the list of known commands. This list contains all
// commands allowed for normal user. List is printing when system doesn't 
// recognize the last sent command.
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Cmd_Help(void) 
{
  printf("calibrate VALUE"ENDLINE"cd"ENDLINE"chkdsk"ENDLINE"cls"ENDLINE"del FILE"ENDLINE"dir"
  		ENDLINE"format"ENDLINE"log FILE"ENDLINE"md"ENDLINE"rd"
  		ENDLINE"s NNNNN"ENDLINE"templog FILE"ENDLINE"type FILE"ENDLINE);
}

//----------------------------------------------------------------------------
// Cmd_Exec
//----------------------------------------------------------------------------
//
// This function is used to executing commands. It recognizes the command and 
// prepares reaction, answer. 
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Cmd_Exec() 
{
  int /*xdata*/ i;
  BYTE /*xdata*/ j;
  char /*xdata*/ ch;
  DWORD /*xdata*/ dw;

  Token = Str_Token(Cmd);

  if(!Token) 
    return;

  printf(ENDLINE);

  
  if(n_of_sect == 0)
//	n_of_sect = ntohl(bootrecord->total_sectors);
	n_of_sect = Sect_Sectors();

  if(format_flag == 1)
  {
  	if(Cmd_Match("y"))
	{
		format_flag = 0;
		Format_Disk();
		return;
	}
	if(Cmd_Match("n"))
		format_flag = 0;
	else
		printf("Are you sure (y/n)?"ENDLINE);	
	return;
  }
  if(Cmd_Match("cls")) {
    putchar(CLS);
    return;
  } 

  if(Cmd_Match("del")) {
    Token = Str_Token(0);
    if(Token) {
      if(!fdelete(Token))
        printf("File not found"ENDLINE);
    } else {
      Cmd_Help();
    }
    return;
  }

  if(Cmd_Match("dir")) {
    Cmd_Dir();
    return;
  }

  if(Cmd_Match("s")) {
    Token=Str_Token(0);

    if((*Token > 0x29) && (*Token < 0x40))
    {
		dw=atol(Token);
		printf("Sector 0x%08lX:"ENDLINE,dw);
	    Sect_Read(dw);
	    for(i=0;i<Sect_Block_Size()/16;i++) {
	      printf("%04X: ",i*16);
	      for(j=0;j<16;j++) {
	        ch=Scratch[i*16+j];
	        printf("%02X ",0x00FF&(int)ch); 
	      }
	      for(j=0;j<16;j++) { 
	        ch=Scratch[i*16+j];
	        putchar((ch>' ' && ch<0x7F)?ch:'.'); 
	      }
		  printf(ENDLINE);
	    }
		printf(ENDLINE);
	}
	else
		printf("Sectors' range 0x00000 - 0x%lX\r\n",n_of_sect); 	

	return;
  }

  if(Cmd_Match("log")) {
    Token=Str_Token(0);
    Log(Token);
    return;
  }

  if(Cmd_Match("type")) {
    Token = Str_Token(0);
    if(Token) {
      Cmd_Type(Token);
    } else {
      Cmd_Help();
    }
    return;
  }

  if(Cmd_Match("chkdsk")) {
    Sect_Print();
    return;
  }

#ifdef __F340_VER__
  if(Cmd_Match("calibrate")) {
    Token=Str_Token(0);
    if((*Token > 0x29) && (*Token < 0x40))
	{
		dw=atol(Token);
		Calibrate((int)dw);
		printf("Calibrated to %d deg C\r\n", (int) dw);
	}
	else
		printf("Missing parameter\r\n");

    return;
  }
  if(Cmd_Match("cd")) {
      Token=Str_Token(0);
	  Change_Directory(Token);
	  return;
  }
  if(Cmd_Match("md")) {
    Token=Str_Token(0);
  	Make_Directory(Token);
	return;
  }
  if(Cmd_Match("templog")) {
    Token=Str_Token(0);
    Temp_Log(Token);
    return;
  }
  if(Cmd_Match("format")) {
  	printf("Are you sure (y/n)?"ENDLINE);
	format_flag = 1;
	//Format_Disk();
	return;
  }
  if(Cmd_Match("rd"))
  {
	Token=Str_Token(0);
  	Remove_Direcory(Token);
	return;
  }
  if(Cmd_Match("mmc"))
  {
    Token=Str_Token(0);
    Get_Status_MMC();
    return;
  }
  if(Cmd_Match("time_out"))
  {
    Is_Initialized = 1;
    return;
  }
  if(Cmd_Match("loop"))
  {
    for(i=0;i<1000;i++)
      Sect_Read(i);
    return;
  }
  if(Cmd_Match("write_test"))
  {
    for(i=0;i<512;i++)
      Scratch[i] = (BYTE)0xBB;
    for(i=0;i<100;i++) {
      Sect_Write(1000+i);
    }
    for(i=0;i<100;i++) {
      Sect_Read(1000+i);
      for(dw=0;dw<512;dw++) {
        if(Scratch[dw] != (BYTE)0xBB) {
          printf("error sector %d\r\n",1000 + i);
          return;
        }
      }
    }
    return;
  }
 /* if(Cmd_Match("y"))
  {
  	if(format_flag == 1)
	{
		Format_Disk();		
		format_flag = 0;
		return;
	}
  }
  if(Cmd_Match("n"))
  {
  	if(format_flag == 1)
	{
		format_flag = 0;
		return;
	}
  }*/

#endif
/*
// If MMC is so fubar that Windows cannot even recognize it for formatting,
// then activate & execute this 'fdisk' command.
//
	if(Cmd_Match("fdisk")) {
		Sect_Read(0);
		Scratch[ 0]=0xEB;Scratch[ 1]=0x3C;Scratch[ 2]=0x90;Scratch[ 3]='M';
		Scratch[ 4]= 'S';Scratch[ 5]= 'D';Scratch[ 6]= 'O';Scratch[ 7]='S';
		Scratch[ 8]= '5';Scratch[ 9]= '.';Scratch[10]= '0';Scratch[11]=0x00;
		Scratch[12]=0x02;Scratch[13]=0x01;Scratch[14]=0x01;Scratch[15]=0x00;

		Scratch[16]=0x02;Scratch[17]=0x00;Scratch[18]=0x02;Scratch[19]=0x60;
		Scratch[20]=0xF4;Scratch[21]=0xF8;Scratch[22]=0xF3;Scratch[23]=0x00;
		Scratch[24]=0x3F;Scratch[25]=0x00;Scratch[26]=0xFF;Scratch[27]=0x00;
		Scratch[28]=0x20;Scratch[29]=0x00;Scratch[30]=0x00;Scratch[31]=0x00;

		Scratch[32]=0x00;Scratch[33]=0x00;Scratch[34]=0x00;Scratch[35]=0x00;
		Scratch[36]=0x00;Scratch[37]=0x00;Scratch[38]=0x29;Scratch[39]=0x42;
		Scratch[40]=0xE9;Scratch[41]=0x1D;Scratch[42]=0xC8;

                                          Scratch[54]=0x46;Scratch[55]=0x41;
		Scratch[56]=0x54;Scratch[57]=0x31;Scratch[58]=0x36;Scratch[59]=0x20;
		Scratch[60]=0x20;Scratch[61]=0x20;

		Scratch[510]=0x55;Scratch[511]=0xAA;

		Sect_Write(0);
		return;
	}
*/

  if(Cmd_Ptr)	{ 
    printf("Bad command or file name."ENDLINE);
    Cmd_Help();
  }
}

//----------------------------------------------------------------------------
// Cmd_Step
//----------------------------------------------------------------------------
//
// This is a crank function. In every cycle it checks the interrupt receive
// flag and starts first action on event. 
//
// Parameters   :
// Return Value :
//----------------------------------------------------------------------------

void Cmd_Step(){
  char ch;
  int buf_size = uart0_rx_size();
  
  while(buf_size-- > 0){
    ch = getchar();
    putchar(ch);
    
    switch(ch){
      case BACKSPACE:
        if(Cmd_Ptr) { 
          Cmd_Ptr--;
          putchar(SPACE);
          putchar(BACKSPACE);
        }
        break;
      case LF:
        Cmd[Cmd_Ptr]='\0';
        Cmd_Exec();
        memset(Cmd,'\0',MAX_LEN);
        write_current_dir();
        putchar(PROMPT);
        Cmd_Ptr=0;
        break;
      case CR:
        break;
      default:
        if(Cmd_Ptr<MAX_LEN-1){Cmd[Cmd_Ptr++]=ch;}
    }
  }
}
