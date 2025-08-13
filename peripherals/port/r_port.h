/*===========================================================================*/
/* Project:  F1x StarterKit V3 Sample Software                               */
/* Module :  r_port.h                                                        */
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
/* History:                                                                  */
/*              V1.00: Initial version                                       */
/*                                                                           */
/*===========================================================================*/
#ifndef R_PORT_H
#define R_PORT_H

/*===========================================================================*/
/* Includes */
/*===========================================================================*/
#include "r_smc_entry.h"
/*===========================================================================*/
/* Defines */
/*===========================================================================*/


//============================================================================
// Defines
//============================================================================


#define protected_write(preg,pstatus,reg,value)   do{\
                                                  (preg)=0xa5u;\
                                                  (reg)=(value);\
                                                  (reg)=~(value);\
                                                  (reg)=(value);\
                                                  }while((pstatus)==1u)


/* Defines for filter settings */
#define R_FCLA_DISABLE_DETECTION		0x00
#define R_FCLA_RISING_EDGE				0x01
#define R_FCLA_FALLING_EDGE				0x02
#define R_FCLA_RISING_AND_FALLING_EDGE	0x03
#define R_FCLA_LOW_LEVEL				0x04
#define R_FCLA_HIGH_LEVEL				0x05


#define R_DNFA_NR_OF_SAMPLES_2				(0x00<<5)
#define R_DNFA_NR_OF_SAMPLES_3				(0x01<<5)
#define R_DNFA_NR_OF_SAMPLES_4				(0x02<<5)
#define R_DNFA_NR_OF_SAMPLES_5				(0x03<<5)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_1		(0x00)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_2		(0x01)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_4		(0x02)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_8		(0x03)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_16	(0x04)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_32	(0x05)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_64	(0x06)
#define R_DNFA_SAMPLING_CLOCK_DEVIDER_128	(0x07)

/* Enumeration for alternative functions of port pins */
enum alt_t
{
    Alt1,
    Alt2,
    Alt3,
    Alt4,
    Alt5,
    Alt6,
    Alt7
};

enum io_t
{
    Input,
    Output
};

enum level_t
{
    Low = 0,
    High = 1
};

/* Enumeration for analog filter signals */
enum fcla_signal_t
{
    R_FCLA_INTP0 = 0,
    R_FCLA_INTP1,
    R_FCLA_INTP2,
    R_FCLA_INTP3,
    R_FCLA_INTP4,
    R_FCLA_INTP5,
    R_FCLA_INTP6,
    R_FCLA_INTP7,
    R_FCLA_INTP8,
    R_FCLA_INTP9,
    R_FCLA_INTP10,
    R_FCLA_INTP11,
    R_FCLA_INTP12,
    R_FCLA_INTP13,
    R_FCLA_INTP14,
    R_FCLA_INTP15,
    R_FCLA_NMI = 0x10
};

/* Enumeration for digital filter signals */
enum dnfa_signal_t
{
	R_DNFA_TAUD0I0 = 0,
	R_DNFA_TAUD0I1,
	R_DNFA_TAUD0I2,
	R_DNFA_TAUD0I3,
	R_DNFA_TAUD0I4,
	R_DNFA_TAUD0I5,
	R_DNFA_TAUD0I6,
	R_DNFA_TAUD0I7,
	R_DNFA_TAUD0I8,
	R_DNFA_TAUD0I9,
	R_DNFA_TAUD0I10,
	R_DNFA_TAUD0I11,
	R_DNFA_TAUD0I12,
	R_DNFA_TAUD0I13,
	R_DNFA_TAUD0I14,
	R_DNFA_TAUD0I15,
	
	R_DNFA_TAUB0I0 = 0x10,
	R_DNFA_TAUB0I1,
	R_DNFA_TAUB0I2,
	R_DNFA_TAUB0I3,
	R_DNFA_TAUB0I4,
	R_DNFA_TAUB0I5,
	R_DNFA_TAUB0I6,
	R_DNFA_TAUB0I7,
	R_DNFA_TAUB0I8,
	R_DNFA_TAUB0I9,
	R_DNFA_TAUB0I10,
	R_DNFA_TAUB0I11,
	R_DNFA_TAUB0I12,
	R_DNFA_TAUB0I13,
	R_DNFA_TAUB0I14,
	R_DNFA_TAUB0I15,
	
/*	R_DNFA_TAUB1I0 = 0x20,
	R_DNFA_TAUB1I1,
	R_DNFA_TAUB1I2,
	R_DNFA_TAUB1I3,
	R_DNFA_TAUB1I4,
	R_DNFA_TAUB1I5,
	R_DNFA_TAUB1I6,
	R_DNFA_TAUB1I7,
	R_DNFA_TAUB1I8,
	R_DNFA_TAUB1I9,
	R_DNFA_TAUB1I10,
	R_DNFA_TAUB1I11,
	R_DNFA_TAUB1I12,
	R_DNFA_TAUB1I13,
	R_DNFA_TAUB1I14,
	R_DNFA_TAUB1I15,
*/	
	R_DNFA_ENCA0TIN0 = 0x30, 
	R_DNFA_ENCA0TIN1,
	R_DNFA_ENCE0,
	R_DNFA_ENCE1,
	R_DNFA_ENCEC,
	
	R_DNFA_SENT0RX = 0x40,
	R_DNFA_SENT1RX,
	
};

/* Structure of registers for each port */
struct pregs_t
{
    volatile uint16_t * P_Reg;
    volatile uint16_t * PNOT_Reg;
    volatile uint16_t * PM_Reg;
    volatile uint16_t * PMC_Reg;
    volatile uint16_t * PFC_Reg;
    volatile uint16_t * PFCE_Reg;
    volatile uint16_t * PFCAE_Reg;
    volatile uint16_t * PIPC_Reg;
    volatile uint16_t * PIBC_Reg;
    volatile const uint16_t * PPR_Reg;
    volatile uint16_t * PD_Reg;
    volatile uint16_t * PU_Reg;
    volatile uint32_t * PODC_Reg;
    volatile uint32_t * PDSC_Reg;
    volatile uint32_t * PPROTS_Reg;
    volatile uint32_t * PPCMD_Reg;
};

/* Enumeration for ports corresponding to the PortList */
enum port_t
{
    Port0 = 0,
    Port8,
    Port9,
    Port10,
    Port11,
    // Port12,
    APort0
};

/* Array with available registers for the available ports. Registers that are not available */
/* for a port are 0.                                                                        */
static const struct pregs_t PortList[]=
{
  {(volatile uint16_t *)&PORT.P0,(volatile uint16_t *) &PORT.PNOT0,(volatile uint16_t *) &PORT.PM0,(volatile uint16_t *) &PORT.PMC0, (volatile uint16_t *)&PORT.PFC0, (volatile uint16_t *)&PORT.PFCE0, (volatile uint16_t *)&PORT.PFCAE0, (volatile uint16_t *)&PORT.PIPC0, (volatile uint16_t *)&PORT.PIBC0,(volatile uint16_t *) &PORT.PPR0, (volatile uint16_t *)&PORT.PD0, (volatile uint16_t *)&PORT.PU0,(volatile uint32_t *)&PORT.PODC0, (volatile uint32_t *)&PORT.PDSC0, (volatile uint32_t *)&PORT.PPROTS0, (volatile uint32_t *)&PORT.PPCMD0},
  {(volatile uint16_t *)&PORT.P8, (volatile uint16_t *)&PORT.PNOT8, (volatile uint16_t *)&PORT.PM8, (volatile uint16_t *)&PORT.PMC8, (volatile uint16_t *)&PORT.PFC8, (volatile uint16_t *)&PORT.PFCE8, 0, 0, (volatile uint16_t *)&PORT.PIBC8, (volatile uint16_t *)&PORT.PPR8, (volatile uint16_t *)&PORT.PD8, (volatile uint16_t *)&PORT.PU8, (volatile uint32_t *)&PORT.PODC8, 0, (volatile uint32_t *)&PORT.PPROTS8, (volatile uint32_t *)&PORT.PPCMD8},
  {(volatile uint16_t *)&PORT.P9, (volatile uint16_t *)&PORT.PNOT9, (volatile uint16_t *)&PORT.PM9, (volatile uint16_t *)&PORT.PMC9, (volatile uint16_t *)&PORT.PFC9, (volatile uint16_t *)&PORT.PFCE9, (volatile uint16_t *)&PORT.PFCAE9, 0, (volatile uint16_t *)&PORT.PIBC9, (volatile uint16_t *)&PORT.PPR9, (volatile uint16_t *)&PORT.PD9, (volatile uint16_t *)&PORT.PU9, (volatile uint32_t *)&PORT.PODC9, 0, (volatile uint32_t *)&PORT.PPROTS9, (volatile uint32_t *)&PORT.PPCMD9},
  {(volatile uint16_t *)&PORT.P10, (volatile uint16_t *)&PORT.PNOT10, (volatile uint16_t *)&PORT.PM10, (volatile uint16_t *)&PORT.PMC10, (volatile uint16_t *)&PORT.PFC10, (volatile uint16_t *)&PORT.PFCE10, (volatile uint16_t *)&PORT.PFCAE10, (volatile uint16_t *)&PORT.PIPC10, (volatile uint16_t *)&PORT.PIBC10, (volatile uint16_t *)&PORT.PPR10, (volatile uint16_t *)&PORT.PD10, (volatile uint16_t *)&PORT.PU10, (volatile uint32_t *)&PORT.PODC10, (volatile uint32_t *)&PORT.PDSC10, (volatile uint32_t *)&PORT.PPROTS10, (volatile uint32_t *)&PORT.PPCMD10},
  {(volatile uint16_t *)&PORT.P11, (volatile uint16_t *)&PORT.PNOT11, (volatile uint16_t *)&PORT.PM11, (volatile uint16_t *)&PORT.PMC11, (volatile uint16_t *)&PORT.PFC11, (volatile uint16_t *)&PORT.PFCE11, (volatile uint16_t *)&PORT.PFCAE11, (volatile uint16_t *)&PORT.PIPC11, (volatile uint16_t *)&PORT.PIBC11, (volatile uint16_t *)&PORT.PPR11, (volatile uint16_t *)&PORT.PD11, (volatile uint16_t *)&PORT.PU11, (volatile uint32_t *)&PORT.PODC11, (volatile uint32_t *)&PORT.PDSC11, (volatile uint32_t *)&PORT.PPROTS11, (volatile uint32_t *)&PORT.PPCMD11},
//   {(volatile uint16_t *)&PORT.P12, (volatile uint16_t *)&PORT.PNOT12, (volatile uint16_t *)&PORT.PM12, (volatile uint16_t *)&PORT.PMC12, (volatile uint16_t *)&PORT.PFC12, (volatile uint16_t *)&PORT.PFCE12, (volatile uint16_t *)&PORT.PFCAE12, (volatile uint16_t *)&PORT.PIPC12, (volatile uint16_t *)&PORT.PIBC12, (volatile uint16_t *)&PORT.PPR12, (volatile uint16_t *)&PORT.PD12, (volatile uint16_t *)&PORT.PU12, (volatile uint32_t *)&PORT.PODC12, (volatile uint32_t *)&PORT.PDSC12, (volatile uint32_t *)&PORT.PPROTS12, (volatile uint32_t *)&PORT.PPCMD12},
  {(volatile uint16_t *)&PORT.AP0, (volatile uint16_t *)&PORT.APNOT0, (volatile uint16_t *)&PORT.APM0, 0, 0, 0, 0, 0, (volatile uint16_t *)&PORT.APIBC0, (volatile uint16_t *)&PORT.APPR0, 0, 0, 0, 0, 0, 0}
  };

/*===========================================================================*/
/* Function defines */
/*===========================================================================*/
void R_PORT_SetGpioOutput(enum port_t Port, uint32_t Pin, enum level_t Level);
void R_PORT_ToggleGpioOutput(enum port_t Port, uint32_t Pin);
void R_PORT_SetGpioInput(enum port_t Port, uint32_t Pin);
void R_PORT_SetGpioHighZ(enum port_t Port, uint32_t Pin);
void R_PORT_SetAltFunc(enum port_t Port, uint32_t Pin, enum alt_t Alt, enum io_t IO);
uint32_t R_PORT_GetLevel(enum port_t Port, uint32_t Pin);
uint16_t R_PORT_GetPort(enum port_t Port);
void R_PORT_SetOpenDrain(enum port_t Port, uint32_t Pin);
void R_PORT_SetPushPull(enum port_t Port, uint32_t Pin);
void R_PORT_ConnectPullUp(enum port_t Port, uint32_t Pin);
void R_PORT_DisconnectPullUp(enum port_t Port, uint32_t Pin);
void R_PORT_ConnectPullDown(enum port_t Port, uint32_t Pin);
void R_PORT_DisconnectPullDown(enum port_t Port, uint32_t Pin);
void R_PORT_SetAnalogFilter(enum fcla_signal_t InputSignal, uint8_t FilterSetting);
void R_PORT_SetDigitalFilter(enum dnfa_signal_t InputSignal, uint8_t FilterSetting);
void R_PORT_ResetDigitalFilter(enum dnfa_signal_t InputSignal);
void R_PORT_EnableIpControl(enum port_t Port, uint32_t Pin);
void R_PORT_DisableIpControl(enum port_t Port, uint32_t Pin);
void R_PORT_EnableFastMode(enum port_t Port, uint32_t Pin);
void R_PORT_DisableFastMode(enum port_t Port, uint32_t Pin);

#endif /* R_PORT_H */
