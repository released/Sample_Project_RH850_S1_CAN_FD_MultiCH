#ifndef __MISC_CONFIG_H__
#define __MISC_CONFIG_H__

/*_____ I N C L U D E S ____________________________________________________*/
// #include <stdio.h>
#include <string.h>

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_8bit
{
	unsigned char bit0:1 ;
	unsigned char bit1:1 ;
	unsigned char bit2:1 ;
	unsigned char bit3:1 ;
	unsigned char bit4:1 ;
	unsigned char bit5:1 ;
	unsigned char bit6:1 ;
	unsigned char bit7:1 ;
};

struct flag_16bit
{
	unsigned char bit0:1 ;
	unsigned char bit1:1 ;
	unsigned char bit2:1 ;
	unsigned char bit3:1 ;
	unsigned char bit4:1 ;
	unsigned char bit5:1 ;
	unsigned char bit6:1 ;
	unsigned char bit7:1 ;

	unsigned char bit8:1 ;
	unsigned char bit9:1 ;
	unsigned char bit10:1 ;
	unsigned char bit11:1 ;
	unsigned char bit12:1 ;
	unsigned char bit13:1 ;
	unsigned char bit14:1 ;
	unsigned char bit15:1 ;	
};

struct flag_32bit
{
	unsigned char bit0:1 ;
	unsigned char bit1:1 ;
	unsigned char bit2:1 ;
	unsigned char bit3:1 ;
	unsigned char bit4:1 ;
	unsigned char bit5:1 ;
	unsigned char bit6:1 ;
	unsigned char bit7:1 ;

	unsigned char bit8:1 ;
	unsigned char bit9:1 ;
	unsigned char bit10:1 ;
	unsigned char bit11:1 ;
	unsigned char bit12:1 ;
	unsigned char bit13:1 ;
	unsigned char bit14:1 ;
	unsigned char bit15:1 ;	

	unsigned char bit16:1 ;
	unsigned char bit17:1 ;
	unsigned char bit18:1 ;
	unsigned char bit19:1 ;
	unsigned char bit20:1 ;
	unsigned char bit21:1 ;
	unsigned char bit22:1 ;
	unsigned char bit23:1 ;

	unsigned char bit24:1 ;
	unsigned char bit25:1 ;
	unsigned char bit26:1 ;
	unsigned char bit27:1 ;
	unsigned char bit28:1 ;
	unsigned char bit29:1 ;
	unsigned char bit30:1 ;
	unsigned char bit31:1 ;
};

/*_____ D E F I N I T I O N S ______________________________________________*/
#define _debug_log_UART_							(1)

// #define ENABLE_TICK_EVENT

#define _DEBUG_LOG_ENABLE

#ifdef _DEBUG_LOG_ENABLE
#define dbg_printf      						tiny_printf
// #define dbg_printf(format, args...) 			tiny_printf("[dbg]"format , ##args)
// #define dbg_printf(format, args...) 			tiny_printf("\033[1;36m" "[log]" format "\033[0m", ##args)
#else
#define dbg_printf(...)
#endif

/*_____ M A C R O S ________________________________________________________*/

#define GET_BITS(value, start, length) 				(((value) >> (start)) & ((1U << (length)) - 1U))

#define set_BIT7(x)        							(x|=0x80)
#define set_BIT6(x)        							(x|=0x40)
#define set_BIT5(x)        							(x|=0x20)
#define set_BIT4(x)        							(x|=0x10)
#define set_BIT3(x)        							(x|=0x08)
#define set_BIT2(x)        							(x|=0x04)
#define set_BIT1(x)        							(x|=0x02)
#define set_BIT0(x)        							(x|=0x01)
                                  
#define clr_BIT7(x)        							(x&=0x7F)
#define clr_BIT6(x)        							(x&=0xBF)
#define clr_BIT5(x)        							(x&=0xDF)
#define clr_BIT4(x)        							(x&=0xEF)
#define clr_BIT3(x)        							(x&=0xF7)
#define clr_BIT2(x)        							(x&=0xFB)
#define clr_BIT1(x)        							(x&=0xFD)
#define clr_BIT0(x)        							(x&=0xFE)


//MACRO : Swap Integers Macro
#ifndef SWAP
#define SWAP(a, b)     								{(a) ^= (b); (b) ^= (a); (a) ^= (b);}
#endif

#ifndef MAX
#define MAX(a,b) 									(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) 									(((a) < (b)) ? (a) : (b))
#endif

//16 --> 8 x 2
#ifndef HIBYTE
#define HIBYTE(v1)              					((unsigned char)((v1)>>8))                      		//v1 is unsigned short
#endif

#ifndef LOBYTE
#define LOBYTE(v1)              					((unsigned char)((v1)&0xFF))
#endif

//8 x 2 --> 16
#ifndef MAKEWORD
#define MAKEWORD(v1,v2)         					((((unsigned short)(v1))<<8)+(unsigned short)(v2))      //v1,v2 is unsigned char
#endif

//8 x 4 --> 32
#ifndef MAKELONG
#define MAKELONG(v1,v2,v3,v4)   					(unsigned long)((v1<<24)+(v2<<16)+(v3<<8)+v4)  			//v1,v2,v3,v4 is unsigned char
#endif

#ifndef SIZEOF
#define SIZEOF(a) 									(sizeof(a) / sizeof(a[0]))
#endif

#ifndef ENDOF
#define ENDOF(a) 									((a) + SIZEOF(a))
#endif

#ifndef CLEAR
#define CLEAR(x) 									(memset(&(x), 0, sizeof (x)))
#endif

#ifndef ABS
#define ABS(X)  									((X) > 0 ? (X) : -(X)) 
#endif

#ifndef UNUSED
#define UNUSED(x) 									( (void)(x) )
#endif

/*_____ F U N C T I O N S __________________________________________________*/

uint8_t mask_to_length(uint32_t mask);
void convertToBin(unsigned long n, unsigned char bit_start, unsigned char bit_end,unsigned char target_bit);
void compare_buffer(unsigned char *src, unsigned char *des, int nBytes);
void reset_buffer(void *dest, unsigned long val, unsigned long size);
void copy_buffer(void *dest, void *src, unsigned long size);
void dump_buffer16(unsigned short *pucBuff, int nBytes);
void dump_buffer16_hex(unsigned short *pucBuff, int nBytes);
void dump_buffer8(unsigned char *pucBuff, int nBytes);
void dump_buffer8_hex(unsigned char *pucBuff, int nBytes);

#endif //__MISC_CONFIG_H__
