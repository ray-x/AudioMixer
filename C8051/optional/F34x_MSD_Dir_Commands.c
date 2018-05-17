//-----------------------------------------------------------------------------
// F34x_MSD_Dir_Commands.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file contains the functions used by the USB MSD RD example application
// to operate on directories
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000033
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

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "F34x_MSD_Dir_Commands.h"
#include "F34x_MSD_File_System.h"
#include "F34x_MSD_Sect_Serv.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern xdata unsigned char Path_Name[200];
extern xdata unsigned long Current_Dir_Block;

code char* Dir_Creattion_Errors[] = {
	0,
	"File already exists",
	"No place for directory",
	"Wrong directory name length"
};

//-----------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Change_Directory
//-----------------------------------------------------------------------------
//
// Return Value : 0 if error occurs 1 otherwise
// Parameters   : dir_name - name of directory to change
//
// This function changes cutrrent directory into dir_name
//-----------------------------------------------------------------------------

int Change_Directory(char* dir_name)
{
	char* prev,*next;
//	xdata char prev_dir_name[200];
  	static xdata char prev_dir_name[200];
	unsigned long prev_dir_block = Current_Dir_Block;
	int max_len = strlen(dir_name);

	strcpy(prev_dir_name,Path_Name);

	prev = dir_name;
	while((next = strstr(prev,"\\")) != NULL)
	{	
		*next = '\0';
		if(next == prev)
		{
			FileSys_Init();
		}
		else
		{
			if(!chngdir(prev))
			{
				Current_Dir_Block = prev_dir_block;
				strcpy(Path_Name,prev_dir_name);
				printf("Directory does not exist"ENDLINE);
				return 0;
			}
		}
		prev = next + 1;	
		// check if it's not root directory
		if(prev >= (dir_name + max_len)) return 1;
	}
	if(!chngdir(prev))
	{
		Current_Dir_Block = prev_dir_block;
		strcpy(Path_Name,prev_dir_name);
		printf("Directory does not exist"ENDLINE);
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------------
// Make_Directory
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : dir_name - name of directory to make
//
// This function makes new directory
//-----------------------------------------------------------------------------
void Make_Directory(char* dir_name)
{
	char* prev,*next;
  	char prev_dir_name[200];
	int max_len = strlen(dir_name);
	int is_deeper = 0;
//	xdata BYTE ret;
	/*xdata*/ BYTE ret = 0;


	strcpy(prev_dir_name,Path_Name);
	prev = dir_name;

	while((next = strstr(prev,"\\")) != NULL)
	{	
		prev = next + 1;	
		// check if it's not root directory
		if(prev >= (dir_name + max_len)) 
		{
			printf("Can not remove root directory"ENDLINE);
			return;
		}
		is_deeper = 1;
	}
	if(is_deeper)
	{
		prev--;
		*prev=0;
		if(Change_Directory(dir_name))
		{
			prev++;
			ret = mkdir(prev);
			Change_Directory(prev_dir_name);
		}
		else
		{
			printf("Unable to create directory"ENDLINE); 	
		}
	}
	else
	{
		ret = mkdir(dir_name);
	}
	if(ret != 0)
	{
		printf("Unable to create directory: %s"ENDLINE,Dir_Creattion_Errors[ret]);
	}
}

//-----------------------------------------------------------------------------
// Remove_Direcory
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : dir_name - name of directory to remove
//
// This function removes directory "dir_name"
//-----------------------------------------------------------------------------
void Remove_Direcory(char* dir_name)
{
	char* prev,*next;
  	/*xdata*/ char prev_dir_name[200];
	int max_len = strlen(dir_name);
	int is_deeper = 0;

	if(strstr(Path_Name,dir_name) == Path_Name)
	{
		printf("Unable to remove directory"ENDLINE);
		return;
	}
	strcpy(prev_dir_name,Path_Name);
	prev = dir_name;

	while((next = strstr(prev,"\\")) != NULL)
	{	
		prev = next + 1;	
		// check if it's not root directory
		if(prev >= (dir_name + max_len)) 
		{
			printf("Can not remove root directory"ENDLINE);
			return;
		}
		is_deeper = 1;
	}

	if(is_deeper)
	{
		prev--;
		*prev=0;
		if(Change_Directory(dir_name))
		{
			prev++;
			if(!rmdir(prev))
			{
				printf("Unable to remove directory"ENDLINE);
			}
			Change_Directory(prev_dir_name);
		}
		return; 
	}
	
	if(!rmdir(dir_name))
	{
		printf("Unable to remove directory"ENDLINE);
	}
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------