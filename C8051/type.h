#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;

typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned long ULONG;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef u8 BYTE;
#endif

// WORD type definition, for KEIL Compiler
#ifndef _WORD_DEF_
#define _WORD_DEF_
typedef union {u16 i; unsigned char c[2];} WORD;
#endif

// DWORD type definition
#ifndef _DWORD_DEF_
#define _DWORD_DEF_
typedef union {u32 i; unsigned char c[4];} DWORD;
#endif

#ifdef __SDCC__
/* All words sent to and received from the host are
 * little endian, this is switched by software when
 * neccessary.  These sections of code have been marked
 * with "Compiler Specific" as above for easier modification
 */
#define LSB 0
#define MSB 1
#else
/* All words sent to and received from the host are
 * little endian, this is switched by software
 */
#define LSB 1
#define MSB 0
#endif

#endif /* __TYPE_H__ */
