
#include "r_port.h"
#include "can.h"
#include "retarget.h"
#include "misc_config.h"

CAN_BUS_HANDLE  can_bus_handle_ch1;
CAN_BUS_HANDLE  can_bus_handle_ch4;
    
/*
    PCAN value : 
    -----------------------
    #1 
    40MHz
    nominal : 500k
        sample rate : 80
        pre-scale : 1
        TSEG1 : 63
        TSEG2 : 16
        SJW : 16

    data : 2M
        sample rate : 70
        pre-scale : 1
        TSEG1 : 13
        TSEG2 : 6
        SJW : 6

    -----------------------
    #2
    40MHz
    nominal : 500k
        sample rate : 75
        pre-scale : 1
        TSEG1 : 59
        TSEG2 : 20
        SJW : 20

    data : 2M
        sample rate : 75
        pre-scale : 1
        TSEG1 : 14
        TSEG2 : 5
        SJW : 5      

    -----------------------
    #3
    40MHz
    nominal : 500k
        sample rate : 75
        pre-scale : 1
        TSEG1 : 59
        TSEG2 : 20
        SJW : 20

    data : 5M
        sample rate : 75
        pre-scale : 1
        TSEG1 : 5
        TSEG2 : 2
        SJW : 5  
        
        SSP : 6
    
        enable TDCE , TDCOC , TDCO[[6:0]
*/

volatile CAN_BUS_GLOBAL_VARIABLE_T can_bus_variable_ch1 = 
{
    .can_tx1_flag           = 0U,
    .can_tx2_flag           = 0U,
    .can_tx3_flag           = 0U,
    .can_tx4_flag           = 0U,
    .can_rx_flag            = 0U,
    .can_rx_polling_flag    = 0U,
    .can_rx_fifo_cnt        = 0U,
    .can_rx_isr_cnt         = 0U,
};

const CAN_BUS_PARAMETER_T can_bus_parameter_ch1 = 
{
    .CAN_CH                 = CAN_CHANNEL_1,
    .CAN_MODE               = CAN_FD_MIX_MODE,
    .CAN_RX_FIFO_BUFFER_NUM = CAN_RX_FIFO_BUFFER_NUMBER0,

    // #1
    // .NBRP                   = CAN_NBRP_1,       //1U - 1U,
    // .NTSEG1                 = CAN_NTSEG1_63TQ,  //63U - 1U,
    // .NTSEG2                 = CAN_NTSEG2_16TQ,  //16U - 1U,
    // .NSJW                   = CAN_NSJW_16TQ,    //16U - 1U,

    // .DBRP                   = CAN_DBRP_1,       //1U - 1U,
    // .DTSEG1                 = CAN_DTSEG1_13TQ,  //13U - 1U,
    // .DTSEG2                 = CAN_DTSEG2_6TQ,   //6U - 1U,
    // .DSJW                   = CAN_DSJW_6TQ,     //6U - 1U,

    // #2
    .NBRP                   = CAN_NBRP_1,
    .NTSEG1                 = CAN_NTSEG1_59TQ,
    .NTSEG2                 = CAN_NTSEG2_20TQ,
    .NSJW                   = CAN_NSJW_20TQ,

    .DBRP                   = CAN_DBRP_1,
    .DTSEG1                 = CAN_DTSEG1_14TQ,
    .DTSEG2                 = CAN_DTSEG2_5TQ,
    .DSJW                   = CAN_DSJW_5TQ,

    // #3
    // .NBRP                   = CAN_NBRP_1,
    // .NTSEG1                 = CAN_NTSEG1_59TQ,
    // .NTSEG2                 = CAN_NTSEG2_20TQ,
    // .NSJW                   = CAN_NSJW_20TQ,

    // .DBRP                   = CAN_DBRP_1,
    // .DTSEG1                 = CAN_DTSEG1_5TQ,
    // .DTSEG2                 = CAN_DTSEG2_2TQ,
    // .DSJW                   = CAN_DSJW_2TQ,
};

volatile CAN_BUS_GLOBAL_VARIABLE_T can_bus_variable_ch4 = 
{
    .can_tx1_flag           = 0U,
    .can_tx2_flag           = 0U,
    .can_tx3_flag           = 0U,
    .can_tx4_flag           = 0U,
    .can_rx_flag            = 0U,
    .can_rx_polling_flag    = 0U,
    .can_rx_fifo_cnt        = 0U,
    .can_rx_isr_cnt         = 0U,
};

const CAN_BUS_PARAMETER_T can_bus_parameter_ch4 = 
{
    .CAN_CH                 = CAN_CHANNEL_4,
    .CAN_MODE               = CAN_FD_MIX_MODE,
    .CAN_RX_FIFO_BUFFER_NUM = CAN_RX_FIFO_BUFFER_NUMBER3,

    .NBRP                   = CAN_NBRP_1,
    .NTSEG1                 = CAN_NTSEG1_59TQ,
    .NTSEG2                 = CAN_NTSEG2_20TQ,
    .NSJW                   = CAN_NSJW_20TQ,

    .DBRP                   = CAN_DBRP_1,
    .DTSEG1                 = CAN_DTSEG1_14TQ,
    .DTSEG2                 = CAN_DTSEG2_5TQ,
    .DSJW                   = CAN_DSJW_5TQ,

};

volatile CAN_BUS_RX_FIFO_FLAG_T can_bus_receive_fifo_interrupt_flag = 
{
    .RxBuffer0           = 0U,
    .RxBuffer1           = 0U,
    .RxBuffer2           = 0U,
    .RxBuffer3           = 0U,
    .RxBuffer4           = 0U,
    .RxBuffer5           = 0U,
    .RxBuffer6           = 0U,
    .RxBuffer7           = 0U,

}; 


const char* tbl_can_fifo_buffer_num[] =
{
    "CAN_RX_FIFO_BUFFER_NUMBER0",
    "CAN_RX_FIFO_BUFFER_NUMBER1",
    "CAN_RX_FIFO_BUFFER_NUMBER2",
    "CAN_RX_FIFO_BUFFER_NUMBER3",
    "CAN_RX_FIFO_BUFFER_NUMBER4",
    "CAN_RX_FIFO_BUFFER_NUMBER5",
    "CAN_RX_FIFO_BUFFER_NUMBER6",
    "CAN_RX_FIFO_BUFFER_NUMBER7",
};

const char* tbl_can_dlc[] =
{
    "CAN_DLC_DISABLE_CHECK",
    "CAN_DLC_1BYTE",
    "CAN_DLC_2BYTE",
    "CAN_DLC_3BYTE",
    "CAN_DLC_4BYTE",
    "CAN_DLC_5BYTE",
    "CAN_DLC_6BYTE",
    "CAN_DLC_7BYTE",
    "CAN_DLC_8BYTE",
    "CAN_DLC_12BYTE",
    "CAN_DLC_16BYTE",
    "CAN_DLC_20BYTE",
    "CAN_DLC_24BYTE",
    "CAN_DLC_32BYTE",
    "CAN_DLC_48BYTE",
    "CAN_DLC_64BYTE",
};

// filter ID : RX_RRT0_ID ~ RX_RRT9_ID
const CAN_RX_RULE_TABLE_T tbl_can_bus_rx_rule_ch1[CAN_RX_RULE_CURRENT_AMOUNT] = 
{
    /*

        {
            .GAFLID = RX_RRT5_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,    //Receive Rule Target Message Select
            .GAFLRTR = (unsigned char)RX_DATA_FRAME,    //RTR Select
            .GAFLIDE = (unsigned char)RX_STANDARD_ID,   //IDE Select
            // .GAFLIDE = (unsigned char)RX_EXTEND_ID,   //IDE Select

            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,   //1: The corresponding ID bit is compared.
            // .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,   //1: The corresponding ID bit is compared.
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,  //1: The RTR bit is compared.
            .GAFLIDEM = IDE_BIT_IS_COMPARED,  //1: The IDE bit is compared.

            .GAFLDLC = (unsigned char)CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,   //1: A receive buffer is used.
            .GAFLPTR = 0x5555,  //Receive Rule Label

            .GAFLFDP_x = (0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,  //Transmit/Receive FIFO Buffer k Select
        },    

        {
            RX_RRT5_ID,
            OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            (unsigned char)RX_DATA_FRAME,
            (unsigned char)RX_STANDARD_ID,
            STANDARD_ID_BIT_IS_COMPARED,
            RESERVED_BIT,
            RTR_BIT_IS_COMPARED,
            IDE_BIT_IS_COMPARED,
            (unsigned char)CAN_DLC_DISABLE_CHECK,
            RESERVED_BIT,
            can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM,
            A_RECEIVE_BUFFER_IS_USED,
            0x5555,
            (0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM),
            TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
    
    */

        {
            .GAFLID = RX_RRT0_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x1234U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT1_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_STANDARD_ID,
            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x1111U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT2_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_REMOTE_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x2222U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT3_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_STANDARD_ID,
            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_NOT_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x3333U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT4_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_NOT_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x4444U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT5_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_STANDARD_ID,
            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x5555U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT6_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x6666U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT7_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x7777U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT8_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x8888U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX_RRT9_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER0,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x9999U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER0),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },

};


const CAN_RX_RULE_TABLE_T tbl_can_bus_rx_rule_ch4[CAN2_RX_RULE_CURRENT_AMOUNT] = 
{
    /*

        {
            .GAFLID = RX_RRT5_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,    //Receive Rule Target Message Select
            .GAFLRTR = (unsigned char)RX_DATA_FRAME,    //RTR Select
            .GAFLIDE = (unsigned char)RX_STANDARD_ID,   //IDE Select
            // .GAFLIDE = (unsigned char)RX_EXTEND_ID,   //IDE Select

            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,   //1: The corresponding ID bit is compared.
            // .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,   //1: The corresponding ID bit is compared.
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,  //1: The RTR bit is compared.
            .GAFLIDEM = IDE_BIT_IS_COMPARED,  //1: The IDE bit is compared.

            .GAFLDLC = (unsigned char)CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,   //1: A receive buffer is used.
            .GAFLPTR = 0x5555,  //Receive Rule Label

            .GAFLFDP_x = (0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,  //Transmit/Receive FIFO Buffer k Select
        },    

        {
            RX_RRT5_ID,
            OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            (unsigned char)RX_DATA_FRAME,
            (unsigned char)RX_STANDARD_ID,
            STANDARD_ID_BIT_IS_COMPARED,
            RESERVED_BIT,
            RTR_BIT_IS_COMPARED,
            IDE_BIT_IS_COMPARED,
            (unsigned char)CAN_DLC_DISABLE_CHECK,
            RESERVED_BIT,
            can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM,
            A_RECEIVE_BUFFER_IS_USED,
            0x5555,
            (0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM),
            TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
    
    */

        {
            .GAFLID = RX2_RRT0_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER3,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x2234U),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER3),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX2_RRT1_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_STANDARD_ID,
            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER3,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x115AU),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER3),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX2_RRT2_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_REMOTE_FRAME,
            .GAFLIDE = RX_EXTEND_ID,
            .GAFLIDM = EXTEND_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_IS_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER3,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x225AU),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER3),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },
        {
            .GAFLID = RX2_RRT3_ID,
            .GAFLLB = OTHER_NODE_TRANSMIT_MSG_RECEIVED,
            .GAFLRTR = RX_DATA_FRAME,
            .GAFLIDE = RX_STANDARD_ID,
            .GAFLIDM = STANDARD_ID_BIT_IS_COMPARED,
            .Reserved0 = RESERVED_BIT,
            .GAFLRTRM = RTR_BIT_NOT_COMPARED,
            .GAFLIDEM = IDE_BIT_IS_COMPARED,
            .GAFLDLC = CAN_DLC_DISABLE_CHECK,
            .Reserved1 = RESERVED_BIT,
            .GAFLRMDP = CAN_RX_FIFO_BUFFER_NUMBER3,
            .GAFLRMV = A_RECEIVE_BUFFER_IS_USED,
            .GAFLPTR = (0x335AU),
            .GAFLFDP_x = (0x1U<<CAN_RX_FIFO_BUFFER_NUMBER3),
            .GAFLFDP_k = TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED,
        },

};

// unsigned short can_rx_isr_cnt=0;

// void can_rx_fifo_read(CAN_REG_TYP * can,CAN_BUS_HANDLE *p, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number)
// {
//     volatile struct RFCC_SET *rfi= (volatile struct RFCC_SET *)(&can->CFDRFID0.UINT32);
//     union __tagDAT *rfsts = (union __tagDAT *)(&can->CFDRFSTS0.UINT32);
//     union __tagDAT *rfpctr = (union __tagDAT *)(&can->CFDRFPCTR0.UINT32);
//     union __tagDAT *rfdf;
//     unsigned short q_number=0;
//     unsigned short rfdf_length=0;
//     volatile unsigned short can_rx_id_read=0;

//     can_rx_id_read = rfi[rfi_number].RFID.UINT32;

//     rfdf = (union __tagDAT *)&rfi[rfi_number].RFDF0.UINT32;

//     switch(can_rx_id_read)
//     {
//     case RX_RRT0_ID:
//     	q_number=0;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);

//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;

//     	break;
//     case RX_RRT1_ID:
//     	q_number =1;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);

//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT2_ID:
//     	q_number =2;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT3_ID:
//     	q_number =3;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT4_ID:
//     	q_number =4;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT5_ID:
//     	q_number =5;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT6_ID:
//     	q_number =6;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT7_ID:
//     	q_number =7;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT8_ID:
//     	q_number =8;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     case RX_RRT9_ID:
//     	q_number =9;
//         p->rx_data[rfi_number][q_number].id.all = rfi[rfi_number].RFID.UINT32;
//         p->rx_data[rfi_number][q_number].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
//         for(rfdf_length=0;rfdf_length<16;rfdf_length++)
//         p->rx_data[rfi_number][q_number].data_u[rfdf_length].data_dw =rfdf[rfdf_length].UINT32;
//     	break;
//     default:
//     	break;
//     }

//     rfpctr[rfi_number].UINT8[0]=0xFF;

//     while (CAN_REG_READ(rfsts[rfi_number].UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_8)!=0);   //RFMC

//     CAN_REG_CLR(rfsts[rfi_number].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);

//     p->rx_data_ack.bytes |= (0x1<<rfi_number);
// }


void can_tx_buf_data_multi_send(CAN_REG_TYP * can,
                                CAN_BUS_TX_INFO_HANDLE *p,
                                CAN_CHANNEL_SEL_e channel,
                                unsigned int tx_buf_cnt_max,
								unsigned int tx_buf_data_length_max)
{
    volatile unsigned char *tmc_p = (volatile unsigned char *)(&can->CFDTMC0);
    volatile unsigned char *tmsts_p = (volatile unsigned char *)(&can->CFDTMSTS0);
    volatile struct TM_SET *tm_set=(volatile struct TM_SET *)(&can->CFDTMID0.UINT32);
    volatile union __tagDAT *tmdf_reg;

    unsigned int p_number=0;
    unsigned int buf_cnt=0;
    unsigned int tmdf_buf_cnt=0;
    unsigned int tx_buf_data_length_word_size_max = tx_buf_data_length_max>>2;

    //TX buffer is fixed number.
    //Evety CAN channel have 32 transmit buffer  (tx_buf_cnt_max=32)
    //TX buffer number = 32*m +31 (m : channel number)
    


    for(buf_cnt=0;buf_cnt<tx_buf_cnt_max;buf_cnt++)
    {
        p_number = buf_cnt+ (channel*32);
        tmdf_reg = (volatile union __tagDAT *)&tm_set[p_number].TMDF0.UINT32;

        tmsts_p[p_number] =0;//clear TMTRF
        tm_set[p_number].TMID.UINT32= p[buf_cnt].id.all;
        tm_set[p_number].TMPTR.UINT32   = p[buf_cnt].ptr.all;

        for(tmdf_buf_cnt=0;tmdf_buf_cnt<tx_buf_data_length_word_size_max;tmdf_buf_cnt++)
        tmdf_reg[tmdf_buf_cnt].UINT32 = p[buf_cnt].df[tmdf_buf_cnt].all;

        tm_set[p_number].TMFDCTR.UINT32 = p[buf_cnt].tmfdctr.all;

    }

    for(buf_cnt=0;buf_cnt<tx_buf_cnt_max;buf_cnt++)
    {   
        p_number = buf_cnt+ (channel*32);
        tmc_p[p_number] = p[buf_cnt].tmc.all;
    }
}

#ifdef CAN_BUF_LOOP_CHECK
unsigned long CAN_RX_cnt=0;
unsigned long rx_ack_flag=0;
void can_rx_buf_data_check(CAN_REG_TYP * can,
                             unsigned int q_number,
                             unsigned int *data,
                             unsigned int *dlc)
{
    // volatile struct __tag445 *can = ((volatile struct __tag445 *)((unsigned long)0xFFD00000));

    unsigned int rmnd_flg=0;
    unsigned long can_base = 0xFFD00000;
    //unsigned int delay=0;

    //q_number 0~63
    if(q_number<32)
    {
         rmnd_flg = (can->CFDRMND0.UINT32 >>q_number)&0x1;
    }
    else if(q_number<64)
    {
        rmnd_flg = (can->CFDRMND1.UINT32 >>(q_number-32))&0x1;
    }

    if(rmnd_flg==1)
    {
        *dlc = (PTR_UL_ADDR_RW(r_CAN_RMPTR(can_base,q_number))>>28)&0xF;

        data[0] = (PTR_UL_ADDR_RW(r_CAN_RDF0(can_base,q_number)))&0xFF;
        data[1] = (PTR_UL_ADDR_RW(r_CAN_RDF0(can_base,q_number))>>8)&0xFF;
        data[2] = (PTR_UL_ADDR_RW(r_CAN_RDF0(can_base,q_number))>>16)&0xFF;
        data[3] = (PTR_UL_ADDR_RW(r_CAN_RDF0(can_base,q_number))>>24)&0xFF;

        data[4] = (PTR_UL_ADDR_RW(r_CAN_RDF1(can_base,q_number)))&0xFF;
        data[5] = (PTR_UL_ADDR_RW(r_CAN_RDF1(can_base,q_number))>>8)&0xFF;
        data[6] = (PTR_UL_ADDR_RW(r_CAN_RDF1(can_base,q_number))>>16)&0xFF;
        data[7] = (PTR_UL_ADDR_RW(r_CAN_RDF1(can_base,q_number))>>24)&0xFF;


        if(q_number<32)
        {
            //clear flag
            can->CFDRMND0.UINT32 &= (~(0x1UL<<q_number));
        }
        else if(q_number<64)
        {
            //clear flag
            can->CFDRMND1.UINT32 &= (~(0x1UL<<(q_number-32)));
        }

        CAN_RX_cnt++;

        rx_ack_flag=1;
    }
}
#endif

#ifdef CAN_FIFO_BUF_CHECK
// unsigned int can_rx_fifo_cnt=0;

void can_rx_fifo_buf_int_check(CAN_REG_TYP * can,CAN_BUS_HANDLE *p, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number)
{
    volatile struct RFCC_SET *rfi= (volatile struct RFCC_SET *)(&can->CFDRFID0.UINT32);
    union __tagDAT *rfsts = (union __tagDAT *)(&can->CFDRFSTS0.UINT32);
    union __tagDAT *rfpctr = (union __tagDAT *)(&can->CFDRFPCTR0.UINT32);

    unsigned int rx_fifo_cnt=0;


    if(CAN_REG_READ(rfsts[rfi_number].UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_8)!=0)
    {
        can_fd_receive_fifo_buffer_decode(can,rfi_number);
        can_fd_receive_fifo_buffer_status(can,rfi_number);

        CAN_REG_CLR(rfsts[rfi_number].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF

        do
        {
            //if data full , wrap the counter to overwrite rx data buffer
            if(p->rx_data_cnt[rfi_number]>=CAN_RX_FIFO_DEPTH)
            {
                p->rx_data_cnt[rfi_number]=0;
                rx_fifo_cnt = p->rx_data_cnt[rfi_number];
            }
            else
            {
                rx_fifo_cnt = p->rx_data_cnt[rfi_number];
            }

            p->rx_data[rfi_number][rx_fifo_cnt].id.all = rfi[rfi_number].RFID.UINT32;
            p->rx_data[rfi_number][rx_fifo_cnt].dlc = (rfi[rfi_number].RFPTR.UINT8[3]>>4);
            p->rx_data[rfi_number][rx_fifo_cnt].data_u[0].data_dw =rfi[rfi_number].RFDF0.UINT32;
            p->rx_data[rfi_number][rx_fifo_cnt].data_u[1].data_dw =rfi[rfi_number].RFDF1.UINT32;
            p->rx_data_cnt[rfi_number]++;

            rfpctr[rfi_number].UINT8[0]=0xFF;
        }
        while (CAN_REG_READ(rfsts[rfi_number].UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_8)!=0);   //RFMC

        CAN_REG_CLR(rfsts[rfi_number].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);

        p->rx_data_ack.bytes |= (0x1<<rfi_number);
        can_bus_variable_ch1.can_rx_fifo_cnt++;
    }
}
#endif

// void can_rx_fifo_interrupt_flag_log(CAN_REG_TYP * can)
// {
//     union __tagDAT *rfsts = (union __tagDAT *)(&can->CFDRFSTS0.UINT32);
//     unsigned char rx_fifo_buffer = 0U;

//     /*********************/
//     rx_fifo_buffer = 0;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer0 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer0 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 1;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer1 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer1 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 2;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer2 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer2 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 3;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer3 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer3 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 4;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer4 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer4 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 5;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer5 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer5 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 6;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer6 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer6 = 0;
//         }
//     }
//     /*********************/
//     rx_fifo_buffer = 7;
//     if (can_bus_receive_fifo_interrupt_flag.RxBuffer7 > 0)
//     {
//         if ( (CAN_REG_READ(rfsts[rx_fifo_buffer].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)) == 0x00U)
//         {
//             tiny_printf("[rx fifo int:%d]\r\n",rx_fifo_buffer);
//         }
//         else
//         {
//             can_bus_receive_fifo_interrupt_flag.RxBuffer7 = 0;
//         }
//     }

// }

// void can_rx_fifo_interrupt_flag_check(CAN_REG_TYP * can)
// {
//     union __tagDAT *rfsts = (union __tagDAT *)(&can->CFDRFSTS0.UINT32);

//     if(CAN_REG_READ(rfsts[0].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer0 = 1U;
//         CAN_REG_CLR(rfsts[0].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[1].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer1 = 1U;
//         CAN_REG_CLR(rfsts[1].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[2].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer2 = 1U;
//         CAN_REG_CLR(rfsts[2].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[3].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer3 = 1U;
//         CAN_REG_CLR(rfsts[3].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[4].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer4 = 1U;
//         CAN_REG_CLR(rfsts[4].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[5].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer5 = 1U;
//         CAN_REG_CLR(rfsts[5].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[6].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer6 = 1U;
//         CAN_REG_CLR(rfsts[6].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }

//     if(CAN_REG_READ(rfsts[7].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1))
//     {
//         can_bus_receive_fifo_interrupt_flag.RxBuffer7 = 1U;
//         CAN_REG_CLR(rfsts[7].UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //RFIF
//     }
// }

static void can_handle_init(CAN_BUS_HANDLE *p)
{
    unsigned int init_cnt=0;
    unsigned int init_cnt_rfi=0;

    for(init_cnt=0;init_cnt<CAN_RX_FIFO_DEPTH;init_cnt++)
    {
        for(init_cnt_rfi=0;init_cnt_rfi<CAN_RX_FIFO_BUFFER_AMOUNT;init_cnt_rfi++)
        {
            p->rx_data[init_cnt_rfi][init_cnt].id.all=0;
            p->rx_data[init_cnt_rfi][init_cnt].data_u[0].data_dw =0x0;
            p->rx_data[init_cnt_rfi][init_cnt].data_u[1].data_dw =0x0;
            p->rx_data[init_cnt_rfi][init_cnt].dlc=0;
            p->rx_data_cnt[init_cnt_rfi]=0;
        }
    }
    
    for(init_cnt=0;init_cnt<CAN_RX_RULE_TABLE_AMOUNT;init_cnt++)
    {
        //rx rule table shadow register init.
        p->rrt_handle[init_cnt].id.all=0;

        //if rrt no used, must set mask as all one.
        p->rrt_handle[init_cnt].mask.bit.MID=0xFFF;
        // p->rrt_handle[init_cnt].mask.bit.MID=0x1FFFFFFF;
        p->rrt_handle[init_cnt].mask.bit.MIDE=0;
        p->rrt_handle[init_cnt].mask.bit.MRTR=0;

        p->rrt_handle[init_cnt].ptr0.all=0;
        p->rrt_handle[init_cnt].ptr1.all=0;   
    }
    
    for(init_cnt=0;init_cnt<CAN_TX_BUFFER_AMOUNT;init_cnt++)
    {
        //tx handle init.
        p->tx_handle[init_cnt].id.all =0;
        p->tx_handle[init_cnt].ptr.all=0;
        p->tx_handle[init_cnt].df[0].all=0;
        p->tx_handle[init_cnt].df[1].all=0;
        p->tx_handle[init_cnt].tmc.all=0;
        p->tx_handle[init_cnt].tmfdctr.all=0;
    }   

    p->rx_data_ack.bytes=0;

}

static void can_fifo_buffer_set(CAN_REG_TYP * can,
                            CAN_RX_FIFO_BUFER_NUMBER_e num,
                            CAN_RX_FIFO_BUFER_INT_EN_e enable_int,
                            CAN_RX_FIFO_BUFER_DEPTH_e depth,
							CAN_RX_FIFO_BUFER_PAYLOAD_e payload,
                            CAN_RX_FIFO_BUFER_INT_TIMING_e int_timing)
{
    volatile union __tagDAT *rfcc = (union __tagDAT *)(&can->CFDRFCC0);

    /*
        Receive FIFO Buffer Payload Storage Size Select 
        b6     b5     b4 
        0      0      0: 8 bytes 
        0      0      1: 12 bytes 
        0      1      0: 16 bytes 
        0      1      1: 20 bytes 
        1      0      0: 24 bytes 
        1      0      1: 32 bytes 
        1      1      0: 48 bytes 
        1      1      1: 64 bytes     
    */    
    CAN_REG_CLR(rfcc[num].UINT32,CAN_REG_BIT4,CAN_REG_LENGTH_3);    
    CAN_REG_SET(rfcc[num].UINT32,CAN_REG_BIT4,payload);  //RFPLS  (if DLC>8  this value must greater than 0 )

    /*
        Receive FIFO Buffer Depth Configuration 
        b10    b9    b8 
        0      0      0: 0 messages 
        0      0      1: 4 messages 
        0      1      0: 8 messages 
        0      1      1: 16 messages 
        1      0      0: 32 messages 
        1      0      1: 48 messages 
        1      1      0: 64 messages 
        1      1      1: 128 messages      
    */    
    CAN_REG_CLR(rfcc[num].UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_3);    
    CAN_REG_SET(rfcc[num].UINT32,CAN_REG_BIT8,depth);  //RFDC


    /*
        Receive FIFO Interrupt Request Timing Select 
        b15    b14    b13 
        0      0      0: When FIFO is 1/8 full. 
        0      0      1: When FIFO is 2/8 full. 
        0      1      0: When FIFO is 3/8 full. 
        0      1      1: When FIFO is 4/8 full. 
        1      0      0: When FIFO is 5/8 full. 
        1      0      1: When FIFO is 6/8 full. 
        1      1      0: When FIFO is 7/8 full. 
        1      1      1: When FIFO is full.      
    */    
    //Modify these bits only in global reset mode
    CAN_REG_CLR(rfcc[num].UINT32,CAN_REG_BIT13,CAN_REG_LENGTH_3);
    CAN_REG_SET(rfcc[num].UINT32,CAN_REG_BIT13,int_timing); //RFIGCV
    

    /*
        Receive FIFO Interrupt Enable 
        0: Receive FIFO interrupt is disabled. 
        1: Receive FIFO interrupt is enabled.   
    */    
    //Modify this bit when the RFE bit set to 0
    CAN_REG_CLR(rfcc[num].UINT32,CAN_REG_BIT1,CAN_REG_LENGTH_1);
    CAN_REG_SET(rfcc[num].UINT32,CAN_REG_BIT1,enable_int); //RFIE

    /*
        Receive FIFO Interrupt Source Select 
        0: An interrupt occurs when the condition set by the RFIGCV[2:0] bits is met. 
        1: An interrupt occurs each time a message has been received.     
    */    
    CAN_REG_CLR(rfcc[num].UINT32,CAN_REG_BIT12,CAN_REG_LENGTH_1);
    CAN_REG_SET(rfcc[num].UINT32,CAN_REG_BIT12,CAN_REG_LENGTH_1); //RFIM
}

static void can_fifo_buffer_enable(CAN_REG_TYP * can,CAN_RX_FIFO_BUFER_NUMBER_e num)
{
    volatile union __tagDAT *rfcc = (union __tagDAT *)(&can->CFDRFCC0);
    
    //Modify this bit in global operating mode or global test mode.
    CAN_REG_CLR(rfcc[num].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1);
 
    /*
        Receive FIFO Buffer Enable 
        0: No receive FIFO buffer is used. 
        1: Receive FIFO buffers are used.     
    */    
    CAN_REG_SET(rfcc[num].UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1); //RFE   
}




static void can_init_rx_rule_table_page_setting(CAN_REG_TYP * can,
                                                CAN_RRT_PAGE_SET_e page_sel,
                                                CAN_BUS_RX_INFO_HANDLE *ptr,
                                                unsigned char rule_count)
{
    /*
        Receive rule ID register            RCFDCnCFDGAFLIDj    <RCFDCn_base> + 1000 H + (10 H × j) 
        Receive rule mask register          RCFDCnCFDGAFLMj     <RCFDCn_base> + 1004 H + (10 H × j) 
        Receive rule pointer 0 register     RCFDCnCFDGAFLP0_j   <RCFDCn_base> + 1008 H + (10 H × j)
        Receive rule pointer 1 register     RCFDCnCFDGAFLP1_j   <RCFDCn_base> + 100C H + (10 H × j)  
    */

    volatile struct RRT_PAGE_SET * rrt_page = (volatile struct RRT_PAGE_SET *)(&can->CFDGAFLID0.UINT32);
    unsigned int j=0;
       
    if(page_sel>=CAN_RX_RULE_TABLE_PAGE63)
    {
        page_sel=CAN_RX_RULE_TABLE_PAGE63;
    }

    //select page
    CAN_REG_CLR(can->CFDGAFLECTR.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_6); 
    CAN_REG_SET(can->CFDGAFLECTR.UINT32,CAN_REG_BIT0,page_sel);//Sixteen receive rules can be set per page.     
    //can->GAFLECTR.BIT.AFLPN =(unsigned char)page_sel;

    /*
        [accept ANY standard ID , data frame]
        CFDGAFLID  = 0x00000000;

        CFDGAFLM   = 0xC0000000;
        CFDGAFLM   = (1UL << 31) | (1UL << 30);  // compare IDE=0, RTR=0
        
        CFDGAFLP0_ = (1UL << 15) | (0 << 8);
        CFDGAFLP1_ = 0;

        [comapre MATCH ID]
        CFDGAFLID  = 0x00000111;

        CFDGAFLM   = 0xDFFFFFFF;    // compare standard ID (bit[28:0])、IDE (bit 31)、RTR (bit 30)
        CFDGAFLM   = (1UL << 31) | (1UL << 30) | 0x1FFFFFFF;

        CFDGAFLP0_ = (1UL << 15) | (0 << 8);
        CFDGAFLP1_ = 0;    

        [accept extened ID]
        CFDGAFLID  = (1UL << 31);
    */

    for(j=0;j<rule_count;j++)
    {
        CAN_REG_SET(rrt_page[j].CFDGAFLID ,CAN_REG_BIT0,ptr[j].id.all);    
        // CAN_REG_SET(rrt_page[j].CFDGAFLID ,CAN_REG_BIT0,0x00000000);   
        // CAN_REG_SET(rrt_page[j].CFDGAFLID ,CAN_REG_BIT0,(1UL << 31));   
        
        CAN_REG_SET(rrt_page[j].CFDGAFLM ,CAN_REG_BIT0,ptr[j].mask.all);
        // CAN_REG_SET(rrt_page[j].CFDGAFLM ,CAN_REG_BIT0,(1UL << 31) | (1UL << 30));  //compare IDE,RTR
        // CAN_REG_SET(rrt_page[j].CFDGAFLM ,CAN_REG_BIT0,0x00000000);// not compare          
        // CAN_REG_CLR(rrt_page[j].CFDGAFLM ,CAN_REG_BIT0,0x1FFFFFFF);// compare all
     
        CAN_REG_SET(rrt_page[j].CFDGAFLP0_ ,CAN_REG_BIT0,ptr[j].ptr0.all);
        // CAN_REG_SET(rrt_page[j].CFDGAFLP0_ ,CAN_REG_BIT0,(1UL << 15) | (0UL << 8));        
        // CAN_REG_CLR(rrt_page[j].CFDGAFLP0_ ,CAN_REG_BIT0,CAN_REG_LENGTH_4);

        CAN_REG_SET(rrt_page[j].CFDGAFLP1_ ,CAN_REG_BIT0,ptr[j].ptr1.all);
        // CAN_REG_SET(rrt_page[j].CFDGAFLP1_ ,CAN_REG_BIT0,0x00000001);
        
        // CAN_REG_SET(rrt_page[j].CFDGAFLP1_ ,CAN_REG_BIT0,CAN_REG_LENGTH_8);
        // CAN_REG_CLR(rrt_page[j].CFDGAFLP1_ ,CAN_REG_BIT0,CAN_REG_LENGTH_8);
    }
    
}

void can_receive_rule_setting(CAN_REG_TYP * can, CAN_CHANNEL_SEL_e ch, unsigned char rule_count)
{
    //channel xx : set number of rules
    //1. The maximum number of rules per channel is 255.
    //2. The total of the number of rules allocated to each channel is not larger than the number of 
    //   rules that can be registered in the entire unit.(total :128*3 receive rules)
    
    /*
        RCFDCnCFDGAFLCFG0 : Number of Rules for Channel 0  , ... , Number of Rules for Channel 3
        RCFDCnCFDGAFLCFG1 : Number of Rules for Channel 4  , ... , Number of Rules for Channel 7

    */
    #if 1
    const unsigned char bit_shift_tbl[4] = 
    {
        CAN_REG_BIT24, CAN_REG_BIT16, CAN_REG_BIT8, CAN_REG_BIT0
    };
    unsigned char reg_sel  = ch >> 2; // 0 : ch 0 ~ 3 , 1 : ch 4 ~ 7
    unsigned char bit_shift = bit_shift_tbl[ch & 0x03];

    if (reg_sel)    // ch 4 ~ 7
    {
        CAN_REG_SET(can->CFDGAFLCFG1.UINT32,bit_shift,rule_count);    //page 0    for Channel 4~7   q =0~15
        tiny_printf("RCFDCnCFDGAFLCFG1:0x%02X-0x%04X\r\n", ch,bit_shift);
    }
    else    // ch 0 ~ 3
    {
        CAN_REG_SET(can->CFDGAFLCFG0.UINT32,bit_shift,rule_count);    //page 0    for Channel 0~3   q =0~15
        tiny_printf("RCFDCnCFDGAFLCFG0:0x%02X-0x%04X\r\n", ch,bit_shift);
    }
    #else
    unsigned char ch_bit_shift = 0;
    unsigned char receive_rule_config_register = 0;

    switch(ch)
    {
        case CAN_CHANNEL_0:
            ch_bit_shift = CAN_REG_BIT24;
            receive_rule_config_register = 0;
            break;
        case CAN_CHANNEL_1:
            ch_bit_shift = CAN_REG_BIT16;
            receive_rule_config_register = 0;
            break;
        case CAN_CHANNEL_2:
            ch_bit_shift = CAN_REG_BIT8;
            receive_rule_config_register = 0;
            break;
        case CAN_CHANNEL_3:
            ch_bit_shift = CAN_REG_BIT0;
            receive_rule_config_register = 0;
            break;


        case CAN_CHANNEL_4:
            ch_bit_shift = CAN_REG_BIT24;
            receive_rule_config_register = 1;
            break;
        case CAN_CHANNEL_5:
            ch_bit_shift = CAN_REG_BIT16;
            receive_rule_config_register = 1;
            break;
        // case CAN_CHANNEL_6:
        //     ch_bit_shift = CAN_REG_BIT8;
        //     receive_rule_config_register = 1;
        //     break;
        // case CAN_CHANNEL_7:
        //     ch_bit_shift = CAN_REG_BIT0;
        //     receive_rule_config_register = 1;
        //     break;

    }

    if (receive_rule_config_register == 0)
    {
        CAN_REG_SET(can->CFDGAFLCFG0.UINT32,ch_bit_shift,rule_count);    //page 0    for Channel 0~3   q =0~15
    }
    else
    {
        CAN_REG_SET(can->CFDGAFLCFG1.UINT32,ch_bit_shift,rule_count);    //page 0    for Channel 4~7   q =0~15
    }
    #endif
}

static void can2_rrt_set(CAN_REG_TYP * can,
                        CAN_BUS_HANDLE *p,
                        const CAN_RX_RULE_TABLE_T *rule,
                        unsigned char rule_count)
{
    unsigned int q=0;

    for(q=0;q<rule_count;q++)
    {
        //RCFDCnCFDGAFLIDjcan2_rrt_set
        p->rrt_handle[q].id.bit.ID=q;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=0;
        p->rrt_handle[q].id.bit.RTR=0;
        
        //RCFDCnCFDGAFLMj
        #if defined (CAN_USE_RX_RULE)
        p->rrt_handle[q].mask.bit.MID=STANDARD_ID_BIT_IS_COMPARED;
        // p->rrt_handle[q].mask.bit.MID=EXTEND_ID_BIT_IS_COMPARED;//The corresponding ID bit is compared
        #else
        p->rrt_handle[q].mask.bit.MID=ALL_ID_BIT_IS_NOT_COMPARED;//The corresponding ID bit is not compared
        #endif
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        #if 1
        p->rrt_handle[q].ptr0.bit.RMDP=(rule + q)->GAFLRMDP;    //Set the receive buffer number to store received message.
        #else
        p->rrt_handle[q].ptr0.bit.RMDP=can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM;    //Set the receive buffer number to store received message.
        #endif
        // tiny_printf("can2_rrt_set-RMDP(%d):0x%08X\r\n", q , p->rrt_handle[q].ptr0.bit.RMDP);
        // p->rrt_handle[q].ptr0.bit.RMV=0;     //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.RMV=1;
        p->rrt_handle[q].ptr0.bit.PTR=0;     //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC=(unsigned char)CAN_DLC_DISABLE_CHECK;

        //RCFDCnCFDGAFLP1_j
        //disable FIFO
        #if 1
        p->rrt_handle[q].ptr1.bit.FDPx=(rule + q)->GAFLFDP_x;   //Receive FIFO Buffer xx Select
        #else
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM);        //Receive FIFO Buffer x Select
        // p->rrt_handle[q].ptr1.bit.FDPx=0x1U;   //Receive FIFO Buffer 0 Select
        #endif
        // tiny_printf("can2_rrt_set-FDPx(%d):0x%08X\r\n", q , p->rrt_handle[q].ptr1.bit.FDPx);
        
        p->rrt_handle[q].ptr1.bit.FDPk=0;       //Transmit/Receive FIFO Buffer k Select
    }

    #if defined (CAN_USE_RX_RULE)
    //Channel 0 Setting:
    for(q=0;q<rule_count;q++)
    {
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID = (rule + q)->GAFLID;  //rule[q].GAFLID;
        p->rrt_handle[q].id.bit.LB = (rule + q)->GAFLLB;
        p->rrt_handle[q].id.bit.IDE = (rule + q)->GAFLIDE;
        p->rrt_handle[q].id.bit.RTR = (rule + q)->GAFLRTR;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID = (rule + q)->GAFLIDM;
        p->rrt_handle[q].mask.bit.MIDE = (rule + q)->GAFLIDEM;
        p->rrt_handle[q].mask.bit.MRTR = (rule + q)->GAFLRTRM;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP = (rule + q)->GAFLRMDP;        //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV = (rule + q)->GAFLRMV;          //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR = (rule + q)->GAFLPTR;          //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC = (rule + q)->GAFLDLC;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx = (rule + q)->GAFLFDP_x;     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk = (rule + q)->GAFLFDP_k;                                    //Transmit/Receive FIFO Buffer disable
    }
    #endif

    can_receive_rule_setting(can,(CAN_CHANNEL_SEL_e)can_bus_parameter_ch4.CAN_CH,rule_count);

    //enable rrt setting
    CAN_REG_SET(can->CFDGAFLECTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);     //AFLDAE
    //can->GAFLECTR.BIT.AFLDAE =1;
    
    can_init_rx_rule_table_page_setting(can,
                                        CAN_RX_RULE_TABLE_PAGE1,
                                        &p->rrt_handle[0],
                                        rule_count);

    //disable rrt setting
    CAN_REG_CLR(can->CFDGAFLECTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);     //AFLDAE
    //can->GAFLECTR.BIT.AFLDAE =0;

    /* Set Rx buffer number*/
    //maximum number of RX buffer is 128    
    // CAN_REG_SET(can->CFDRMNB.UINT32,CAN_REG_BIT0,rule_count);     //NRXMB
}

static void can_rrt_set(CAN_REG_TYP * can,
                        CAN_BUS_HANDLE *p,
                        const CAN_RX_RULE_TABLE_T *rule,
                        unsigned char rule_count)
{
    unsigned int q=0;

    for(q=0;q<rule_count;q++)
    {
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=q;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=0;
        p->rrt_handle[q].id.bit.RTR=0;
        
        //RCFDCnCFDGAFLMj
        #if defined (CAN_USE_RX_RULE)
        p->rrt_handle[q].mask.bit.MID=STANDARD_ID_BIT_IS_COMPARED;
        // p->rrt_handle[q].mask.bit.MID=EXTEND_ID_BIT_IS_COMPARED;//The corresponding ID bit is compared
        #else
        p->rrt_handle[q].mask.bit.MID=ALL_ID_BIT_IS_NOT_COMPARED;//The corresponding ID bit is not compared
        #endif
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        #if 1
        p->rrt_handle[q].ptr0.bit.RMDP=(rule + q)->GAFLRMDP;    //Set the receive buffer number to store received message.
        #else
        p->rrt_handle[q].ptr0.bit.RMDP=can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM;    //Set the receive buffer number to store received message.
        #endif
        // tiny_printf("can_rrt_set-RMDP(%d):0x%08X\r\n", q , p->rrt_handle[q].ptr0.bit.RMDP);
        // p->rrt_handle[q].ptr0.bit.RMV=0;     //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.RMV=1;
        p->rrt_handle[q].ptr0.bit.PTR=0;     //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC=(unsigned char)CAN_DLC_DISABLE_CHECK;

        //RCFDCnCFDGAFLP1_j
        //disable FIFO
        #if 1
        p->rrt_handle[q].ptr1.bit.FDPx=(rule + q)->GAFLFDP_x;   //Receive FIFO Buffer xx Select
        #else
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);        //Receive FIFO Buffer x Select
        // p->rrt_handle[q].ptr1.bit.FDPx=0x1U;   //Receive FIFO Buffer 0 Select
        #endif
        // tiny_printf("can_rrt_set-FDPx(%d):0x%08X\r\n", q , p->rrt_handle[q].ptr1.bit.FDPx);
        
        p->rrt_handle[q].ptr1.bit.FDPk=0;       //Transmit/Receive FIFO Buffer k Select
    }

    #if defined (CAN_USE_RX_RULE)
    //Channel 0 Setting:
    #if 1
    for(q=0;q<rule_count;q++)
    {
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID = (rule + q)->GAFLID;  //rule[q].GAFLID;
        p->rrt_handle[q].id.bit.LB = (rule + q)->GAFLLB;
        p->rrt_handle[q].id.bit.IDE = (rule + q)->GAFLIDE;
        p->rrt_handle[q].id.bit.RTR = (rule + q)->GAFLRTR;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID = (rule + q)->GAFLIDM;
        p->rrt_handle[q].mask.bit.MIDE = (rule + q)->GAFLIDEM;
        p->rrt_handle[q].mask.bit.MRTR = (rule + q)->GAFLRTRM;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP = (rule + q)->GAFLRMDP;        //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV = (rule + q)->GAFLRMV;          //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR = (rule + q)->GAFLPTR;          //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC = (rule + q)->GAFLDLC;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx = (rule + q)->GAFLFDP_x;     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk = (rule + q)->GAFLFDP_k;                                    //Transmit/Receive FIFO Buffer disable
    }
    #else
    //Setting for page 0(0~15) 
    {
        q=0;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT0_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    
    {
        q=1;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT1_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }    
    {
        q=2;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT2_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    
    {
        q=3;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT3_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    {
        q=4;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT4_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    
    {
        q=5;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT5_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_STANDARD_ID;//RX_EXTEND_ID
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=1;
        p->rrt_handle[q].mask.bit.MRTR=1;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=CAN_RX_FIFO_BUFFER_NUMBER0;           //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=1;            //0: No receive buffer is used.,1: A receive buffer is used. 
        p->rrt_handle[q].ptr0.bit.PTR=0x5555;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<CAN_RX_FIFO_BUFFER_NUMBER0);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    {
        q=6;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT6_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    
    {
        q=7;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT7_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }  
    {
        q=8;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT8_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    }
    {
        q=9;
        //RCFDCnCFDGAFLIDj
        p->rrt_handle[q].id.bit.ID=RX_RRT9_ID;
        p->rrt_handle[q].id.bit.LB=0;
        p->rrt_handle[q].id.bit.IDE=(unsigned char)RX_EXTEND_ID;
        p->rrt_handle[q].id.bit.RTR=(unsigned char)RX_DATA_FRAME;
        
        //RCFDCnCFDGAFLMj
        p->rrt_handle[q].mask.bit.MID=0xFFF;
        p->rrt_handle[q].mask.bit.MIDE=0;
        p->rrt_handle[q].mask.bit.MRTR=0;

        //RCFDCnCFDGAFLP0_j
        p->rrt_handle[q].ptr0.bit.RMDP=0;      //Set the receive buffer number to store received message.
        p->rrt_handle[q].ptr0.bit.RMV=0;       //Receive Buffer disable
        p->rrt_handle[q].ptr0.bit.PTR=0;       //Receive Rule Label (additional infomation 12bit)
        p->rrt_handle[q].ptr0.bit.DLC= (unsigned char)CAN_DLC_DISABLE_CHECK;
        
        //RCFDCnCFDGAFLP1_j
        //enable RX FIFO
        p->rrt_handle[q].ptr1.bit.FDPx=(0x1<<can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);     //Receive FIFO Buffer 0 Selected(select by bit filed)  / one FIFO buffer depth 128 message
        p->rrt_handle[q].ptr1.bit.FDPk=0;                                     //Transmit/Receive FIFO Buffer disable
    } 
    #endif   
    #endif

    can_receive_rule_setting(can,(CAN_CHANNEL_SEL_e)can_bus_parameter_ch1.CAN_CH,rule_count);

    //enable rrt setting
    CAN_REG_SET(can->CFDGAFLECTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);     //AFLDAE
    //can->GAFLECTR.BIT.AFLDAE =1;
    
    can_init_rx_rule_table_page_setting(can,
                                        CAN_RX_RULE_TABLE_PAGE0,
                                        &p->rrt_handle[0],
                                        rule_count);

    //disable rrt setting
    CAN_REG_CLR(can->CFDGAFLECTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);     //AFLDAE
    //can->GAFLECTR.BIT.AFLDAE =0;

    /* Set Rx buffer number*/
    //maximum number of RX buffer is 128    
    // CAN_REG_SET(can->CFDRMNB.UINT32,CAN_REG_BIT0,rule_count);     //NRXMB
}

// static void can_tx_normal_buffer_set(CAN_BUS_HANDLE *p,CAN_FD_MODE_e mode)
// {
//     unsigned int p_number=0;
//     unsigned int buf_cnt=0;
//     unsigned short i=0;
//     unsigned short dlc=0;
//     //TX buffer is fixed number.
//     //Evety CAN channel have 32 transmit buffer
//     //TX buffer number = 32*m +31 (m : channel number)

//     //Channel 0 : TX buffer Number 0~31
//     for(buf_cnt=0;buf_cnt<16;buf_cnt++)
//     {
//         p_number = buf_cnt;
//         p->tx_handle[p_number].id.bit.tmid =(unsigned long)(0x200+p_number);
    
//         p->tx_handle[p_number].id.bit.hlen =(unsigned char)TX_DATA_HISTORY_NOT_STORE;
//         p->tx_handle[p_number].id.bit.rtr =(unsigned char)TX_DATA_FRAME;
//         p->tx_handle[p_number].id.bit.ide =(unsigned char)TX_STANDARD_ID;

//         p->tx_handle[p_number].ptr.bit.dlc=(unsigned int)TX_DATA_LENGTH_1;
//         p->tx_handle[p_number].df[0].byte.TMDB[0] = 0x10+p_number;
//         p->tx_handle[p_number].tmc.bit.tmom=0;
//         p->tx_handle[p_number].tmc.bit.tmtar=0;

//         if(mode == CAN_FD_MIX_MODE || mode == CAN_FD_ONLY_MODE)
//         {
//         	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CAN_FD_FRAME;
//         	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_CHANGE;
//         	p->tx_handle[p_number].tmfdctr.bit.tmesi = 1;
//         }
//         else
//         {
//         	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CLASSIC_CAN_FRAME;
//         	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_NO_CHANGE;
//         	p->tx_handle[p_number].tmfdctr.bit.tmesi = 0;
//         }
//         //always set 1
//         p->tx_handle[p_number].tmc.bit.tmtr=1;
//     }

//     //data buffer init.
//     p_number=0;
//     dlc = (unsigned short)CAN_DLC_64BYTE;
//     p->tx_handle[p_number].ptr.bit.dlc=dlc;

//     for(i=0;i<(dlc+1);i++)
//     {
//         p->tx_handle[p_number].df[i].byte.TMDB[0] = 0x10+i+1;
//         p->tx_handle[p_number].df[i].byte.TMDB[1] = 0x10+i+2;
//         p->tx_handle[p_number].df[i].byte.TMDB[2] = 0x10+i+3;
//         p->tx_handle[p_number].df[i].byte.TMDB[3] = 0x10+i+4;
//     }

// }

static void can_start(CAN_REG_TYP * can,CAN_CHANNEL_SEL_e channel)
{
    static unsigned short can_global_mode_start_once_flag=0;
    volatile struct CHANNEL_SET *cst= (volatile struct CHANNEL_SET *)(&can->CFDC0NCFG.UINT32);
    //volatile struct CHANNEL_SET_2 *cst2= (volatile struct CHANNEL_SET_2 *)(&can->CFDC0DCFG.UINT32);

    if(can_global_mode_start_once_flag!=0x55AA)
    {
        //Transition to global operating mode
        CAN_REG_CLR(can->CFDGCTR.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_2);    //GMDC

        while(CAN_REG_READ(can->CFDGSTS.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)!=0);  //GRSTSTS

        can_global_mode_start_once_flag=0x55AA;
    }
    

    // CAN_REG_SET(cst[channel].CxCTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1); //BEIE
    // CAN_REG_SET(cst[channel].CxCTR.UINT32,CAN_REG_BIT11,CAN_REG_LENGTH_1); //BOEIE
    // CAN_REG_SET(cst[channel].CxCTR.UINT32,CAN_REG_BIT12,CAN_REG_LENGTH_1); //BORIE

    //Transition to channel communication mode
    // TODO : 
    #if 1
    CAN_REG_CLR(cst[channel].CxCTR.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_2); //CHMDC
    while(CAN_REG_READ(cst[channel].CxSTS.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)!=0);  //CRSTSTS
    #else
    CAN_REG_CLR(cst[can_bus_parameter_ch1.CAN_CH].CxCTR.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_2); //CHMDC
    while(CAN_REG_READ(cst[can_bus_parameter_ch1.CAN_CH].CxSTS.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)!=0);  //CRSTSTS
    
    CAN_REG_CLR(cst[can_bus_parameter_ch4.CAN_CH].CxCTR.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_2); //CHMDC
    while(CAN_REG_READ(cst[can_bus_parameter_ch4.CAN_CH].CxSTS.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1)!=0);  //CRSTSTS
    #endif
}


static void can_syncp(CAN_REG_TYP * can)
{
    volatile unsigned long can_syncp_dummy_read;

    //Dummy reading of any register of the same peripheral group can be used instead.
    can_syncp_dummy_read = can->CFDC0CTR.UINT32;
    __syncp(); /* Peripheral synchronize  */
}


static void can_normal_mode_set(CAN_REG_TYP * can,
								 CAN_FD_MODE_e can_mode,
                                 CAN_CHANNEL_SEL_e channel,
                                 CAN_BIT_TIME_DSJW_SEL_e dsjw,
                                 CAN_BIT_TIME_DTSEG2_SEL_e dtseg2,
                                 CAN_BIT_TIME_DTSEG1_SEL_e dtseg1,
                                 CAN_BIT_TIME_DBRP_SEL_e dbrp,
                                 CAN_BIT_TIME_NSJW_SEL_e nsjw,
                                 CAN_BIT_TIME_NTSEG2_SEL_e ntseg2,
                                 CAN_BIT_TIME_NTSEG1_SEL_e ntseg1,
                                 CAN_BIT_TIME_NBRP_SEL_e nbrp
                                 )
                         
{
    static unsigned short can_global_mode_init_flag;
    volatile struct CHANNEL_SET *cst= (volatile struct CHANNEL_SET *)(&can->CFDC0NCFG.UINT32);
    volatile struct CHANNEL_SET_2 *cst2= (volatile struct CHANNEL_SET_2 *)(&can->CFDC0DCFG.UINT32);
    // volatile unsigned short can_bus_mode=0;
    //page 2145
    
    //multi channel init. will excute global init. at first time.
    if(can_global_mode_init_flag!=0x55AA)
    {
        //Global mode switch
        
        while(CAN_REG_READ(can->CFDGSTS.UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1)!=0);  //GRAMINIT

        // Transition from global stop mode to global reset mode 
        // (Set GSLPR in the RCFDCnCFDGCTR register to 0)
        CAN_REG_CLR(can->CFDGCTR.UINT32,CAN_REG_BIT2,CAN_REG_LENGTH_1);    //from global stop mode to global reset mode

        while(CAN_REG_READ(can->CFDGSTS.UINT32,CAN_REG_BIT2,CAN_REG_LENGTH_1)!=0);  //GSLPSTS

        // R_CANFD_Global_error_Interrupt_Init();

        //===========only for F1KM-S1 R7F701587x Kit init.===================
        //CAN_REG_SET(can->CFDGRMCFG.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1);   //set as FD mode  
        //===================================================================
        
        CAN_REG_SET(can->CFDGCFG.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1);   //TPRI Transmit buffer Number priority
        CAN_REG_CLR(can->CFDGCFG.UINT32,CAN_REG_BIT1,CAN_REG_LENGTH_1);   //DCE DLC check is disabled.
        CAN_REG_CLR(can->CFDGCFG.UINT32,CAN_REG_BIT2,CAN_REG_LENGTH_1);   //DRE DLC replacement is disabled.
        CAN_REG_CLR(can->CFDGCFG.UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);   //MME Mirror function is disabled.
        //can->GCFG.BIT.EEFE=0;   //The ECC error flag is disabled.
        //can->GCFG.BIT.TMTSCE=0; //Transmission timestamp is disabled.

        CAN_REG_SET(can->CFDGCFG.UINT32,CAN_REG_BIT4,CAN_DCS_CLKC);    //DCS CAN Clock Source Select 

        can_global_mode_init_flag =0x55AA;
    }
            
    CAN_REG_CLR(cst[channel].CxCTR.UINT32,CAN_REG_BIT2,CAN_REG_LENGTH_1);//CSLPR from channel stop mode to channel reset mode
    while(CAN_REG_READ(cst[channel].CxSTS.UINT32,CAN_REG_BIT2,CAN_REG_LENGTH_1)!=0);//CSLPSTS

    CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT29,CAN_REG_LENGTH_1); //REFE

    switch(can_mode)
    {
    case CAN_FD_MIX_MODE:
        CAN_REG_CLR(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT30,CAN_REG_LENGTH_1); //CLOE =0
        CAN_REG_CLR(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT28,CAN_REG_LENGTH_1); //FDOE =0
    	break;
    case CAN_FD_ONLY_MODE:
        CAN_REG_CLR(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT30,CAN_REG_LENGTH_1); //CLOE =0
        CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT28,CAN_REG_LENGTH_1); //FDOE =1
    	break;
    case CAN_STANDARD_MODE:
        CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT30,CAN_REG_LENGTH_1); //CLOE =1
        CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT28,CAN_REG_LENGTH_1); //FDOE =1
    	break;
    default:
    	break;
    }

    #if defined (ENABLE_TDC)  // Transmitter Delay Compensation Enable
    CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT9,CAN_REG_LENGTH_1);    // TDCE = 1
    CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);    // TDCOC = 1
    //The SSP offset value = (set value of TDCO[6:0] bits + 1). 
    CAN_REG_SET(cst2[channel].CxFDCFG.UINT32,CAN_REG_BIT16,6);   // TDCO[6:0] 
    #endif

    CAN_REG_SET(cst[channel].CxNCFG.UINT32,CAN_REG_BIT24,(unsigned short)ntseg2);
    CAN_REG_SET(cst[channel].CxNCFG.UINT32,CAN_REG_BIT16,(unsigned short)ntseg1);
    CAN_REG_SET(cst[channel].CxNCFG.UINT32,CAN_REG_BIT11,(unsigned short)nsjw);
    CAN_REG_SET(cst[channel].CxNCFG.UINT32,CAN_REG_BIT0,(unsigned short)nbrp);     //value 0~1023
    

    CAN_REG_SET(cst2[channel].CxDCFG.UINT32,CAN_REG_BIT24,(unsigned short)dsjw);
    CAN_REG_SET(cst2[channel].CxDCFG.UINT32,CAN_REG_BIT20,(unsigned short)dtseg2);
    CAN_REG_SET(cst2[channel].CxDCFG.UINT32,CAN_REG_BIT16,(unsigned short)dtseg1);
    CAN_REG_SET(cst2[channel].CxDCFG.UINT32,CAN_REG_BIT0,(unsigned short)dbrp); //value 0~255

//         clkc       ==> CKSCLK_IPERI2   ==> C_ISO_PERI2     40Mhz
//         clk_xincan ==> CKSCLK_ICANOSC  ==> C_ISO_CANOSC    disable
//
//         DCS=0    fCAN =clkc
//         DCS=1    fCAN =clk_xincan
//
//                              fCAN
//     NBRP   =( ----------------------------------------) -1
//             Norminal bit rate  x(TQ COUNT of 1 BIT TIME)
//                              fCAN
//     DBRP  =( ----------------------------------------) -1
//              Data bit rate  x(TQ COUNT of 1 BIT TIME)

//  Norminal bit rate = Data bit rate  =500 Khz
//
//  TQ COUNT RANGE(SS+ TSEG1 +TSEG2) = 8TQ ~25TQ
//  TSEG1 > TSEG2 >=SJW
//
//   TQ COUNT = 20
//   SS =1 , TSEG1=14 ,TSEG2 = 5 , SJW =1  , Sample Point = 75%
//
//   BRP =4  , P =BRP-1 =3

    //clkc=PPLLCLK2=40M
    /*Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/1/80 = 0.5Mbps(500Kbps) */
    //NTS2=20; NTS1=59; NSJW=20; NBRP=0 , SamplePoint": 75
    
    //Bitrate = fCAN/(BRP+1)/(1+TSEG1+TSEG2) = 40M/1/8 = 5Mbps
    //DSJW=2; DTS2=2; DTS1=5; DBRP=0 , SamplePoint": 75

/*
    check RCFDCnCFDCmNCFG: 

    Classical CAN only mode :   
    ●  Maximum 1 Mbps 

    Communication speed (CANm bit time clock) =   1  / CANm bit time 

    CANm bit time = CANmTq × Tq count per bit 

    CANmTq =   (NBRP[9: 0] bits in the RCFDCnCFDCmNCFG register + 1)  / fCAN 

    fCAN: Frequency of CAN clock (selected by the DCS bit in the RCFDCnCFDGCFG register) 

    CAN FD mode and CAN FD only mode: 

    ●  Data bit rate: max.5 Mbps ( Nominal bit rate ≤ 500 Kbps )  

    ●  Data bit rate: max.2 Mbps ( 1Mbps ≥ Nominal bit rate > 500 Kbps )   

    Note :  fCAN = 40 MHz 

    Transmission rate (CANm nominal bit time clock) = 1 / CANm nominal bit time 

    Transmission rate (CANm data bit time clock) =   1 / CANm data bit time 

    CANm nominal bit time = CANmTq(N) × Tq count per nominal bit 

    CANm data bit time = CANmTq(D) × Tq count per data bit 

    CANmTq(N) =   (NBRP[9: 0] bits in the RCFDCnCFDCmNCFG register + 1) /fCAN 

    CANmTq(D) =   (DBRP[7: 0] bits in the RCFDCnCFDCmDCFG register + 1) /fCAN 

    fCAN: Frequency of CAN clock (selected by the DCS bit in the RCFDCnCFDGCFG register) 

    m = 0 to 7 (n = 0), m = 0 to 3 with (n = 1) 

    Tq: Time quantum 

*/

}


void can_rx_interrupt_cbk(void)
{    
	can_bus_variable_ch1.can_rx_isr_cnt++;

    can_rx_fifo_buf_int_check(&RCFDC0,&can_bus_handle_ch1,can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);
    #if defined (ENABLE_MULTI_CAN_CH)
    can_rx_fifo_buf_int_check(&RCFDC0,&can_bus_handle_ch4,can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM);
    #endif

    // can_rx_fifo_interrupt_flag_check(&RCFDC0);    
    // can_rx_fifo_interrupt_flag_log(&RCFDC0);

    can_bus_variable_ch1.can_rx_flag = 1;
}

// void can_run(void)
// {
// 	if(can_bus_variable_ch1.can_tx1_flag==1)
// 	{
// 		can_bus_variable_ch1.can_tx1_flag=0;
        
// 		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch1.tx_handle[0],can_bus_parameter_ch1.CAN_CH,1,64);
// 	}

// }


void can_init(void)
{
    can_handle_init(&can_bus_handle_ch1);
    #if defined (ENABLE_MULTI_CAN_CH)
    can_handle_init(&can_bus_handle_ch4);
    #endif

    can_normal_mode_set(&RCFDC0,
    					(CAN_FD_MODE_e)can_bus_parameter_ch1.CAN_MODE,
                        (CAN_CHANNEL_SEL_e)can_bus_parameter_ch1.CAN_CH,
                        (CAN_BIT_TIME_DSJW_SEL_e)can_bus_parameter_ch1.DSJW,              //DSJW
                        (CAN_BIT_TIME_DTSEG2_SEL_e)can_bus_parameter_ch1.DTSEG2,          //DTSEG2
                        (CAN_BIT_TIME_DTSEG1_SEL_e)can_bus_parameter_ch1.DTSEG1,          //DTSEG1
                        (CAN_BIT_TIME_DBRP_SEL_e)can_bus_parameter_ch1.DBRP,              //DBRP
                        (CAN_BIT_TIME_NSJW_SEL_e)can_bus_parameter_ch1.NSJW,              //NSJW
						(CAN_BIT_TIME_NTSEG2_SEL_e)can_bus_parameter_ch1.NTSEG2,          //NTSEG2
						(CAN_BIT_TIME_NTSEG1_SEL_e)can_bus_parameter_ch1.NTSEG1,          //NTSEG1
						(CAN_BIT_TIME_NBRP_SEL_e)can_bus_parameter_ch1.NBRP               //NBRP
                        );


    #if defined (ENABLE_MULTI_CAN_CH)
    can_normal_mode_set(&RCFDC0,
    					(CAN_FD_MODE_e)can_bus_parameter_ch4.CAN_MODE,
                        (CAN_CHANNEL_SEL_e)can_bus_parameter_ch4.CAN_CH,
                        (CAN_BIT_TIME_DSJW_SEL_e)can_bus_parameter_ch4.DSJW,              //DSJW
                        (CAN_BIT_TIME_DTSEG2_SEL_e)can_bus_parameter_ch4.DTSEG2,          //DTSEG2
                        (CAN_BIT_TIME_DTSEG1_SEL_e)can_bus_parameter_ch4.DTSEG1,          //DTSEG1
                        (CAN_BIT_TIME_DBRP_SEL_e)can_bus_parameter_ch4.DBRP,              //DBRP
                        (CAN_BIT_TIME_NSJW_SEL_e)can_bus_parameter_ch4.NSJW,              //NSJW
						(CAN_BIT_TIME_NTSEG2_SEL_e)can_bus_parameter_ch4.NTSEG2,          //NTSEG2
						(CAN_BIT_TIME_NTSEG1_SEL_e)can_bus_parameter_ch4.NTSEG1,          //NTSEG1
						(CAN_BIT_TIME_NBRP_SEL_e)can_bus_parameter_ch4.NBRP               //NBRP
                        );
    #endif

    can_rx_buffer_set(&RCFDC0);

    can_rrt_set(&RCFDC0,&can_bus_handle_ch1,tbl_can_bus_rx_rule_ch1,CAN_RX_RULE_CURRENT_AMOUNT);
    #if defined (ENABLE_MULTI_CAN_CH)
    can2_rrt_set(&RCFDC0,&can_bus_handle_ch4,tbl_can_bus_rx_rule_ch4,CAN2_RX_RULE_CURRENT_AMOUNT);
    #endif

    can_fifo_buffer_set(&RCFDC0,
                        (CAN_RX_FIFO_BUFER_NUMBER_e)can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM,
                        #if defined (CAN_RX_POLLING)
                        CAN_RX_FIFO_BUFFER_INIERRUPT_DISABLE,   //CAN_RX_FIFO_BUFFER_INIERRUPT_ENABLE
                        #elif defined (CAN_RX_INTERRUPT)
                        CAN_RX_FIFO_BUFFER_INIERRUPT_ENABLE,
                        #endif
						CAN_RX_FIFO_BUFFER_DEPTH16,
						CAN_RX_FIFO_BUFFER_PAYLOAD_64bytes,
                        CAN_RX_FIFO_BUFFER_FIFO_INT_FULL
                        );

    #if defined (ENABLE_MULTI_CAN_CH)
    can_fifo_buffer_set(&RCFDC0,
                        (CAN_RX_FIFO_BUFER_NUMBER_e)can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM,
                        #if defined (CAN_RX_POLLING)
                        CAN_RX_FIFO_BUFFER_INIERRUPT_DISABLE,   //CAN_RX_FIFO_BUFFER_INIERRUPT_ENABLE
                        #elif defined (CAN_RX_INTERRUPT)
                        CAN_RX_FIFO_BUFFER_INIERRUPT_ENABLE,
                        #endif
						CAN_RX_FIFO_BUFFER_DEPTH16,
						CAN_RX_FIFO_BUFFER_PAYLOAD_64bytes,
                        CAN_RX_FIFO_BUFFER_FIFO_INT_FULL
                        );
    #endif

    // can_tx_normal_buffer_set(&can_bus_handle_ch1,can_bus_parameter_ch1.CAN_MODE);

    can_start(&RCFDC0,can_bus_parameter_ch1.CAN_CH);
    #if defined (ENABLE_MULTI_CAN_CH)
    can_start(&RCFDC0,can_bus_parameter_ch4.CAN_CH);
    #endif

    //can_fifo_buffer_enable must execute after the can_start
    can_fifo_buffer_enable(&RCFDC0,can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);
    #if defined (ENABLE_MULTI_CAN_CH)
    can_fifo_buffer_enable(&RCFDC0,can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM);
    #endif

    can_syncp(&RCFDC0);

    can_reg_dump_log();
}

void R_CLKC_SetRscanClockDomain(uint32_t RscanModuleClockDomain, uint32_t RscanComClockDomain)
{
    /* Select RS-CAN Module Clock */
    protected_write(WPROTR.PROTCMD1, WPROTR.PROTS1, CLKCTL.CKSC_ICANS_CTL, RscanModuleClockDomain);
    while (CLKCTL.CKSC_ICANS_ACT !=RscanModuleClockDomain);

    /* Select RS-CAN Communication Clock */
    protected_write(WPROTR.PROTCMD1, WPROTR.PROTS1, CLKCTL.CKSC_ICANOSCD_CTL, RscanComClockDomain);
    while (CLKCTL.CKSC_ICANOSCD_CTL != RscanComClockDomain);
}


/*
    need to replace when code generage EACH TIMES
*/
// TODO : 
// @ r_cg_invector.c

// [SEARCH1]
// /* CAN receive FIFO interrupt; */
// extern void eiint23(void);

// [REPLACE WITH BELOW]
/* CAN receive FIFO interrupt; */
// extern void can_rx_fifo_interrupt(void);

// [SEARCH2]
// /* CAN receive FIFO interrupt; */
// (void *)eiint23,

// [REPLACE WITH BELOW]
// /* CAN receive FIFO interrupt; */
// (void *)can_rx_fifo_interrupt,


/* CAN GLOBAL ERROR INTERRUPT */
#pragma interrupt can_global_error_interrupt(enable=false, channel=22, fpu=true, callt=false)
void can_global_error_interrupt(void)
{
    // can_global_error_interrupt_cbk(&RCFDC0);
}

#pragma interrupt can_rx_fifo_interrupt(enable=false, channel=23, fpu=true, callt=false)
void can_rx_fifo_interrupt(void)
{
    /* Start user code for can_rx_fifo_interrupt. Do not edit comment generated here */
    can_rx_interrupt_cbk();
    /* End user code. Do not edit comment generated here */
}


// void R_CANFD_Global_error_Interrupt_Init(void)
// {

//     // global error
//     INTC1.ICRCANGERR0.BIT.TBRCANGERR0 = _INT_TABLE_VECTOR; //select table interrupt
//     INTC1.ICRCANGERR0.BIT.RFRCANGERR0 = _INT_REQUEST_NOT_OCCUR;
//     INTC1.ICRCANGERR0.BIT.MKRCANGERR0 = _INT_PROCESSING_ENABLED;
//     /*
//         Corresponding Interrupt Enable Bit
//         DEIE in the RCFDCnCFDGCTR register 
//         MEIE in the RCFDCnCFDGCTR register 
//         THLEIE in the RCFDCnCFDGCTR register 
//         CMPOFIE in the RCFDCnCFDGCTR register 
//     */

//     CAN_REG_CLR(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);   //DEIE
//     CAN_REG_SET(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_1);

//     /*
//         DEIE Bit 
//         When the DEIE bit is set to 1 and the DEF flag in the RCFDCnCFDGERFL register is set to 1, an interrupt request is 
//         generated. Modify this bit only in global reset mode. 

//         DLC Error Interrupt Enable 
//         0: DLC error interrupt is disabled. 
//         1: DLC error interrupt is enabled.     
//     */

//     CAN_REG_CLR(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT9,CAN_REG_LENGTH_1);   //MEIE
//     CAN_REG_SET(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT9,CAN_REG_LENGTH_1);

//     /*
//         MEIE Bit 
//         When the MEIE bit is set to 1 and the MES flag in the RCFDCnCFDGERFL register is set to 1, an interrupt request is 
//         generated. Modify this bit only in global reset mode. 

//         FIFO Message Lost Interrupt Enable 
//         0: FIFO message lost interrupt is disabled. 
//         1: FIFO message lost interrupt is enabled.     
//     */

//     CAN_REG_CLR(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT10,CAN_REG_LENGTH_1);  //THLEIE
//     CAN_REG_SET(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT10,CAN_REG_LENGTH_1);

//     /*
//         THLEIE Bit 
//         When the THLEIE bit is set to 1 and the THLES flag in the RCFDCnCFDGERFL register is set to 1, an interrupt 
//         request is generated. Modify this bit only in global reset mode. 

//         Transmit History Buffer Overflow Interrupt Enable 
//         0: Transmit history buffer overflow interrupt is disabled. 
//         1: Transmit history buffer overflow interrupt is enabled.    
//     */

//     CAN_REG_CLR(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT11,CAN_REG_LENGTH_1);  //CMPOFIE 
//     CAN_REG_SET(RCFDC0.CFDGCTR.UINT32,CAN_REG_BIT11,CAN_REG_LENGTH_1);

//     /*
//         CMPOFIE Bit 
//         When the CMPOF flag in the RCFDCnCFDGERFL register is set to 1 after the CMPOFIE bit is set to 1, an interrupt 
//         request occurs. Modify this bit only in global reset mode. 

//         Payload Overflow Interrupt Enable 
//         0: A payload overflow interrupt is disabled. 
//         1: A payload overflow interrupt is enabled.     
//     */
// }

void R_CANFD_Interrupt_Control_Init(void)
{
    /*
        22   ICRCANGERR0  	FFFE EA2C H   INTRCANGERR0  	CAN global error interrupt 
        23   ICRCANGRECC0  	FFFE EA2E H   INTRCANGRECC0  	CAN receive FIFO interrupt
        
        24   ICRCAN0ERR  	FFFE EA30 H   INTRCAN0ERR  		CAN0 error interrupt
        25   ICRCAN0REC  	FFFE EA32 H   INTRCAN0REC  		CAN0 transmit/receive FIFO receive complete interrupt 
        26   ICRCAN0TRX  	FFFE EA34 H   INTRCAN0TRX  		CAN0 transmit interrupt
        
        113  ICRCAN1ERR  	FFFF B0E2 H   INTRCAN1ERR  		CAN1 error interrupt
        114  ICRCAN1REC  	FFFF B0E4 H   INTRCAN1REC  		CAN1 transmit/receive FIFO receive complete interrupt 
        115  ICRCAN1TRX  	FFFF B0E6 H   INTRCAN1TRX  		CAN1 transmit interrupt
                    
        217  ICRCAN2ERR  	FFFF B1B2 H   INTRCAN2ERR  		CAN2 error interrupt
        218  ICRCAN2REC  	FFFF B1B4 H   INTRCAN2REC  		CAN2 transmit/receive FIFO receive complete interrupt 
        219  ICRCAN2TRX  	FFFF B1B6 H   INTRCAN2TRX  		CAN2 transmit interrupt  
                    
        220  ICRCAN3ERR  	FFFF B1B8 H   INTRCAN3ERR  		CAN3 error interrupt  
        221  ICRCAN3REC  	FFFF B1BA H   INTRCAN3REC  		CAN3 transmit/receive FIFO receive complete interrupt 
        222  ICRCAN3TRX  	FFFF B1BC H   INTRCAN3TRX  		CAN3 transmit interrupt
                    
        272  ICRCAN4ERR  	FFFF B220 H   INTRCAN4ERR  		CAN4 error interrupt  
        273  ICRCAN4REC  	FFFF B222 H   INTRCAN4REC  		CAN4 transmit/receive FIFO receive complete interrupt 
        274  ICRCAN4TRX  	FFFF B224 H   INTRCAN4TRX  		CAN4 transmit interrupt
                    
        287  ICRCAN5ERR  	FFFF B23E H   INTRCAN5ERR  		CAN5 error interrupt  
        288  ICRCAN5REC  	FFFF B240 H   INTRCAN5REC  		CAN5 transmit/receive FIFO receive complete interrupt 
        289  ICRCAN5TRX  	FFFF B242 H   INTRCAN5TRX  		CAN5 transmit interrupt
    */

    /*INTRCANGRECC0 : CAN CHANNEL RX FIFO interrupt*/

    INTC1.ICRCANGRECC0.BIT.CTRCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.RFRCANGRECC0 = _INT_REQUEST_NOT_OCCUR;        
    INTC1.ICRCANGRECC0.BIT.MKRCANGRECC0 = _INT_PROCESSING_ENABLED;
    INTC1.ICRCANGRECC0.BIT.TBRCANGRECC0 = _INT_TABLE_VECTOR; //select table interrupt

    INTC1.ICRCANGRECC0.BIT.P3RCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.P2RCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.P1RCANGRECC0 = 1;
    INTC1.ICRCANGRECC0.BIT.P0RCANGRECC0 = 1;

    //CAN Receive/Transmit FIFO receive complete interrupt
    // INTC1.ICRCAN1REC.BIT.TBRCAN1REC = _INT_TABLE_VECTOR;
    // INTC1.ICRCAN1REC.BIT.MKRCAN1REC = _INT_PROCESSING_ENABLED;
    // INTC1.ICRCAN1REC.BIT.RFRCAN1REC = _INT_REQUEST_NOT_OCCUR;

    //CAN channel 1 Transmission interrupt
    // INTC1.ICRCAN1TRX.BIT.TBRCAN1TRX = _INT_TABLE_VECTOR;
    // INTC1.ICRCAN1TRX.BIT.MKRCAN1TRX = _INT_PROCESSING_ENABLED;
    // INTC1.ICRCAN1TRX.BIT.RFRCAN1TRX = _INT_REQUEST_NOT_OCCUR;

}

void R_CANFD_Deinit(void)
{
    // R_PORT_SetGpioHighZ(Port0, 7);
    // R_PORT_SetGpioHighZ(Port0, 8);
    // R_PORT_SetGpioHighZ(Port0, 9);
    // R_PORT_SetGpioHighZ(Port0, 10);
}


/*
    F1KM/S1
    refer to c  Alternative Function , Table 2C.41

    CAN0RX:P0_1(ALT2/ALT7)/P10_0(ALT2/ALT7) 
    CAN0TX:P0_0(ALT2)/P10_1(ALT2)

    CAN1RX:P0_2(ALT2/ALT7)/P10_6(ALT4/ALT7)
    CAN1TX:P0_3(ALT2)/P10_7(ALT4)

    CAN2RX:P0_5(ALT1/ALT7)                      /P11_15(ALT1/ALT7)
    CAN2TX:P0_4(ALT1)                           /P12_0(ALT1)

    CAN3RX:P0_7(ALT5/ALT7)/P11_3(ALT2/ALT7)     /P1_2(ALT1/ALT7)
    CAN3TX:P0_8(ALT5)/P11_4(ALT1)               /P1_3(ALT1)

    CAN4RX:P0_9(ALT5/ALT7)                      /P1_12(ALT7)/P20_2(ALT7)    
    CAN4TX:P0_10(ALT5)                          /P1_13(ALT1)/P20_3(ALT1)
    
    CAN5RX:P0_13(ALT5/ALT7)/P11_5(ALT7)
    CAN5TX:P0_14(ALT5)/P11_6(ALT1)

*/
void R_CANFD_GPIO_ALT_Init(void)
{
    // R_PORT_SetAltFunc(Port0, 1, Alt7, Input);   /* CAN0 RX */
    // R_PORT_SetAltFunc(Port0, 0, Alt2, Output);  /* CAN0 TX*/
    // R_PORT_SetAltFunc(Port10, 0, Alt7, Input);   /* CAN0 RX */
    // R_PORT_SetAltFunc(Port10, 1, Alt2, Output);  /* CAN0 TX*/

    // R_PORT_SetAltFunc(Port0, 2, Alt7, Input);   /* CAN1 RX */
    // R_PORT_SetAltFunc(Port0, 3, Alt2, Output);  /* CAN1 TX*/
    R_PORT_SetAltFunc(Port10, 6, Alt4, Input);   /* CAN1 RX */
    R_PORT_SetAltFunc(Port10, 7, Alt4, Output);  /* CAN1 TX*/

    // R_PORT_SetAltFunc(Port0, 5, Alt7, Input);   /* CAN2 RX */
    // R_PORT_SetAltFunc(Port0, 4, Alt1, Output);  /* CAN2 TX*/
    // R_PORT_SetAltFunc(Port11, 15, Alt7, Input);   /* CAN2 RX */
    // R_PORT_SetAltFunc(Port12, 0, Alt1, Output);  /* CAN2 TX*/

    // R_PORT_SetAltFunc(Port0, 7, Alt7, Input);   /* CAN3 RX */
    // R_PORT_SetAltFunc(Port0, 8, Alt5, Output);  /* CAN3 TX*/
    // R_PORT_SetAltFunc(Port11, 3, Alt7, Input);   /* CAN3 RX */
    // R_PORT_SetAltFunc(Port11, 4, Alt1, Output);  /* CAN3 TX*/
    // R_PORT_SetAltFunc(Port1, 2, Alt7, Input);   /* CAN3 RX */
    // R_PORT_SetAltFunc(Port1, 3, Alt1, Output);  /* CAN3 TX*/

    R_PORT_SetAltFunc(Port0, 9, Alt5, Input);   /* CAN4 TX*/
    R_PORT_SetAltFunc(Port0, 10, Alt5, Output);  /* CAN4 RX */
    // R_PORT_SetAltFunc(Port1, 12, Alt7, Input);   /* CAN4 TX*/
    // R_PORT_SetAltFunc(Port1, 13, Alt1, Output);  /* CAN4 RX */
    // R_PORT_SetAltFunc(Port20, 2, Alt7, Input);   /* CAN4 TX*/
    // R_PORT_SetAltFunc(Port20, 3, Alt1, Output);  /* CAN4 RX */

    // R_PORT_SetAltFunc(Port0, 13, Alt7, Input);   /* CAN5 TX*/
    // R_PORT_SetAltFunc(Port0, 14, Alt5, Output);  /* CAN5 RX */
    // R_PORT_SetAltFunc(Port11, 5, Alt7, Input);   /* CAN5 TX*/
    // R_PORT_SetAltFunc(Port11, 6, Alt1, Output);  /* CAN5 RX */
}

void R_CANFD_Init(void)
{
    R_CANFD_GPIO_ALT_Init();

    /*  Module Clock = PPLLCLK, Communication Clock = MainOSC */
    // R_CLKC_SetRscanClockDomain(R_CLKC_RSCAN_PPLLCLK,
    //                             R_CLKC_RSCAN_DIV_MOSC1);

    #if defined (CAN_RX_INTERRUPT)
    R_CANFD_Interrupt_Control_Init();
    #endif

    can_init();

}


void can_reg_dump_log(void)
{
    tiny_printf("CFDC0NCFG:0x%08X\r\n", RCFDC0.CFDC0NCFG.UINT32);
    tiny_printf("CFDC0CTR:0x%08X\r\n", RCFDC0.CFDC0CTR.UINT32);
    tiny_printf("CFDC0STS:0x%08X\r\n", RCFDC0.CFDC0STS.UINT32);
    tiny_printf("CFDC0ERFL:0x%08X\r\n", RCFDC0.CFDC0ERFL.UINT32);

    tiny_printf("CFDC3NCFG:0x%08X\r\n", RCFDC0.CFDC3NCFG.UINT32);
    tiny_printf("CFDC3CTR:0x%08X\r\n", RCFDC0.CFDC3CTR.UINT32);
    tiny_printf("CFDC3STS:0x%08X\r\n", RCFDC0.CFDC3STS.UINT32);
    tiny_printf("CFDC3ERFL:0x%08X\r\n", RCFDC0.CFDC3ERFL.UINT32);      


    tiny_printf("CFDRFCC0:0x%08X\r\n", RCFDC0.CFDRFCC0.UINT32);

    tiny_printf("GAFLID0:0x%08X\r\n", RCFDC0.CFDGAFLID0.UINT32);
    tiny_printf("GAFLMASK0:0x%08X\r\n", RCFDC0.CFDGAFLM0.UINT32);
    tiny_printf("GAFLP0_0:0x%08X\r\n", RCFDC0.CFDGAFLP0_0.UINT32);
    tiny_printf("GAFLP1_0:0x%08X\r\n", RCFDC0.CFDGAFLP1_0.UINT32);

    tiny_printf("CFDGCTR:0x%08X\r\n", RCFDC0.CFDGCTR.UINT32);
    tiny_printf("CFDGSTS:0x%08X\r\n", RCFDC0.CFDGSTS.UINT32); 

    tiny_printf("CFDGAFLECTR:0x%08X\r\n", RCFDC0.CFDGAFLECTR.UINT32); 
    tiny_printf("CFDGAFLCFG0:0x%08X\r\n", RCFDC0.CFDGAFLCFG0.UINT32); 
    tiny_printf("CFDGAFLCFG1:0x%08X\r\n", RCFDC0.CFDGAFLCFG1.UINT32); 

    tiny_printf("CFDRMNB:0x%08X\r\n", RCFDC0.CFDRMNB.UINT32);
    tiny_printf("CFDRMND0:0x%08X\r\n", RCFDC0.CFDRMND0.UINT32);
}

void can_rx_buffer_set(CAN_REG_TYP * can)
{
    /* Set Rx buffer number*/
    //maximum number of RX buffer is 128    
    //Receive buffer: RMPLS[2:0] bits in the RCFDCnCFDRMNB register 
    CAN_REG_SET(can->CFDRMNB.UINT32,CAN_REG_BIT0,128);                      //NRXMB[7:0]  Receive Buffer Number Configuration 
    CAN_REG_SET(can->CFDRMNB.UINT32,CAN_REG_BIT8,CAN_REG_LENGTH_3);         //RMPLS[2:0]  Receive Buffer Payload Storage Size Select 

}

static void can_tx2_normal_buffer_set(CAN_BUS_HANDLE *p,CAN_FD_MODE_e mode,unsigned int p_number,unsigned int transmit_buffer_id,unsigned char *txbuffer,unsigned char len )
{    
    unsigned int buf_cnt=0;
    unsigned short i=0;
    unsigned short dlc=0;
    //TX buffer is fixed number.
    //Evety CAN channel have 32 transmit buffer
    //TX buffer number = 32*m +31 (m : channel number)

    //Channel 0 : TX buffer Number 0~31
    // for(buf_cnt=0;buf_cnt<16;buf_cnt++)
    {
        p_number = buf_cnt;
        p->tx_handle[p_number].id.bit.tmid =(unsigned long)(transmit_buffer_id+p_number);
    
        p->tx_handle[p_number].id.bit.hlen =(unsigned char)TX_DATA_HISTORY_NOT_STORE;
        p->tx_handle[p_number].id.bit.rtr =(unsigned char)TX_DATA_FRAME;
        p->tx_handle[p_number].id.bit.ide =(unsigned char)TX_STANDARD_ID;

        p->tx_handle[p_number].ptr.bit.dlc=(unsigned int)TX_DATA_LENGTH_1;
        // p->tx_handle[p_number].df[0].byte.TMDB[0] = 0x10+p_number;
        p->tx_handle[p_number].df[0].byte.TMDB[0] = p_number;
        p->tx_handle[p_number].tmc.bit.tmom=0;
        p->tx_handle[p_number].tmc.bit.tmtar=0;

        if(mode == CAN_FD_MIX_MODE || mode == CAN_FD_ONLY_MODE)
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CAN_FD_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 1;
        }
        else
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CLASSIC_CAN_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_NO_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 0;
        }
        //always set 1
        p->tx_handle[p_number].tmc.bit.tmtr=1;
    }

    // data buffer init.
    p_number=0;

    switch(len)
    {
        case 8:
            dlc = (unsigned short)CAN_DLC_8BYTE;
            break;
        case 16:
            dlc = (unsigned short)CAN_DLC_16BYTE;
            break;
        case 48:
            dlc = (unsigned short)CAN_DLC_48BYTE;
            break;
        case 64:
            dlc = (unsigned short)CAN_DLC_64BYTE;
            break;
    }
    p->tx_handle[p_number].ptr.bit.dlc=dlc;

    for(i=0;i<(dlc+1);i++)
    {
        p->tx_handle[p_number].df[i].byte.TMDB[0] = *(txbuffer+i*4+0);
        p->tx_handle[p_number].df[i].byte.TMDB[1] = *(txbuffer+i*4+1);
        p->tx_handle[p_number].df[i].byte.TMDB[2] = *(txbuffer+i*4+2);
        p->tx_handle[p_number].df[i].byte.TMDB[3] = *(txbuffer+i*4+3);
    }
}


static void can_tx_normal_buffer4_set(CAN_BUS_HANDLE *p,CAN_FD_MODE_e mode,unsigned int p_number,unsigned int transmit_buffer_id,unsigned char *txbuffer )
{    
    unsigned int buf_cnt=0;
    unsigned short i=0;
    unsigned short dlc=0;
    //TX buffer is fixed number.
    //Evety CAN channel have 32 transmit buffer
    //TX buffer number = 32*m +31 (m : channel number)

    //Channel 0 : TX buffer Number 0~31
    // for(buf_cnt=0;buf_cnt<16;buf_cnt++)
    {
        p_number = buf_cnt;
        p->tx_handle[p_number].id.bit.tmid =(unsigned long)(transmit_buffer_id+p_number);
    
        p->tx_handle[p_number].id.bit.hlen =(unsigned char)TX_DATA_HISTORY_NOT_STORE;
        p->tx_handle[p_number].id.bit.rtr =(unsigned char)TX_DATA_FRAME;
        p->tx_handle[p_number].id.bit.ide =(unsigned char)TX_STANDARD_ID;

        p->tx_handle[p_number].ptr.bit.dlc=(unsigned int)TX_DATA_LENGTH_1;
        // p->tx_handle[p_number].df[0].byte.TMDB[0] = 0x10+p_number;
        p->tx_handle[p_number].df[0].byte.TMDB[0] = p_number;
        p->tx_handle[p_number].tmc.bit.tmom=0;
        p->tx_handle[p_number].tmc.bit.tmtar=0;

        if(mode == CAN_FD_MIX_MODE || mode == CAN_FD_ONLY_MODE)
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CAN_FD_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 1;
        }
        else
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CLASSIC_CAN_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_NO_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 0;
        }
        //always set 1
        p->tx_handle[p_number].tmc.bit.tmtr=1;
    }

    // data buffer init.
    p_number=0;
    dlc = (unsigned short)CAN_DLC_64BYTE;
    p->tx_handle[p_number].ptr.bit.dlc=dlc;

    for(i=0;i<(dlc+1);i++)
    {
        p->tx_handle[p_number].df[i].byte.TMDB[0] = *(txbuffer+i*4+0);
        p->tx_handle[p_number].df[i].byte.TMDB[1] = *(txbuffer+i*4+1);
        p->tx_handle[p_number].df[i].byte.TMDB[2] = *(txbuffer+i*4+2);
        p->tx_handle[p_number].df[i].byte.TMDB[3] = *(txbuffer+i*4+3);
    }
}

static void can_tx_normal_buffer3_set(CAN_BUS_HANDLE *p,CAN_FD_MODE_e mode,unsigned int p_number,unsigned int transmit_buffer_id,unsigned char *txbuffer )
{    
    unsigned int buf_cnt=0;
    unsigned short i=0;
    unsigned short dlc=0;
    //TX buffer is fixed number.
    //Evety CAN channel have 32 transmit buffer
    //TX buffer number = 32*m +31 (m : channel number)

    //Channel 0 : TX buffer Number 0~31
    // for(buf_cnt=0;buf_cnt<16;buf_cnt++)
    {
        p_number = buf_cnt;
        p->tx_handle[p_number].id.bit.tmid =(unsigned long)(transmit_buffer_id+p_number);
    
        p->tx_handle[p_number].id.bit.hlen =(unsigned char)TX_DATA_HISTORY_NOT_STORE;
        p->tx_handle[p_number].id.bit.rtr =(unsigned char)TX_DATA_FRAME;
        p->tx_handle[p_number].id.bit.ide =(unsigned char)TX_STANDARD_ID;

        p->tx_handle[p_number].ptr.bit.dlc=(unsigned int)TX_DATA_LENGTH_1;
        // p->tx_handle[p_number].df[0].byte.TMDB[0] = 0x10+p_number;
        p->tx_handle[p_number].df[0].byte.TMDB[0] = p_number;
        p->tx_handle[p_number].tmc.bit.tmom=0;
        p->tx_handle[p_number].tmc.bit.tmtar=0;

        if(mode == CAN_FD_MIX_MODE || mode == CAN_FD_ONLY_MODE)
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CAN_FD_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 1;
        }
        else
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CLASSIC_CAN_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_NO_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 0;
        }
        //always set 1
        p->tx_handle[p_number].tmc.bit.tmtr=1;
    }

    // data buffer init.
    p_number=0;
    dlc = (unsigned short)CAN_DLC_16BYTE;
    p->tx_handle[p_number].ptr.bit.dlc=dlc;

    for(i=0;i<(dlc+1);i++)
    {
        p->tx_handle[p_number].df[i].byte.TMDB[0] = *(txbuffer+i*4+0);
        p->tx_handle[p_number].df[i].byte.TMDB[1] = *(txbuffer+i*4+1);
        p->tx_handle[p_number].df[i].byte.TMDB[2] = *(txbuffer+i*4+2);
        p->tx_handle[p_number].df[i].byte.TMDB[3] = *(txbuffer+i*4+3);
    }
}

static void can_tx_normal_buffer2_set(CAN_BUS_HANDLE *p,CAN_FD_MODE_e mode,unsigned int p_number,unsigned int transmit_buffer_id )
{    
    static unsigned char counter = 0;
    // unsigned int transmit_buffer_id = 0x205;
    // unsigned int p_number=0;
    unsigned int buf_cnt=0;
    unsigned short i=0;
    unsigned short dlc=0;
    //TX buffer is fixed number.
    //Evety CAN channel have 32 transmit buffer
    //TX buffer number = 32*m +31 (m : channel number)

    //Channel 0 : TX buffer Number 0~31
    // for(buf_cnt=0;buf_cnt<16;buf_cnt++)
    {
        p_number = buf_cnt;
        p->tx_handle[p_number].id.bit.tmid =(unsigned long)(transmit_buffer_id+p_number);
    
        p->tx_handle[p_number].id.bit.hlen =(unsigned char)TX_DATA_HISTORY_NOT_STORE;
        p->tx_handle[p_number].id.bit.rtr =(unsigned char)TX_DATA_FRAME;
        p->tx_handle[p_number].id.bit.ide =(unsigned char)TX_STANDARD_ID;

        p->tx_handle[p_number].ptr.bit.dlc=(unsigned int)TX_DATA_LENGTH_1;
        // p->tx_handle[p_number].df[0].byte.TMDB[0] = 0x10+p_number;
        p->tx_handle[p_number].df[0].byte.TMDB[0] = p_number;
        p->tx_handle[p_number].tmc.bit.tmom=0;
        p->tx_handle[p_number].tmc.bit.tmtar=0;

        if(mode == CAN_FD_MIX_MODE || mode == CAN_FD_ONLY_MODE)
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CAN_FD_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 1;
        }
        else
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CLASSIC_CAN_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_NO_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 0;
        }
        //always set 1
        p->tx_handle[p_number].tmc.bit.tmtr=1;
    }

    // data buffer init.
    p_number=0;
    dlc = (unsigned short)CAN_DLC_32BYTE;
    p->tx_handle[p_number].ptr.bit.dlc=dlc;

    for(i=0;i<(dlc+1);i++)
    {
        p->tx_handle[p_number].df[i].byte.TMDB[0] = 0xAA+counter;
        p->tx_handle[p_number].df[i].byte.TMDB[1] = 0xAA+counter;
        p->tx_handle[p_number].df[i].byte.TMDB[2] = 0xAA+counter;
        p->tx_handle[p_number].df[i].byte.TMDB[3] = 0xAA+counter;
    }
    counter += 0x11;
}


static void can_tx_normal_buffer1_set(CAN_BUS_HANDLE *p,CAN_FD_MODE_e mode,unsigned int p_number,unsigned int transmit_buffer_id )
{    
    static unsigned char counter = 0;
    // unsigned int transmit_buffer_id = 0x205;
    // unsigned int p_number=0;
    unsigned int buf_cnt=0;
    unsigned short i=0;
    unsigned short dlc=0;
    //TX buffer is fixed number.
    //Evety CAN channel have 32 transmit buffer
    //TX buffer number = 32*m +31 (m : channel number)

    //Channel 0 : TX buffer Number 0~31
    // for(buf_cnt=0;buf_cnt<16;buf_cnt++)
    {
        p_number = buf_cnt;
        p->tx_handle[p_number].id.bit.tmid =(unsigned long)(transmit_buffer_id+p_number);
    
        p->tx_handle[p_number].id.bit.hlen =(unsigned char)TX_DATA_HISTORY_NOT_STORE;
        p->tx_handle[p_number].id.bit.rtr =(unsigned char)TX_DATA_FRAME;
        p->tx_handle[p_number].id.bit.ide =(unsigned char)TX_STANDARD_ID;

        p->tx_handle[p_number].ptr.bit.dlc=(unsigned int)TX_DATA_LENGTH_1;
        // p->tx_handle[p_number].df[0].byte.TMDB[0] = 0x10+p_number;
        p->tx_handle[p_number].df[0].byte.TMDB[0] = p_number;
        p->tx_handle[p_number].tmc.bit.tmom=0;
        p->tx_handle[p_number].tmc.bit.tmtar=0;

        if(mode == CAN_FD_MIX_MODE || mode == CAN_FD_ONLY_MODE)
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CAN_FD_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 1;
        }
        else
        {
        	p->tx_handle[p_number].tmfdctr.bit.tmfdf =TX_BUF_SEND_CLASSIC_CAN_FRAME;
        	p->tx_handle[p_number].tmfdctr.bit.tmbrs =TX_BUF_BIT_RATE_NO_CHANGE;
        	p->tx_handle[p_number].tmfdctr.bit.tmesi = 0;
        }
        //always set 1
        p->tx_handle[p_number].tmc.bit.tmtr=1;
    }

    // data buffer init.
    p_number=0;
    dlc = (unsigned short)CAN_DLC_64BYTE;
    p->tx_handle[p_number].ptr.bit.dlc=dlc;

    for(i=0;i<(dlc+1);i++)
    {
        p->tx_handle[p_number].df[i].byte.TMDB[0] = 0x10+i*4+1+counter;
        p->tx_handle[p_number].df[i].byte.TMDB[1] = 0x10+i*4+2+counter;
        p->tx_handle[p_number].df[i].byte.TMDB[2] = 0x10+i*4+3+counter;
        p->tx_handle[p_number].df[i].byte.TMDB[3] = 0x10+i*4+4+counter;
    }
    counter++;
}

// unsigned char can_global_error_interrupt_cbk(CAN_REG_TYP * can)
// {

//     /*
//         DEF in the RCFDCnCFDGERFL register
//         MES in the RCFDCnCFDGERFL register
//         THLES in the RCFDCnCFDGERFL register
//         CMPOF in the RCFDCnCFDGERFL register    
//     */

//     if (CAN_REG_READ(can->CFDGERFL.UINT32,CAN_REG_BIT0,0x1U))
//     {
//         /*
//             DEF  DLC Error Flag 
//             0: No DLC error has occurred. 
//             1: A DLC error has occurred.       
            
//             DEF Flag 
//             The DEF flag is set to 1 when an error has been detected during the DLC check. The program can clear this flag by 
//             writing 0 to this bit. 
//             To clear the flags of the register to 0, the program must write 0 to the corresponding flag to be cleared. When writing 0, 
//             using store instruction, set the bit to be set to “0” to “0”, and the bits not to be set to “0” to “1”.             

//         */
       
//         // CAN_REG_CLR(can->CFDGERFL.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1);    
//         can->CFDGERFL.UINT32 = 0xFFFFFFFEU;
//         tiny_printf("[CAN global error]DEF(DLC Error Flag)\r\n");
//     }


//     if (CAN_REG_READ(can->CFDGERFL.UINT32,CAN_REG_BIT1,0x1U))
//     {
//         /*
//             MES  FIFO Message Lost Status Flag 
//             0: No FIFO message lost error has occurred. 
//             1: A FIFO message lost error has occurred. 
//         */
       
//         CAN_REG_CLR(can->CFDGERFL.UINT32,CAN_REG_BIT1,CAN_REG_LENGTH_1);
        
//         tiny_printf("[CAN global error]MES(FIFO Message Lost Status Flag)\r\n");
//     }    

//     if (CAN_REG_READ(can->CFDGERFL.UINT32,CAN_REG_BIT2,0x1U))
//     {
//         /*
//             THLES  Transmit History Buffer Overflow Status Flag 
//             0: No transmit history buffer overflow has occurred. 
//             1: A transmit history buffer overflow has occurred. 
//         */
       
//         CAN_REG_CLR(can->CFDGERFL.UINT32,CAN_REG_BIT2,CAN_REG_LENGTH_1);
        
//         tiny_printf("[CAN global error]THLES(Transmit History Buffer Overflow Status Flag)\r\n");
//     }    

//     if (CAN_REG_READ(can->CFDGERFL.UINT32,CAN_REG_BIT3,0x1U))
//     {
//         /*
//             CMPOF  Payload Overflow Flag 
//             0: No payload overflow has occurred. 
//             1: A payload overflow has occurred. 
//         */
       
//         // CAN_REG_CLR(can->CFDGERFL.UINT32,CAN_REG_BIT3,CAN_REG_LENGTH_1);    
//         can->CFDGERFL.UINT32 = 0xFFFFFFF7U;
        
//         tiny_printf("[CAN global error]CMPOF(Payload Overflow Flag)\r\n");
//     }    

//     return 0;
// }

/*
    −  Receive buffers (RCFDCnCFDRMIDq, RCFDCnCFDRMPTRq, RCFDCnCFDRMFDSTSq, RCFDCnCFDRMDFb_q registers) 
*/

signed char can_fd_receive_buffer_decode(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number)
{

    /*
        Table 24.15  Indices for Individual Products (RH850/F1KM-S1)
        LQFP64 : CAN0 , CAN1 , CAN2
                m = 0 to 2
                z = 0 to 2
                j = 0 to 15
                k = 0 to 8
                x = 0 to 7
                d = 0 to 15
                q = 0 to 47
                p = 0 to 95
                b = 0 to 15
                r = 0 to 63
                y = 0, 1 
        LQFP100 : CAN0 , CAN1 , CAN2 , CAN3  , CAN4 , CAN5 
                m = 0 to 5 
                z = 0 to 5 
                j = 0 to 15 
                k = 0 to 17
                x = 0 to 7* 1
                d = 0 to 15
                q = 0 to 95
                p = 0 to 191
                b = 0 to 15
                r = 0 to 63 
                y = 0 to 2 
    */

    unsigned char i = 0;

    union __tagDAT *rmnd0 = (union __tagDAT *)(&can->CFDRMND0.UINT32);  //(y = 0 to 3) 
    union __tagDAT *rmid0 = (union __tagDAT *)(&can->CFDRMID0.UINT32);  //(q = 0 to 127)
    union __tagDAT *rmptr0 = (union __tagDAT *)(&can->CFDRMPTR0.UINT32);  //(q = 0 to 127)
    union __tagDAT *rmfdsts0 = (union __tagDAT *)(&can->CFDRMFDSTS0.UINT32);  //(q = 0 to 127)
    union __tagDAT *rmdf0_0 = (union __tagDAT *)(&can->CFDRMDF0_0.UINT32);  //(b = 0 to 15, q = 0 to 127) 

    unsigned long re_val = 0U;   
    unsigned short q_number = rfi_number << 5;

    unsigned char* s = NULL;

    /*Is the RMNSq flag in the RCFDCnCFDRMNDy register 1?*/
    #if 1
    if ((rmnd0[0].UINT32 == 0) && 
        (rmnd0[1].UINT32 == 0) && 
        (rmnd0[2].UINT32 == 0) )
    {
        return -1;
    }

    if ( rmnd0[0].UINT32 != (0x1U << rfi_number)) // hard code
    {
        return -1;
    }

    #else
    re_val = CAN_REG_READ(can->CFDRMND0.UINT32,CAN_REG_BIT0,0xFFFFFFFFU);

    if (re_val == 0)
    {
        return -1;
    }
    #endif

    tiny_printf("\r\n");
    tiny_printf("-----receive_buffer:0x%04X,0x%02X,(%s)-----\r\n",q_number,rfi_number,tbl_can_fifo_buffer_num[rfi_number]);
    tiny_printf("-----receive_buffer:0x%08X-----\r\n",rmnd0[0].UINT32);
    tiny_printf("-----receive_buffer:0x%08X-----\r\n",rmnd0[1].UINT32);
    tiny_printf("-----receive_buffer:0x%08X-----\r\n",rmnd0[2].UINT32);

    /*******************************/
    tiny_printf("RCFDCnCFDRMNDy:\r\n"); 
    for(i = 0 ; i < 3 ; i++)  // CFDRMND0 ~ CFDRMND2
    {
        tiny_printf("[%d:0x%08X]0x%08X\r\n",i,&rmnd0[i].UINT32,rmnd0[i].UINT32);
    }

    CAN_REG_CLR(can->CFDRMND0.UINT32,CAN_REG_BIT0,0xFFFFFFFF);
    CAN_REG_CLR(can->CFDRMND1.UINT32,CAN_REG_BIT0,0xFFFFFFFF);
    CAN_REG_CLR(can->CFDRMND2.UINT32,CAN_REG_BIT0,0xFFFFFFFF);

    /*******************************/
    tiny_printf("RCFDCnCFDRMIDq(0x%08X):0x%08X\r\n",
                &rmid0[q_number].UINT32,
                rmid0[q_number].UINT32);
    /*
        RMIDE  Receive Buffer IDE 
        0: Standard ID 
        1: Extended ID 
    */
    re_val = CAN_REG_READ(rmid0[q_number].UINT32,CAN_REG_BIT31,0x1U);
    tiny_printf("--RMIDE:0x%02X",re_val);
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Standard ID") : ("1:Extended ID") );

    /*
        RMRTR  Receive Buffer RTR/RRS 
        ●  When the received message is a classical CAN frame 
            0: Data frame 
            1: Remote frame 
        ●  When the received message is a CAN FD frame 
            The RRS bit value of the received message can be read.    
    */
    re_val = CAN_REG_READ(rmid0[q_number].UINT32,CAN_REG_BIT30,0x1U);
    tiny_printf("--RMRTR:0x%02X",re_val);
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Data frame") : ("1:Remote frame") );

    re_val = CAN_REG_READ(rmid0[q_number].UINT32,CAN_REG_BIT0,0x1FFFFFFFU);
    tiny_printf("--RMID:0x%02X\r\n",re_val);  //RMID[28:0] 

    /*******************************/
    tiny_printf("RCFDCnCFDRMPTRq(0x%08X):0x%08X\r\n",
                &rmptr0[q_number].UINT32,
                rmptr0[q_number].UINT32); 

    re_val = CAN_REG_READ(rmptr0[q_number].UINT32,CAN_REG_BIT28,0xFU);
    tiny_printf("--RMDLC:0x%02X",re_val); //RMDLC[3:0] 
    tiny_printf("[%s]\r\n",tbl_can_dlc[re_val]);    

    re_val = CAN_REG_READ(rmptr0[q_number].UINT32,CAN_REG_BIT0,0xFFFFU);
    tiny_printf("--RMTS:0x%02X",re_val); //Receive Buffer Timestamp Data
    s = "Receive Buffer Timestamp Data";
    tiny_printf("[%s]\r\n",s);    
    
    /*******************************/
    tiny_printf("RCFDCnCFDRMFDSTSq(0x%08X):0x%08X\r\n",
                &rmfdsts0[q_number].UINT32,
                rmfdsts0[q_number].UINT32); 

    re_val = CAN_REG_READ(rmfdsts0[q_number].UINT32,CAN_REG_BIT16,0xFFFFU);
    tiny_printf("--RMPTR:0x%02X",re_val); //RMPTR[15:0]  Receive Buffer Label Data 
    s = "Receive Buffer Label Data";
    tiny_printf("[%s]\r\n",s);    

    /*
        0: Classical CAN frame 
        1: CAN FD frame 
    */
    re_val = CAN_REG_READ(rmfdsts0[q_number].UINT32,CAN_REG_BIT2,0x1U);
    tiny_printf("--RMFDF:0x%02X",re_val); //RMFDF 
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Classical CAN frame") : ("1:CAN FD frame") );

    re_val = CAN_REG_READ(rmfdsts0[q_number].UINT32,CAN_REG_BIT1,0x1U);
    tiny_printf("--RMBRS:0x%02X",re_val); //RMBRS 
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:bit rate in the data area does not change") : ("1:bit rate in the data area changes") );

    re_val = CAN_REG_READ(rmfdsts0[q_number].UINT32,CAN_REG_BIT0,0x1U);
    tiny_printf("--RMESI:0x%02X",re_val); //RMESI 
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Error active node ") : ("1:Error passive node") );

    /*******************************/
    tiny_printf("RCFDCnCFDRMDFb_q:\r\n"); 
    for(i = 0;i < 16 ; i++)
    {
        tiny_printf("[%2d:0x%08X]0x%08X\r\n",i,&rmdf0_0[i+q_number].UINT32,rmdf0_0[i+q_number].UINT32);
    }

    tiny_printf("\r\n");   

    return 0;
}

/*
    −  Receive FIFO buffer access registers (RCFDCnCFDRFIDx, RCFDCnCFDRFPTRx, RCFDCnCFDRFFDSTSx, and RCFDCnCFDRFDFd_x registers) 
*/
void can_fd_receive_fifo_buffer_decode(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number)
{
    unsigned char i = 0;

    union __tagDAT *rfid0 = (union __tagDAT *)(&can->CFDRFID0.UINT32);  //(x = 0 to 7) 
    union __tagDAT *rfptr0 = (union __tagDAT *)(&can->CFDRFPTR0.UINT32);  //(x = 0 to 7) 
    union __tagDAT *rffdsts0 = (union __tagDAT *)(&can->CFDRFFDSTS0.UINT32);  //(x = 0 to 7) 
    union __tagDAT *rfdf0_0 = (union __tagDAT *)(&can->CFDRFDF0_0.UINT32);  //(d = 0 to 15, x = 0 to 7) 

    unsigned long re_val = 0U;    
    unsigned short q_number = rfi_number << 5;

    unsigned char* s = NULL;

    tiny_printf("-----receive_fif0_buffer-----\r\n");

    /*******************************/
    tiny_printf("RCFDCnCFDRFIDx(0x%08X):0x%08X\r\n",
                &rfid0[q_number].UINT32,
                rfid0[q_number].UINT32); 
    /*
        RFIDE  Receive FIFO Buffer IDE 
        0: Standard ID 
        1: Extended ID 
    */
    re_val = CAN_REG_READ(rfid0[q_number].UINT32,CAN_REG_BIT31,0x1U);
    tiny_printf("--RFIDE:0x%02X",re_val);
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Standard ID") : ("1:Extended ID") );

    /*
        Receive FIFO Buffer RTR/RRS 
        ●  When the received message is a classical CAN frame 
            0: Data frame 
            1: Remote frame 
        ●  When the received message is a CAN FD frame 
        The RRS bit value of the received message can be read. 
    */
    re_val = CAN_REG_READ(rfid0[q_number].UINT32,CAN_REG_BIT30,0x1U);
    tiny_printf("--RFRTR:0x%02X",re_val);
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Data frame") : ("1:Remote frame") );

    re_val = CAN_REG_READ(rfid0[q_number].UINT32,CAN_REG_BIT0,0x1FFFFFFFU);
    tiny_printf("--RMID:0x%02X\r\n",re_val);  //RFID[28:0]  Receive FIFO Buffer ID Data 

    /*******************************/
    tiny_printf("RCFDCnCFDRFPTRx(0x%08X):0x%08X\r\n",
                &rfptr0[q_number].UINT32,
                rfptr0[q_number].UINT32); 

    re_val = CAN_REG_READ(rfptr0[q_number].UINT32,CAN_REG_BIT28,0xFU);
    tiny_printf("--RFDLC:0x%02X",re_val); //RFDLC[3:0]  Receive FIFO Buffer DLC Data 
    tiny_printf("[%s]\r\n",tbl_can_dlc[re_val]);   

    re_val = CAN_REG_READ(rfptr0[q_number].UINT32,CAN_REG_BIT0,0xFFFFU);
    tiny_printf("--RFTS:0x%02X",re_val); //RFTS[15:0]  Receive FIFO Buffer Timestamp Data
    s = "Receive FIFO Buffer Timestamp Data";
    tiny_printf("[%s]\r\n",s);  

    /*******************************/
    tiny_printf("RCFDCnCFDRFFDSTSx(0x%08X):0x%08X\r\n",
                &rffdsts0[q_number].UINT32,
                rffdsts0[q_number].UINT32); 

    re_val = CAN_REG_READ(rffdsts0[q_number].UINT32,CAN_REG_BIT16,0xFFFFU);
    tiny_printf("--RFPTR:0x%02X",re_val); //RFPTR[15:0]  Receive FIFO Buffer Label Data 
    s = "Receive FIFO Buffer Label Data";
    tiny_printf("[%s]\r\n",s);  

    /*
        FDF 
        0: Classical CAN frame 
        1: CAN FD frame     
    */
    re_val = CAN_REG_READ(rffdsts0[q_number].UINT32,CAN_REG_BIT2,0x1U);
    tiny_printf("--RFFDF:0x%02X",re_val); //RFFDF 
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Classical CAN frame") : ("1:CAN FD frame") );

    /*
        BRS 
        0: The bit rate in the data area does not change. 
        1: The bit rate in the data area changes.     

    */
    re_val = CAN_REG_READ(rffdsts0[q_number].UINT32,CAN_REG_BIT1,0x1U);
    tiny_printf("--RFBRS:0x%02X",re_val); //RFBRS 
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:bit rate in the data area does not change") : ("1:bit rate in the data area changes") );

    /*
        ESI 
        0: Error active node 
        1: Error passive node 

    */
    re_val = CAN_REG_READ(rffdsts0[q_number].UINT32,CAN_REG_BIT0,0x1U);
    tiny_printf("--RFESI:0x%02X",re_val); //RFESI 
    tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:Error active node ") : ("1:Error passive node") );

    /*******************************/
    tiny_printf("RCFDCnCFDRFDFd_x:\r\n"); 
    for(i = 0;i < 16 ; i++)
    {
        tiny_printf("[%2d:0x%08X]0x%08X\r\n",i,&rfdf0_0[i+q_number].UINT32,rfdf0_0[i+q_number].UINT32);
    }

    // tiny_printf("\r\n"); 
}

void can_fd_receive_fifo_buffer_status(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number)
{
    // unsigned char i = 0;
    unsigned char flag = 0;

    union __tagDAT *rmnd0 = (union __tagDAT *)(&can->CFDRMND0.UINT32);  //(y = 0 to 3) 
    union __tagDAT *rfsts0 = (union __tagDAT *)(&can->CFDRFSTS0.UINT32);
    union __tagDAT *rfpctr = (union __tagDAT *)(&can->CFDRFPCTR0.UINT32);

    unsigned long re_val = 0U;    
    // unsigned short q_number = 0U;

    // unsigned char* s = NULL;

    flag = (rmnd0[0].UINT32 | rmnd0[1].UINT32 | rmnd0[2].UINT32) ? 1 : 0;   

    CAN_REG_CLR(can->CFDRMND0.UINT32,CAN_REG_BIT0,0xFFFFFFFF);
    CAN_REG_CLR(can->CFDRMND1.UINT32,CAN_REG_BIT0,0xFFFFFFFF);
    CAN_REG_CLR(can->CFDRMND2.UINT32,CAN_REG_BIT0,0xFFFFFFFF);  

    if(flag)
    {        
        tiny_printf("-----receive_fifo_buffer_status-----\r\n");

        tiny_printf("RCFDCnCFDRFSTSx(0x%08X):0x%08X\r\n",
                    &rfsts0[rfi_number].UINT32,
                    rfsts0[rfi_number].UINT32); 

        re_val = CAN_REG_READ(rfsts0[rfi_number].UINT32,CAN_REG_BIT8,0xFFU);
        tiny_printf("--RFMC:0x%02X\r\n",re_val); //RFMC[7:0]  Receive FIFO Unread Message Counter  

        re_val = CAN_REG_READ(rfsts0[rfi_number].UINT32,CAN_REG_BIT3,0x1U);
        tiny_printf("--RFIF:0x%02X",re_val); //RFIF  Receive FIFO Interrupt Request Flag
        tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:No receive FIFO interrupt request") : ("1:A receive FIFO interrupt request") );                   
        if (re_val) //To clear the RFMLT or RFIF flag to 0, use a store instruction to write “0” to the given flag and “1” to the other flags.
        {
            rfsts0[rfi_number].UINT32 = 0xFFFFFFF7U;
        }

        re_val = CAN_REG_READ(rfsts0[rfi_number].UINT32,CAN_REG_BIT2,0x1U);
        tiny_printf("--RFMLT:0x%02X",re_val); //RFMLT  Receive FIFO Message Lost Flag
        tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:No receive FIFO message is lost") : ("1:A receive FIFO message is lost") );               
        if (re_val) //To clear the RFMLT or RFIF flag to 0, use a store instruction to write “0” to the given flag and “1” to the other flags.
        {
            rfsts0[rfi_number].UINT32 = 0xFFFFFFFBU;
        }

        re_val = CAN_REG_READ(rfsts0[rfi_number].UINT32,CAN_REG_BIT1,0x1U);
        tiny_printf("--RFFLL:0x%02X",re_val); //RFFLL  Receive FIFO Buffer Full Status Flag
        tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:The receive FIFO buffer is not full") : ("1:The receive FIFO buffer is full") ); 

        re_val = CAN_REG_READ(rfsts0[rfi_number].UINT32,CAN_REG_BIT0,0x1U);
        tiny_printf("--RFEMP:0x%02X",re_val); //RFEMP  Receive FIFO Buffer Empty Status Flag
        tiny_printf("[%s]\r\n", (re_val == 0 ) ? ("0:receive FIFO buffer contains unread message.") : ("1:receive FIFO buffer contains no unread message") );     

        rfpctr[rfi_number].UINT8[0]=0xFF;

        tiny_printf("\r\n"); 
    
        // CAN_REG_CLR(can->CFDRMND0.UINT32,CAN_REG_BIT0,CAN_REG_LENGTH_1);
    }
}

void can_fd_loop_process(void)
{
    unsigned int transmit_buffer_id = 0;
    unsigned int p_number=0;    
    unsigned char txbuffer[64] = {0};
    static unsigned char count = 0;

    // unsigned int ary_data[8] = {0};
    // unsigned int ary_dlc[64] = {0};

    uint8_t i = 0;
    uint8_t len = 0;

    #if defined (CAN_RX_POLLING)
    can_fd_receive_buffer_decode(&RCFDC0,can_bus_parameter_ch1.CAN_RX_FIFO_BUFFER_NUM);
    #if defined (ENABLE_MULTI_CAN_CH)
    can_fd_receive_buffer_decode(&RCFDC0,can_bus_parameter_ch4.CAN_RX_FIFO_BUFFER_NUM);
    #endif
    #endif

    
	if(can_bus_variable_ch4.can_tx4_flag==1)
	{
		can_bus_variable_ch4.can_tx4_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x408;
        p_number = 7;
        len = 64;

        for(i = 0; i < len ;i++)
        {
            txbuffer[i] = 0x40+i+count;
        }
        count += 0x10;
        can_tx2_normal_buffer_set(&can_bus_handle_ch4,can_bus_parameter_ch4.CAN_MODE,p_number,transmit_buffer_id,txbuffer,len);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch4.tx_handle[0],can_bus_parameter_ch4.CAN_CH,1,len);

        tiny_printf("CAN2 TX2 xfer finish\r\n");
	}

    
	if(can_bus_variable_ch4.can_tx3_flag==1)
	{
		can_bus_variable_ch4.can_tx3_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x407;
        p_number = 6;
        len = 48;

        for(i = 0; i < len ;i++)
        {
            txbuffer[i] = 0x30+i+count;
        }
        count += 0x10;
        can_tx2_normal_buffer_set(&can_bus_handle_ch4,can_bus_parameter_ch4.CAN_MODE,p_number,transmit_buffer_id,txbuffer,len);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch4.tx_handle[0],can_bus_parameter_ch4.CAN_CH,1,len);

        tiny_printf("CAN2 TX3 xfer finish\r\n");
	}
    
	if(can_bus_variable_ch4.can_tx2_flag==1)
	{
		can_bus_variable_ch4.can_tx2_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x406;
        p_number = 5;
        len = 16;

        for(i = 0; i < len ;i++)
        {
            txbuffer[i] = 0x20+i+count;
        }
        count += 0x10;
        can_tx2_normal_buffer_set(&can_bus_handle_ch4,can_bus_parameter_ch4.CAN_MODE,p_number,transmit_buffer_id,txbuffer,len);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch4.tx_handle[0],can_bus_parameter_ch4.CAN_CH,1,len);

        tiny_printf("CAN2 TX2 xfer finish\r\n");
	}

	if(can_bus_variable_ch4.can_tx1_flag==1)
	{
		can_bus_variable_ch4.can_tx1_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x405;
        p_number = 4;
        len = 8;

        for(i = 0; i < len ;i++)
        {
            txbuffer[i] = 0x10+i+count;
        }
        count += 0x10;
        can_tx2_normal_buffer_set(&can_bus_handle_ch4,can_bus_parameter_ch4.CAN_MODE,p_number,transmit_buffer_id,txbuffer,len);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch4.tx_handle[0],can_bus_parameter_ch4.CAN_CH,1,len);

        tiny_printf("CAN2 TX1 xfer finish\r\n");
	}

	if(can_bus_variable_ch1.can_tx4_flag==1)
	{
		can_bus_variable_ch1.can_tx4_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x208;
        p_number = 3;

        for(i = 0; i < 64 ;i++)
        {
            txbuffer[i] = 0x10+i+count;
        }
        count += 0x10;
        can_tx_normal_buffer4_set(&can_bus_handle_ch1,can_bus_parameter_ch1.CAN_MODE,p_number,transmit_buffer_id,txbuffer);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch1.tx_handle[0],can_bus_parameter_ch1.CAN_CH,1,64);

        tiny_printf("CAN TX4 xfer finish\r\n");
	}

	if(can_bus_variable_ch1.can_tx3_flag==1)
	{
		can_bus_variable_ch1.can_tx3_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x207;
        p_number = 2;

        for(i = 0; i < 16 ;i++)
        {
            txbuffer[i] = 0x30+i+count;
        }
        count += 0x10;
        can_tx_normal_buffer3_set(&can_bus_handle_ch1,can_bus_parameter_ch1.CAN_MODE,p_number,transmit_buffer_id,txbuffer);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch1.tx_handle[0],can_bus_parameter_ch1.CAN_CH,1,16);

        tiny_printf("CAN TX3 xfer finish\r\n");
	}

	if(can_bus_variable_ch1.can_tx2_flag==1)
	{
		can_bus_variable_ch1.can_tx2_flag=0;          
        
        // prepare data
        transmit_buffer_id = 0x206;
        p_number = 1;

        can_tx_normal_buffer2_set(&can_bus_handle_ch1,can_bus_parameter_ch1.CAN_MODE,p_number,transmit_buffer_id);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch1.tx_handle[0],can_bus_parameter_ch1.CAN_CH,1,32);

        tiny_printf("CAN TX2 xfer finish\r\n");
	}

	if(can_bus_variable_ch1.can_tx1_flag==1)
	{
		can_bus_variable_ch1.can_tx1_flag=0;        
        
        // prepare data
        transmit_buffer_id = 0x205;
        p_number = 0;

        can_tx_normal_buffer1_set(&can_bus_handle_ch1,can_bus_parameter_ch1.CAN_MODE,p_number,transmit_buffer_id);
		can_tx_buf_data_multi_send(&RCFDC0,&can_bus_handle_ch1.tx_handle[0],can_bus_parameter_ch1.CAN_CH,1,64);

        tiny_printf("CAN TX1 xfer finish\r\n");
	}

    if (can_bus_variable_ch1.can_rx_flag)
    {    


        can_bus_variable_ch1.can_rx_flag = 0;
    }
}



