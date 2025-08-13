#ifndef __RETARGET_H__
#define __RETARGET_H__

/*_____ I N C L U D E S ____________________________________________________*/
// #include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "iodefine.h"
#include "r_cg_macrodriver.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
// #define UART_XFER                                       (RLN30.LUTDR.UINT16)                    //SDR00L
// #define UART_XFER                                       (RLN30.LUTDR.UINT8[L])                  //SDR00L
// #define DISABLE_UART_INT_STATUS_FLAG                    (INTC2.ICRLIN30UR2.BIT.MKRLIN30UR2)     //STMK0
// #define CLEAR_UART_INT_STATUS_FLAG                      (INTC2.ICRLIN30UR2.BIT.RFRLIN30UR2)     //STIF0


/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void SendChar(unsigned char ch);
int putchar(int c);

// int fputc(int ch, FILE *f);
// int fputs(const char *str, FILE *fp);
// int puts(const char *s);

int tiny_printf(const char *fmt, ...);

#endif //__RETARGET_H__
