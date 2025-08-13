/*===========================================================================*/
/* Project:  F1x StarterKit V3 Sample Software                               */
/* Module :  r_port.c                                                        */
/* Version:  V1.00                                                           */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2016 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* In case of any question please do not hesitate to contact:                */
/*                                                                           */
/*        ABG Software Tool Support                                          */
/*                                                                           */
/*        Renesas Electronics Europe GmbH                                    */
/*        Arcadiastrasse 10                                                  */
/*        D-40472 Duesseldorf, Germany                                       */
/*                                                                           */
/*        e-mail: software_support-eu@lm.renesas.com                         */
/*        FAX:   +49 - (0)211 / 65 03 - 11 31                                */
/*                                                                           */
/*===========================================================================*/
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/*                                                                           */
/* Source code for the port configuration functions.                         */
/*                                                                           */
/*===========================================================================*/

/*===========================================================================*/
/* Includes */
/*===========================================================================*/
#include "r_smc_entry.h"
#include "r_port.h"

/*===========================================================================*/
/* Functions */
/*===========================================================================*/
/*****************************************************************************
** Function:    R_PORT_SetGpioOutput
** Description: Set Port_Pin to GPIO Output. Initial output level is low.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
**              Level: Output level High or Low
** Return:      None
******************************************************************************/
void R_PORT_SetGpioOutput(enum port_t Port, uint32_t Pin, enum level_t Level)
{
    if(Level == Low)
        {
            *PortList[Port].P_Reg &= ~(1u<<Pin);
        }
    else /* Level = High */
        {
            *PortList[Port].P_Reg |= (1u<<Pin);
        }
    *PortList[Port].PM_Reg &= ~(1u<<Pin);
    *PortList[Port].PMC_Reg &= ~(1u<<Pin);
}

/*****************************************************************************
** Function:    R_PORT_ToggleGpioOutput
** Description: Toggles the output level of Port_Pin.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_ToggleGpioOutput(enum port_t Port, uint32_t Pin)
{
    *PortList[Port].PNOT_Reg |= 1<<Pin;
}

/*****************************************************************************
** Function:    R_PORT_SetGpioInput
** Description: Set Port_Pin to GPIO Input with Port Input Buffer enabled.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_SetGpioInput(enum port_t Port, uint32_t Pin)
{
    *PortList[Port].PM_Reg |= 1<<Pin;
    *PortList[Port].PIBC_Reg |= 1<<Pin;
    *PortList[Port].PMC_Reg &= ~(1u<<Pin);
}

/*****************************************************************************
** Function:    R_PORT_SetGpioHighZ
** Description: Set Port_Pin to GPIO High Impedant Input (Port Input Buffer disabled).
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_SetGpioHighZ(enum port_t Port, uint32_t Pin)
{
    *PortList[Port].PIBC_Reg &= ~(1<<Pin);
    *PortList[Port].PM_Reg |= 1<<Pin;
}

/*****************************************************************************
** Function:    R_PORT_SetAltFunc
** Description: Configures Port_Pin to the chosen alternative function.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
**              Alt: Alternative Function (Alt1-Alt7)
**              IO: Input/Output direction
** Return:      None
******************************************************************************/
void R_PORT_SetAltFunc(enum port_t Port, uint32_t Pin, enum alt_t Alt, enum io_t IO)
{   
    switch(Alt)
    {
        case Alt1:
            *PortList[Port].PFCAE_Reg &= ~(1<<Pin);
            *PortList[Port].PFCE_Reg &= ~(1<<Pin);
            *PortList[Port].PFC_Reg &= ~(1<<Pin);
        break;
        
        case Alt2:
            *PortList[Port].PFCAE_Reg &= ~(1<<Pin);
            *PortList[Port].PFCE_Reg &= ~(1<<Pin);
            *PortList[Port].PFC_Reg |= 1<<Pin;
        break;
        
        case Alt3:
            *PortList[Port].PFCAE_Reg &= ~(1<<Pin);
            *PortList[Port].PFCE_Reg |= 1<<Pin;
            *PortList[Port].PFC_Reg &= ~(1<<Pin);
        break;
        
        case Alt4:
            *PortList[Port].PFCAE_Reg &= ~(1<<Pin);
            *PortList[Port].PFCE_Reg |= 1<<Pin;
            *PortList[Port].PFC_Reg |= 1<<Pin;
        break;
        
        case Alt5:
            *PortList[Port].PFCAE_Reg |= 1<<Pin;
            *PortList[Port].PFCE_Reg &= ~(1<<Pin);
            *PortList[Port].PFC_Reg &= ~(1<<Pin);
        break;
        
        case Alt6:
            *PortList[Port].PFCAE_Reg |= 1<<Pin;
            *PortList[Port].PFCE_Reg &= ~(1<<Pin);
            *PortList[Port].PFC_Reg |= 1<<Pin;
        break;
        
        case Alt7:
            *PortList[Port].PFCAE_Reg |= 1<<Pin;
            *PortList[Port].PFCE_Reg |= 1<<Pin;
            *PortList[Port].PFC_Reg &= ~(1<<Pin);
        break;
        
        default:
        break;
    }
    
    switch(IO)
    {
        case Input:
            *PortList[Port].PM_Reg |= 1<<Pin;
        break;
        
        case Output:
            *PortList[Port].PM_Reg &= ~(1<<Pin);
        break;
        
        default:
        break;
        
    }
    
    *PortList[Port].PMC_Reg |= 1u<<Pin;
}


/*****************************************************************************
** Function:    R_PORT_GetLevel
** Description: Gets the state of a Pin.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      0 - Pin is low
**              1 - Pin is high
******************************************************************************/
uint32_t R_PORT_GetLevel(enum port_t Port, uint32_t Pin)
{
    uint16_t PortLevel;
    
    PortLevel = *PortList[Port].PPR_Reg;
    PortLevel &= 1<<Pin;
    
    if(PortLevel == 0)
        {
            return 0;
        }
    else
        {
            return 1;
        }
}
/*****************************************************************************
** Function:    R_PORT_GetPort
** Description: Gets the state of a Port.
** Parameter:   Port: Portgroup
** Return:      0 - Pin is low
**              1 - Pin is high
******************************************************************************/
uint16_t R_PORT_GetPort(enum port_t Port)
{
    uint16_t PortLevel;
    
    PortLevel = *PortList[Port].PPR_Reg;
    return PortLevel;
}
/*****************************************************************************
** Function:    R_PORT_SetOpenDrain
** Description: Sets pin characteristics to open-drain.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_SetOpenDrain(enum port_t Port, uint32_t Pin)
{
  uint32_t loc_podcValue;
  loc_podcValue = *PortList[Port].PODC_Reg;
  loc_podcValue |= 1<<Pin;
  protected_write(*PortList[Port].PPCMD_Reg, *PortList[Port].PPROTS_Reg, *PortList[Port].PODC_Reg, loc_podcValue);
}  

/*****************************************************************************
** Function:    R_PORT_SetPushPull
** Description: Sets pin characteristics to push-pull.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/ 
void R_PORT_SetPushPull(enum port_t Port, uint32_t Pin)
{
  uint32_t loc_podcValue;
  loc_podcValue = *PortList[Port].PODC_Reg;
  loc_podcValue &= ~(1<<Pin);
  protected_write(*PortList[Port].PPCMD_Reg, *PortList[Port].PPROTS_Reg, *PortList[Port].PODC_Reg, loc_podcValue);
}

/*****************************************************************************
** Function:    R_PORT_ConnectPullUp
** Description: Connects internal pull up
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/ 
void R_PORT_ConnectPullUp(enum port_t Port, uint32_t Pin)
{
	*PortList[Port].PU_Reg |= (1<<Pin);
}

/*****************************************************************************
** Function:    R_PORT_DisconnectPullUp
** Description: Disconnects internal pull up
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/ 
void R_PORT_DisconnectPullUp(enum port_t Port, uint32_t Pin)
{
	*PortList[Port].PU_Reg &= ~(1<<Pin);
}
/*****************************************************************************
** Function:    R_PORT_ConnectPullDown
** Description: Connects internal pull down
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/ 
void R_PORT_ConnectPullDown(enum port_t Port, uint32_t Pin)
{
	*PortList[Port].PD_Reg |= (1<<Pin);
}

/*****************************************************************************
** Function:    R_PORT_DisconnectPullDown
** Description: Connects internal pull down
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/ 
void R_PORT_DisconnectPullDown(enum port_t Port, uint32_t Pin)
{
	*PortList[Port].PD_Reg &= ~(1<<Pin);
}

/*****************************************************************************
** Function:    R_PORT_SetAnalogFilter
** Description: Sets analog filter setting.
** Parameter:   fcla_signal_t InputSignal:  R_FCLA_NMI or
**                                          R_FLCA_INTPn (n=0-15)
**
**              FilterSetting
** Return:      None
******************************************************************************/ 
void R_PORT_SetAnalogFilter(enum fcla_signal_t InputSignal, uint8_t FilterSetting)
{
    volatile uint8_t *loc_FCLA_INTP = (volatile uint8_t *)&FCLA0.CTL0_INTPL;
    
    if(InputSignal == R_FCLA_NMI)
        {
            FCLA0.CTL0_NMI = (FilterSetting&0x07);
        }
    else
        {
            loc_FCLA_INTP += (InputSignal*4);
            *loc_FCLA_INTP = (FilterSetting&0x07);
        }
}

/*****************************************************************************
** Function:    R_PORT_SetDigitalFilter
** Description: Enables and sets digital filter setting. (Filter Settings are 
**				set for all channels of Filter Type E)
** Parameter:   dnfa_signal_t InputSignal:  R_DNFA_TAUB0In (n=0-15) or
**                                          R_DNFA_TAUD0In (n=0-15)
**
**              FilterSetting
** Return:      None
******************************************************************************/ 
void R_PORT_SetDigitalFilter(enum dnfa_signal_t InputSignal, uint8_t FilterSetting)
{
   if(InputSignal >= R_DNFA_TAUD0I0 && InputSignal <= R_DNFA_TAUD0I15)
	{
		DNF.ATAUD0IEN  |= (1 << InputSignal);
		DNF.ATAUD0ICTL |= (FilterSetting&0x67);
	}
    else if(InputSignal >= R_DNFA_TAUB0I0 && InputSignal <= R_DNFA_TAUB0I15)
	{
		DNF.ATAUB0IEN  |= (1 << (InputSignal - 0x10));
		DNF.ATAUB0ICTL |= (FilterSetting&0x67);
	}
/*	else if(InputSignal >= R_DNFA_TAUB1I0 && InputSignal <= R_DNFA_TAUB1I15)
	{
		DNFATAUB1IEN  |= (1 << (InputSignal - 0x20));
		DNFATAUB1ICTL |= (FilterSetting&0x67);		
	}
*/
	else if(InputSignal >= R_DNFA_ENCA0TIN0 && InputSignal <= R_DNFA_ENCEC)
	{
		DNF.AENCA0IEN  |= (1 << (InputSignal - 0x30));
		DNF.AENCA0ICTL |= (FilterSetting&0x67);		
	}
	else if(InputSignal >= R_DNFA_SENT0RX && InputSignal <= R_DNFA_SENT1RX)
	{
		DNF.ASENTIEN  |= (1 << (InputSignal - 0x40));
		DNF.ASENTICTL |= (FilterSetting&0x67);		
	}
}
/*****************************************************************************
** Function:    R_PORT_ResetDigitalFilter
** Description: Resets digital filter setting.
** Parameter:   fcla_signal_t InputSignal:  R_FCLA_NMI or
**                                          R_FLCA_INTPn (n=0-15)
**
**              FilterSetting
** Return:      None
******************************************************************************/ 
void R_PORT_ResetDigitalFilter(enum dnfa_signal_t InputSignal)
{
	// Nach obigem Vorbild bauen!
    
	// Bis dahin:
	/*
	if (Port==Port9 && Pin == 0)
		DNFASENTIENL 	|=  0x00;
	if (Port==Port0 && Pin == 0)
		DNFATAUD0IEN	|=  0x00;
	*/
	
	if(InputSignal >= R_DNFA_TAUD0I0 && InputSignal <= R_DNFA_TAUD0I15)
	{
		DNF.ATAUD0IEN  &= ~(1 << InputSignal);
	}
    else if(InputSignal >= R_DNFA_TAUB0I0 && InputSignal <= R_DNFA_TAUB0I15)
	{
		DNF.ATAUB0IEN  &= ~(1 << (InputSignal - 0x10));
	}
/*	else if(InputSignal >= R_DNFA_TAUB1I0 && InputSignal <= R_DNFA_TAUB1I15)
	{
		DNFATAUB1IEN  &= ~(1 << (InputSignal - 0x20));	
	}
*/
	else if(InputSignal >= R_DNFA_ENCA0TIN0 && InputSignal <= R_DNFA_ENCEC)
	{
		DNF.AENCA0IEN  &= ~(1 << (InputSignal - 0x30));	
	}
	else if(InputSignal >= R_DNFA_SENT0RX && InputSignal <= R_DNFA_SENT1RX)
	{
		DNF.ASENTIEN  &= ~(1 << (InputSignal - 0x40));	
	}
}

/*****************************************************************************
** Function:    R_PORT_EnableIpControl
** Description: Enables Input/Output direction control by IP.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_EnableIpControl(enum port_t Port, uint32_t Pin)
{
    *PortList[Port].PIPC_Reg |= 1<<Pin;
}

/*****************************************************************************
** Function:    R_PORT_DisableIpControl
** Description: Disables Input/Output direction control by IP.
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_DisableIpControl(enum port_t Port, uint32_t Pin)
{
    *PortList[Port].PIPC_Reg &= ~(1<<Pin);
}

/*****************************************************************************
** Function:    R_PORT_EnableFastMode
** Description: Set output drive strength to high (fast mode).
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_EnableFastMode(enum port_t Port, uint32_t Pin)
{
    uint32_t loc_pdscValue;
    
    loc_pdscValue = (*PortList[Port].PDSC_Reg);
    loc_pdscValue |= (1<<Pin);
    
    protected_write(*PortList[Port].PPCMD_Reg, *PortList[Port].PPROTS_Reg, *PortList[Port].PDSC_Reg, loc_pdscValue);
}

/*****************************************************************************
** Function:    R_PORT_DisableFastMode
** Description: Set output drive strength to low (slow mode).
** Parameter:   Port: Portgroup
**              Pin: Pin Number
** Return:      None
******************************************************************************/
void R_PORT_DisableFastMode(enum port_t Port, uint32_t Pin)
{
    uint32_t loc_pdscValue;
    
    loc_pdscValue = (*PortList[Port].PDSC_Reg);
    loc_pdscValue &= ~(1<<Pin);
    
    protected_write(*PortList[Port].PPCMD_Reg, *PortList[Port].PPROTS_Reg, *PortList[Port].PDSC_Reg, loc_pdscValue);
}

