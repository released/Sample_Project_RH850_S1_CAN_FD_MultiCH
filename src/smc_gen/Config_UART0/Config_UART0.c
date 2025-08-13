/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018, 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_UART0.c
* Component Version: 1.5.1
* Device(s)        : R7F701684
* Description      : This file implements device driver for Config_UART0.
***********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_UART0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t *  gp_uart0_tx_address;    /* uart0 transmit data address */
volatile uint16_t   g_uart0_tx_count;       /* uart0 transmit data number */
volatile uint8_t *  gp_uart0_rx_address;    /* uart0 receive data address */
volatile uint16_t   g_uart0_rx_count;       /* uart0 receive data number */
volatile uint16_t   g_uart0_rx_length;      /* uart0 receive data length */
extern volatile uint32_t g_cg_sync_read;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Create
* Description  : This function initializes the Config_UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create(void)
{
    uint32_t tmp_port;

    /* Set LIN reset mode */
    RLN30.LCUC = _UART_LIN_RESET_MODE_CAUSED;
    /* Disable ICRLIN30UR0 operation and clear request */
    INTC2.ICRLIN30UR0.BIT.MKRLIN30UR0 = _INT_PROCESSING_DISABLED;
    INTC2.ICRLIN30UR0.BIT.RFRLIN30UR0 = _INT_REQUEST_NOT_OCCUR;
    /* Disable ICRLIN30UR1 operation and clear request */
    INTC2.ICRLIN30UR1.BIT.MKRLIN30UR1 = _INT_PROCESSING_DISABLED;
    INTC2.ICRLIN30UR1.BIT.RFRLIN30UR1 = _INT_REQUEST_NOT_OCCUR;
    /* Disable ICRLIN30UR2 operation and clear request */
    INTC2.ICRLIN30UR2.BIT.MKRLIN30UR2 = _INT_PROCESSING_DISABLED;
    INTC2.ICRLIN30UR2.BIT.RFRLIN30UR2 = _INT_REQUEST_NOT_OCCUR;
    /* Set ICRLIN30UR0 table method */
    INTC2.ICRLIN30UR0.BIT.TBRLIN30UR0 = _INT_TABLE_VECTOR;
    /* Set ICRLIN30UR0 priority */
    INTC2.ICRLIN30UR0.UINT16 &= _INT_PRIORITY_LOWEST;
    /* Set ICRLIN30UR1 table method */
    INTC2.ICRLIN30UR1.BIT.TBRLIN30UR1 = _INT_TABLE_VECTOR;
    /* Set ICRLIN30UR1 priority */
    INTC2.ICRLIN30UR1.UINT16 &= _INT_PRIORITY_LOWEST;
    /* Set ICRLIN30UR2 table method */
    INTC2.ICRLIN30UR2.BIT.TBRLIN30UR2 = _INT_TABLE_VECTOR;
    /* Set ICRLIN30UR2 priority */
    INTC2.ICRLIN30UR2.UINT16 &= _INT_PRIORITY_LOWEST;
    /* Set UART0 setting */
    RLN30.LWBR = _UART_6_SAMPLING | _UART_PRESCALER_CLOCK_SELECT_2;
    RLN30.LBRP01.UINT16 = _UART0_BAUD_RATE_PRESCALER;
    RLN30.LMD = _UART_NOISE_FILTER_ENABLED | _UART_MODE_SELECT;
    RLN30.LEDE = _UART_FRAMING_ERROR_DETECTED | _UART_OVERRUN_ERROR_DETECTED;
    RLN30.LBFC = _UART_TRANSMISSION_NORMAL | _UART_RECEPTION_NORMAL | _UART_PARITY_PROHIBITED | _UART_STOP_BIT_1 | 
                 _UART_LSB | _UART_LENGTH_8;
    RLN30.LCUC = _UART_LIN_RESET_MODE_CANCELED;
    /* Synchronization processing */
    g_cg_sync_read = RLN30.LCUC;
    __syncp();
    /* Set RLIN30RX pin */
    PORT.PIBC10 &= _PORT_CLEAR_BIT9;
    PORT.PBDC10 &= _PORT_CLEAR_BIT9;
    PORT.PM10 |= _PORT_SET_BIT9;
    PORT.PMC10 &= _PORT_CLEAR_BIT9;
    PORT.PFC10 &= _PORT_CLEAR_BIT9;
    PORT.PFCE10 |= _PORT_SET_BIT9;
    PORT.PFCAE10 |= _PORT_SET_BIT9;
    PORT.PMC10 |= _PORT_SET_BIT9;
    /* Set RLIN30TX pin */
    PORT.PIBC10 &= _PORT_CLEAR_BIT10;
    PORT.PBDC10 &= _PORT_CLEAR_BIT10;
    PORT.PM10 |= _PORT_SET_BIT10;
    PORT.PMC10 &= _PORT_CLEAR_BIT10;
    tmp_port = PORT.PDSC10;
    PORT.PPCMD10 = _WRITE_PROTECT_COMMAND;
    PORT.PDSC10 = (tmp_port & _PORT_CLEAR_BIT10);
    PORT.PDSC10 = (uint32_t) ~(tmp_port & _PORT_CLEAR_BIT10);
    PORT.PDSC10 = (tmp_port & _PORT_CLEAR_BIT10);
    PORT.PFC10 |= _PORT_SET_BIT10;
    PORT.PFCE10 &= _PORT_CLEAR_BIT10;
    PORT.PFCAE10 &= _PORT_CLEAR_BIT10;
    PORT.PMC10 |= _PORT_SET_BIT10;
    PORT.PM10 &= _PORT_CLEAR_BIT10;

    R_Config_UART0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Start
* Description  : This function starts the Config_UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Start(void)
{
    /* Enable UART0 operation */
    RLN30.LUOER |= _UART_RECEPTION_ENABLED | _UART_TRANSMISSION_ENABLED;
    /* Clear ICRLIN30UR0 interrupt request and enable operation */
    INTC2.ICRLIN30UR0.BIT.RFRLIN30UR0 = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICRLIN30UR0.BIT.MKRLIN30UR0 = _INT_PROCESSING_ENABLED;
    /* Clear ICRLIN30UR1 interrupt request and enable operation */
    INTC2.ICRLIN30UR1.BIT.RFRLIN30UR1 = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICRLIN30UR1.BIT.MKRLIN30UR1 = _INT_PROCESSING_ENABLED;
    /* Clear ICRLIN30UR2 interrupt request and enable operation */
    INTC2.ICRLIN30UR2.BIT.RFRLIN30UR2 = _INT_REQUEST_NOT_OCCUR;
    INTC2.ICRLIN30UR2.BIT.MKRLIN30UR2 = _INT_PROCESSING_ENABLED;
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Stop
* Description  : This function stops the CSIH0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Stop(void)
{
    /* Disable ICRLIN30UR0 operation */
    INTC2.ICRLIN30UR0.BIT.MKRLIN30UR0 = _INT_PROCESSING_DISABLED;
    /* Disable ICRLIN30UR1 operation */
    INTC2.ICRLIN30UR1.BIT.MKRLIN30UR1 = _INT_PROCESSING_DISABLED;
    /* Disable ICRLIN30UR2 operation */
    INTC2.ICRLIN30UR2.BIT.MKRLIN30UR2 = _INT_PROCESSING_DISABLED;
    /* Disable UART0 operation */
    RLN30.LUOER &= (uint8_t) ~(_UART_RECEPTION_ENABLED | _UART_TRANSMISSION_ENABLED);
    /* Synchronization processing */
    g_cg_sync_read = RLN30.LCUC;
    __syncp();
    /* Clear ICRLIN30UR0 request */
    INTC2.ICRLIN30UR0.BIT.RFRLIN30UR0 = _INT_REQUEST_NOT_OCCUR;
    /* Clear ICRLIN30UR1 request */
    INTC2.ICRLIN30UR1.BIT.RFRLIN30UR1 = _INT_REQUEST_NOT_OCCUR;
    /* Clear ICRLIN30UR2 request */
    INTC2.ICRLIN30UR2.BIT.RFRLIN30UR2 = _INT_REQUEST_NOT_OCCUR;
    /* Synchronization processing */
    g_cg_sync_read = RLN30.LCUC;
    __syncp();
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Receive
* Description  : This function receives UART0 data.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
MD_STATUS R_Config_UART0_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;
    if (rx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        if (( RLN30.LST & _UART_RECEPTION_OPERATED ) == 0U)
        {
            g_uart0_rx_count = 0U;
            g_uart0_rx_length = rx_num;
            gp_uart0_rx_address = rx_buf;
        }
        else
        {
            status = MD_ERROR;
        }
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Send
* Description  : This function sends UART0 data.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
MD_STATUS R_Config_UART0_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;
    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;
        if ((RLN30.LST & _UART_TRANSMISSION_OPERATED) == 0U)
        {
            /* Clear ICRLIN30UR0 request and Disable operation */
            INTC2.ICRLIN30UR0.BIT.MKRLIN30UR0 = _INT_PROCESSING_DISABLED;
            INTC2.ICRLIN30UR0.BIT.RFRLIN30UR0 = _INT_REQUEST_NOT_OCCUR;
            RLN30.LUTDR.UINT16 = *gp_uart0_tx_address;
            gp_uart0_tx_address++;
            g_uart0_tx_count--;
            /* Clear ICRLIN30UR0 requestt and enable operation */
            INTC2.ICRLIN30UR0.BIT.RFRLIN30UR0 = _INT_REQUEST_NOT_OCCUR;
            INTC2.ICRLIN30UR0.BIT.MKRLIN30UR0 = _INT_PROCESSING_ENABLED;
        }
        else
        {
            status = MD_ERROR;
        }
    }
    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
