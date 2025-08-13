#ifndef __CUSTOM_FUNCTION_H__
#define __CUSTOM_FUNCTION_H__

/*_____ I N C L U D E S ____________________________________________________*/

/*_____ D E C L A R A T I O N S ____________________________________________*/

// #define GPIO_GETLEVEL(n,Pin)                            (PORT.PPR##n &= 1u<<Pin)
#define GPIO_GETLEVEL(n,Pin)                                (((PORT.PPR##n & 1u<<Pin) == 0U)? 0U : 1U)

#define GPIO_TOGGLE(n,Pin)                              (PORT.PNOT##n |= 1u<<Pin)
#define GPIO_HIGH(Port,Pin)                             (PORT.Port |= 1u<<Pin)
#define GPIO_LOW(Port,Pin)                              (PORT.Port &= ~(1u<<Pin))

#define CSIG_IS_BUSY(x)                                 (x.STR0 & (1 << 7))
#define CSIH_IS_BUSY(x)                                 (x.STR0 & (1 << 7))

typedef unsigned char      								bool;
// #define bool  _Bool

// #ifndef TRUE
// #define TRUE  (1)
// #endif

// #ifndef true
// #define true  (1)
// #endif

// #ifndef FALSE
// #define FALSE  (0)
// #endif

// #ifndef false
// #define false  (0)
// #endif

// #ifndef BIT0
// #define BIT0     (0x00000001UL)       ///< Bit 0 mask of an 32 bit integer
// #endif

// #ifndef BIT1
// #define BIT1     (0x00000002UL)       ///< Bit 1 mask of an 32 bit integer
// #endif

// #ifndef BIT2
// #define BIT2     (0x00000004UL)       ///< Bit 2 mask of an 32 bit integer
// #endif

// #ifndef BIT3
// #define BIT3     (0x00000008UL)       ///< Bit 3 mask of an 32 bit integer
// #endif

// #ifndef BIT4
// #define BIT4     (0x00000010UL)       ///< Bit 4 mask of an 32 bit integer
// #endif

// #ifndef BIT5
// #define BIT5     (0x00000020UL)       ///< Bit 5 mask of an 32 bit integer
// #endif

// #ifndef BIT6
// #define BIT6     (0x00000040UL)       ///< Bit 6 mask of an 32 bit integer
// #endif

// #ifndef BIT7
// #define BIT7     (0x00000080UL)       ///< Bit 7 mask of an 32 bit integer
// #endif

/*_____ D E F I N I T I O N S ______________________________________________*/

/*  
	template
	typedef struct _peripheral_manager_t
	{
		uint8_t u8Cmd;
		uint8_t au8Buf[33];
		uint8_t u8RecCnt;
		uint8_t bByPass;
		uint16_t* pu16Far;
	}PERIPHERAL_MANAGER_T;

	volatile PERIPHERAL_MANAGER_T g_PeripheralManager = 
	{
		.u8Cmd = 0,
		.au8Buf = {0},		//.au8Buf = {100U, 200U},
		.u8RecCnt = 0,
		.bByPass = FALSE,
		.pu16Far = NULL,	//.pu16Far = 0	
	};
	extern volatile PERIPHERAL_MANAGER_T g_PeripheralManager;
*/

typedef struct _uart_manager_t
{
	unsigned char g_uart0rxbuf;
	unsigned char g_uart0rxerr;
}UART_MANAGER_T;
extern volatile UART_MANAGER_T UART0Manager;
/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void ostmr_1ms_IRQ(void);
void ostimer_dealyms(unsigned long ms);

void tmr_1ms_IRQ(void);

void LED_Toggle(void);

void loop(void);

void UARTx_ErrorCheckProcess(unsigned char err);
void UARTx_Process(unsigned char rxbuf);
void RH850_software_reset(void);
void hardware_init(void);


#endif //__CUSTOM_FUNCTION_H__

