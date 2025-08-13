# Sample_Project_RH850_S1_CAN_FD_MultiCH
Sample_Project_RH850_S1_CAN_FD_MultiCH

update @ 2025/08/13

1. initial RH850 EVB - RH850/F1KM-S1 (BLDC) Starter Kit , to test below function 

- UART : RX:P10_9 , TX:P10_10

- CAN1 : RX:P10_7 (polling , no RX rule , accept all ID) , TX:P10_6 (polling)

- CAN1 : RX:P10_7 (interrupt , no RX rule , accept all ID) , TX:P10_6 (polling)

- CAN4 : RX:P0_10 (polling , no RX rule , accept all ID) , TX:P0_9 (polling)

- CAN4 : RX:P0_10 (interrupt , no RX rule , accept all ID) , TX:P0_9 (polling)

	- use #define ENABLE_MULTI_CAN_CH , to multi CH setting( ex : CAN1 , CAN4 )

	- use #define CAN_RX_POLLING , #define CAN_RX_INTERRUPT , to change RX receive method

	- check : p->rrt_handle[q].mask.bit.MID=ALL_ID_BIT_IS_NOT_COMPARED;

```c
static void can_rrt_set(CAN_REG_TYP * can,
                          CAN_BUS_HANDLE *p,
                          const CAN_RX_RULE_TABLE_T *rule)
{
    unsigned int q=0;

    for(q=0;q<CAN_RX_RULE_TABLE_AMOUNT;q++)
    {
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=q;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=0;
        p->rrt_handle[q].id.bit.RTR=0;
        
        //RCFDCnCFDGAFLMj
        // p->rrt_handle[q].mask.bit.MID=STANDARD_ID_BIT_IS_COMPARED;
        // p->rrt_handle[q].mask.bit.MID=EXTEND_ID_BIT_IS_COMPARED;//The corresponding ID bit is compared
        p->rrt_handle[q].mask.bit.MID=ALL_ID_BIT_IS_NOT_COMPARED;
```

RX use polling or interrupt
```c
// #define CAN_RX_POLLING
#define CAN_RX_INTERRUPT
```

check define : ENABLE_MULTI_CAN_CH
```c
#define ENABLE_MULTI_CAN_CH
```

- __KEYPOINT1__ : when CAN_RX_FIFO_BUFFER_NUM is NOT CAN_RX_FIFO_BUFFER_NUMBER0 , need to shift the q_number with multiple words 

	- 0x80 byte (128 byte) , which is 32 WORD

```c
volatile CAN_BUS_PARAMETER_T can_bus_parameter_ch4 = 
{
    .CAN_CH                 = CAN_CHANNEL_4,
    .CAN_MODE               = CAN_FD_MIX_MODE,
    .CAN_RX_FIFO_BUFFER_NUM = CAN_RX_FIFO_BUFFER_NUMBER3,
```


```c
under signed char can_fd_receive_buffer_decode(CAN_REG_TYP * can)

    unsigned short q_number = can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM << 5;

under void can_fd_receive_fifo_buffer_decode(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number)
   
    unsigned short q_number = rfi_number << 5;
```

- below is register for reference

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/Receive_FIFO_register.jpg)

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/Receive_FIFO_buffer_register.jpg)


- __KEYPOINT2__ : Sixteen receive rules can be set per page. 

	- each RX channel RULE , must align to 16 or need to copy one by one

```c
#define CAN_RX_RULE_CURRENT_AMOUNT                  (16)
#define CAN2_RX_RULE_CURRENT_AMOUNT                 (16)
```
can_rrt_set , CAN1 , use PAGE0
```c
    can_init_rx_rule_table_page_setting(can,
                                        CAN_RX_RULE_TABLE_PAGE0,
                                        &p->rrt_handle[0],
                                        rule_count);
```

can2_rrt_set , CAN4 , use PAGE1
```c

    can_init_rx_rule_table_page_setting(can,
                                        CAN_RX_RULE_TABLE_PAGE1,
```

2. Below is EVB switch

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/EVB_CAN_cfg.jpg)

3. Below is PCAN config setting 

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/PCAN_cfg.jpg)

4. Below is power on message

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_MCU_power_on.jpg)

5. when use UART terminal , which send CAN TX message from RH850 EVB , and rececive with PCAN

use digit 1 ~ 4 , to send CAN1 TX data 

use digit 5 ~ 8 , to send CAN4 TX data 

digit 1 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx1.jpg)


digit 2 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx2.jpg)


digit 3 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx3.jpg)


digit 4 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx4.jpg)


digit 5 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx5.jpg)


digit 6 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx6.jpg)


digit 7 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx7.jpg)


digit 8 , 
![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/log_tx8.jpg)


6. Below is different ID test condition , which send by PCAN (refer to PCAN_xfer.xmt) , and rececive with RH850 RX


- ID : 55 , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_055.jpg)


- ID : 66 , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_066.jpg)


- ID : 77 , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_077.jpg)


- ID : 100 , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_100.jpg)


- ID : 103 , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_103.jpg)


- ID : 103 RTR , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_103_RTR.jpg)


- ID : 123 , polling

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_pollig_ID_123.jpg)


- ID : 00 , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_000.jpg)


- ID : 44 , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_044.jpg)


- ID : 7F , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_07F.jpg)


- ID : 88 , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_088.jpg)


- ID : 102_RTR , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_102_RTR.jpg)


- ID : 102_RTR_extend , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_102_RTR_extend.jpg)


- ID : 104 , interrupt

![image](https://github.com/released/Sample_Project_RH850_S1_CAN_FD_MultiCH/blob/main/rx_interrupt_ID_104.jpg)


7. How to initial interrupt function , 

- Add function declare in r_cg_intvector.c , need to replace when code generage EACH TIMES

SEARCH : extern void eiint23(void);

replace with : extern void can_rx_fifo_interrupt(void);

```c
/* CAN receive FIFO interrupt; */
extern void can_rx_fifo_interrupt(void);
/* CAN0 error interrupt; */
extern void eiint24(void);
```

SEARCH : (void *)eiint23,

replace with : (void *)can_rx_fifo_interrupt,

```c
/* CAN receive FIFO interrupt; */
(void *)can_rx_fifo_interrupt,
/* CAN0 error interrupt; */
(void *)eiint24,
```

- Add interrupt handler declare with irq number 

```c
#pragma interrupt can_rx_fifo_interrupt(enable=false, channel=23, fpu=true, callt=false)
void can_rx_fifo_interrupt(void)
{
    /* Start user code for can_rx_fifo_interrupt. Do not edit comment generated here */
    can_rx_interrupt_cbk();
    /* End user code. Do not edit comment generated here */
}
```

- Add interrupt initial function , 

```c

void R_CANFD_Interrupt_Control_Init(void)
{
	// 23   ICRCANGRECC0  	FFFE EA2E H   INTRCANGRECC0  	CAN receive FIFO interrupt

    /*INTRCANGRECC0 : CAN CHANNEL RX FIFO interrupt*/

    INTC1.ICRCANGRECC0.BIT.CTRCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.RFRCANGRECC0 = _INT_REQUEST_NOT_OCCUR;        
    INTC1.ICRCANGRECC0.BIT.MKRCANGRECC0 = _INT_PROCESSING_ENABLED;
    INTC1.ICRCANGRECC0.BIT.TBRCANGRECC0 = _INT_TABLE_VECTOR; //select table interrupt

    INTC1.ICRCANGRECC0.BIT.P3RCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.P2RCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.P1RCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.P0RCANGRECC0 = 1;
}


