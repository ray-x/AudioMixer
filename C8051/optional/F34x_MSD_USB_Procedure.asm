;//----------------------------------------------------------------------------
;// F34x_MSD_USB_Procedure.asm
;//----------------------------------------------------------------------------
;// Copyright 2006 Silicon Laboratories, Inc.
;// http://www.silabs.com
;//
;// Program Description:
;//
;// This file contains commands for USB interface.
;//
;//
;//
;// How To Test:    See Readme.txt
;//
;//
;// FID:            34X000064
;// Target:         C8051F34x
;// Tool chain:     Keil
;// Command Line:   See Readme.txt
;// Project Name:   F34x_USB_MSD
;//
;// Release 1.1_chiaki-1.0 : 2006-07-01
;//    -First trial to port to sdcc
;//
;// Release 1.1
;//    -All changes by PKC
;//    -09 JUN 2006
;//    -Removed individual SFR definitions and included c8051f340.inc
;//
;// Release 1.0
;//    -Initial Release
;//

.area USB_PROCEDURE (rel,con,code)

.globl _Fifo_Write
.globl _Fifo_Read

;$include (c8051f340.inc)               ; Include register definition file

;USB_PROCEDURE SEGMENT CODE
;RSEG USB_PROCEDURE

;PUBLIC	_Fifo_Write
;PUBLIC	_Fifo_Read

_Fifo_Write:
;R1,R2,R3 - buffer address
;R7 - endpoint address
;R4,R5 - LENGTH OF BUFFER

		push		ACC
		mov		A, R0
		push		ACC
		mov		A, R1
		push		ACC
		mov		A, R2
		push		ACC
		mov		A, R3
		push		ACC
		mov		A, R4
		push		ACC
		mov		A, R5
		push		ACC
		mov		A, R7
		push		ACC
		push		DPL
		push		DPH
		;
		mov		DPTR, #_Fifo_RW_Ep_Address
		movx		A, @DPTR
		mov		R7, A
		mov		DPTR, #_Fifo_RW_Buf_Length
		movx		A, @DPTR
		mov		R5, A			;R5 : Low byte of the buffer length
		inc		DPTR
		movx		A, @DPTR
		mov		R4, A			;R4 : High byte of the buffer length
		mov		R1, _Fifo_RW_Buf_Ptr	;R1 : Low  byte of the buffer pointer
		mov		R2, _Fifo_RW_Buf_Ptr+1	;R2 : Mid  byte of the buffer pointer
		mov		R3, _Fifo_RW_Buf_Ptr+2	;R3 : High byte of the buffer pointer
		;					;in case of SDCC --
		;					;  R3 = 40h : DSEG, ISEG
		;					;  R3 = 80h : CODE (CONST)
		;					;  R3 = 00h : XSEG
		;
BEGIN_WAIT_ON_USB:
	
		MOV		A,_USB0ADR
		JB		ACC.7,BEGIN_WAIT_ON_USB
		MOV		_USB0ADR,R7

		MOV		A,R3
		jz		COPY_XDATA
		;
		cjne		A, #0x40, 1$
		ljmp		COPY_IDATA
		;
1$:		cjne		A, #0x80, 2$
		ljmp		COPY_CODE
		;
2$:		sjmp		.			;loop forever on fatal
		;
COPY_XDATA:
		
		MOV		DPH,R2
		MOV		DPL,R1

XDATA_WRITE_COPY_LABEL:

		MOV		A,R5
		DEC		R5
		JNZ		XDATA_WRITE_DATA	
		MOV		r5,#0xff
		MOV		A,R4			
		JZ		XDATA_FINISH_COPY 
		DEC		R4	

XDATA_WRITE_DATA:
				
		MOV		A,_USB0ADR
		JB		ACC.7,XDATA_WRITE_DATA
		
		MOVX		A,@DPTR
		MOV   		_USB0DAT,A 

		INC		DPL
		MOV		A,DPL
		JNZ		XDATA_NO_DPH
		INC		DPH

XDATA_NO_DPH:
	
		SJMP		XDATA_WRITE_COPY_LABEL

XDATA_FINISH_COPY:

		pop		DPH
		pop		DPL
		pop		ACC
		mov		R7, A
		pop		ACC
		mov		R5, A
		pop		ACC
		mov		R4, A
		pop		ACC
		mov		R3, A
		pop		ACC
		mov		R2, A
		pop		ACC
		mov		R1, A
		pop		ACC
		mov		R0, A
		pop		ACC
		;
		RET


;/////////////////////////////
;// COPY DATA FROM IDATA
;/////////////////////////////	

COPY_IDATA:
	
		MOV		A,R1
		MOV		R0,A ; R0 - POINTER TO DATA DDDRESS

IDATA_WRITE_COPY_LABEL:
	
		MOV		A,R5
		DEC		R5
		JNZ		IDATA_WRITE_DATA	
		MOV		r5,#0xff
		MOV		A,R4			
		JZ		IDATA_FINISH_COPY 
		DEC		R4	

IDATA_WRITE_DATA:
				
		MOV		A,_USB0ADR
		JB		ACC.7,IDATA_WRITE_DATA
		
		MOV		A,@R0
		MOV   		_USB0DAT,A 

		INC		R0
	
		SJMP		IDATA_WRITE_COPY_LABEL

IDATA_FINISH_COPY:

		sjmp		XDATA_FINISH_COPY
		;
;		RET

;////////////////////////////////
;///	copy from code segment
;////////////////////////////////

COPY_CODE:

		MOV		DPH,R2
		MOV		DPL,R1

CODE_WRITE_COPY_LABEL:

		MOV		A,R5
		DEC		R5
		JNZ		CODE_WRITE_DATA	
		MOV		r5,#0xff
		MOV		A,R4			
		JZ		CODE_FINISH_COPY 
		DEC		R4	

CODE_WRITE_DATA:
				
		MOV		A,_USB0ADR
		JB		ACC.7,CODE_WRITE_DATA
		
		CLR		A
		MOVC		A,@A + DPTR
		MOV   		_USB0DAT,A 

		INC		DPL
		MOV		A,DPL
		JNZ		CODE_NO_DPH
		INC		DPH

CODE_NO_DPH:
	
		SJMP		CODE_WRITE_COPY_LABEL


CODE_FINISH_COPY:
		sjmp		XDATA_FINISH_COPY
		;
;		RET

;/////////////////////////////////////////
;/////////////////////////////////////////
;/////////////////////////////////////////

;/////////////////////////////////////////
;////	FIFO READ PROCEDURE
;/////////////////////////////////////////

_Fifo_Read:
;R1,R2,R3 - buffer address
;R7 - endpoint address
;R4,R5 - LENGTH OF BUFFER

		push		ACC
		mov		A, R0
		push		ACC
		mov		A, R1
		push		ACC
		mov		A, R2
		push		ACC
		mov		A, R3
		push		ACC
		mov		A, R4
		push		ACC
		mov		A, R5
		push		ACC
		mov		A, R7
		push		ACC
		push		DPL
		push		DPH
		;
		mov		DPTR, #_Fifo_RW_Ep_Address
		movx		A, @DPTR
		mov		R7, A
		mov		DPTR, #_Fifo_RW_Buf_Length
		movx		A, @DPTR
		mov		R5, A			;R5 : Low byte of the buffer length
		inc		DPTR
		movx		A, @DPTR
		mov		R4, A			;R4 : High byte of the buffer length
		mov		R1, _Fifo_RW_Buf_Ptr	;R1 : Low  byte of the buffer pointer
		mov		R2, _Fifo_RW_Buf_Ptr+1	;R2 : Mid  byte of the buffer pointer
		mov		R3, _Fifo_RW_Buf_Ptr+2	;R3 : High byte of the buffer pointer
		;					;in case of SDCC --
		;					;  R3 = 40h : DSEG, ISEG
		;					;  R3 = 80h : CODE (CONST) - never occur
		;					;  R3 = 00h : XSEG
		;
		MOV		A,R3
		jz		READ_COPY_XDATA
		;
		cjne		A, #0x40, 1$
		ljmp		READ_COPY_IDATA
		;
1$:		sjmp		.			;loop forever on fatal
		;
READ_COPY_XDATA:
		MOV		DPH,R2
		MOV   		DPL,R1

		MOV		A,R5
		JNZ		READ_BEGIN_XDATA_WAIT_ON_USB
		MOV		A,R4
		JZ		READ_XDATA_FINISH_COPY
		DEC		R4

READ_BEGIN_XDATA_WAIT_ON_USB:
	
		MOV		A,_USB0ADR
		JB		ACC.7,READ_BEGIN_XDATA_WAIT_ON_USB
		MOV		_USB0ADR,R7
		ORL		_USB0ADR,#0xC0

READ_XDATA_COPY_LABEL:

		MOV		A,_USB0ADR
		JB		ACC.7,READ_XDATA_COPY_LABEL
		
		MOV		A,_USB0DAT
		MOVX		@DPTR,A
		
		
		DJNZ		R5,XDATA_READ_NO_DEC_HI_COUNTER
		
		MOV		A,R4		
		JZ		READ_XDATA_FINISH_COPY 	
		DEC		R4

XDATA_READ_NO_DEC_HI_COUNTER:
			
		INC		DPL
		MOV		A,DPL
		JNZ		READ_XDATA_COPY_LABEL
		INC		DPH
		SJMP		READ_XDATA_COPY_LABEL

READ_XDATA_FINISH_COPY:

		pop		DPH
		pop		DPL
		pop		ACC
		mov		R7, A
		pop		ACC
		mov		R5, A
		pop		ACC
		mov		R4, A
		pop		ACC
		mov		R3, A
		pop		ACC
		mov		R2, A
		pop		ACC
		mov		R1, A
		pop		ACC
		mov		R0, A
		pop		ACC
		;
	;	CLR 	A
	;	MOV		USB0ADR,A
		RET

READ_COPY_IDATA:

		MOV		A,R1
		MOV		R0,A ; R0 - POINTER TO DATA DDDRESS

		MOV		A,R5
		JNZ		READ_BEGIN_IDATA_WAIT_ON_USB
		MOV		A,R4
		JZ		READ_IDATA_FINISH_COPY
		DEC		R4

READ_BEGIN_IDATA_WAIT_ON_USB:
	
		MOV		A,_USB0ADR
		JB		ACC.7,READ_BEGIN_IDATA_WAIT_ON_USB
		MOV		_USB0ADR,R7
		ORL		_USB0ADR,#0xC0

READ_IDATA_COPY_LABEL:

		MOV		A,_USB0ADR
		JB		ACC.7,READ_IDATA_COPY_LABEL
		
		MOV		A,_USB0DAT
		MOVX		@R0,A
		
		
		DJNZ		R5,IDATA_READ_NO_DEC_HI_COUNTER
		
		MOV		A,R4		
		JZ		READ_IDATA_FINISH_COPY 	
		DEC		R4

IDATA_READ_NO_DEC_HI_COUNTER:
			
		INC		R0
		SJMP		READ_IDATA_COPY_LABEL

READ_IDATA_FINISH_COPY:

		sjmp		READ_XDATA_FINISH_COPY
		;
	;	CLR		A
	;	MOV		USB0ADR,A

;		RET
;END	