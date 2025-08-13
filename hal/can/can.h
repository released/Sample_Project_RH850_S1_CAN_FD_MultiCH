#define CAN_REG_TYP_HANDLE

#include "iodefine.h"
#include "can_user.h"

#define ENABLE_MULTI_CAN_CH
// #define ENABLE_TDC

// #define CAN_USE_RX_RULE
#define CAN_RX_POLLING
// #define CAN_RX_INTERRUPT

#define CAN_FIFO_BUF_CHECK

//#define	CAN_FD_MIX_MODE		0
//#define	CAN_FD_ONLY_MODE	1
//#define	CAN_STANDARD_MODE	2

#define TX_BUF_SEND_CLASSIC_CAN_FRAME	0
#define TX_BUF_SEND_CAN_FD_FRAME		1

#define TX_BUF_BIT_RATE_NO_CHANGE		0
#define TX_BUF_BIT_RATE_CHANGE			1

#define RX_RRT0_ID		0x100
#define RX_RRT1_ID		0x101
#define RX_RRT2_ID		0x102
#define RX_RRT3_ID		0x103
#define RX_RRT4_ID		0x104
#define RX_RRT5_ID		0x105
#define RX_RRT6_ID		0x106
#define RX_RRT7_ID		0x107
#define RX_RRT8_ID		0x108
#define RX_RRT9_ID		0x109

#define RX2_RRT0_ID		0x200
#define RX2_RRT1_ID		0x201
#define RX2_RRT2_ID		0x202
#define RX2_RRT3_ID		0x203

typedef volatile struct __tag445 CAN_REG_TYP;

typedef enum
{
	CAN_FD_MIX_MODE=0,
	CAN_FD_ONLY_MODE=1,
	CAN_STANDARD_MODE=2
}CAN_FD_MODE_e;

typedef enum
{
    CAN_DCS_CLKC=0,
    CAN_DCS_CLK_XINCAN
}CAN_DCS_CLK_SEL_e;

typedef enum
{
    CAN_CHANNEL_0=0,
    CAN_CHANNEL_1,
    CAN_CHANNEL_2,
    CAN_CHANNEL_3,
    CAN_CHANNEL_4,
    CAN_CHANNEL_5
}CAN_CHANNEL_SEL_e;

//=========================================

// DSJW[2:0] Bits , Possible values are 1 to 8 Tq
typedef enum
{
    CAN_DSJW_1TQ=0,
    CAN_DSJW_2TQ,
    CAN_DSJW_3TQ,
    CAN_DSJW_4TQ,
    CAN_DSJW_5TQ,
    CAN_DSJW_6TQ,
    CAN_DSJW_7TQ,
    CAN_DSJW_8TQ
}CAN_BIT_TIME_DSJW_SEL_e;

// DTSEG2[2:0] Bits , Possible values are 2 to 8 Tq.
typedef enum
{
    CAN_DTSEG2_2TQ=1,
    CAN_DTSEG2_3TQ=2,
    CAN_DTSEG2_4TQ=3,
    CAN_DTSEG2_5TQ=4,
    CAN_DTSEG2_6TQ=5,
    CAN_DTSEG2_7TQ=6,
    CAN_DTSEG2_8TQ=7
}CAN_BIT_TIME_DTSEG2_SEL_e;

// DTSEG1[3:0] Bits , Possible values are 2 to 16 Tq.
typedef enum
{
    CAN_DTSEG1_2TQ=1,
    CAN_DTSEG1_3TQ=2,
    CAN_DTSEG1_4TQ=3,
    CAN_DTSEG1_5TQ=4,
    CAN_DTSEG1_6TQ=5,
    CAN_DTSEG1_7TQ=6,
    CAN_DTSEG1_8TQ=7,
    CAN_DTSEG1_9TQ=8,
    CAN_DTSEG1_10TQ=9,
    CAN_DTSEG1_11TQ=10,
    CAN_DTSEG1_12TQ=11,
    CAN_DTSEG1_13TQ=12,
    CAN_DTSEG1_14TQ=13,
    CAN_DTSEG1_15TQ=14,
    CAN_DTSEG1_16TQ=15,
}CAN_BIT_TIME_DTSEG1_SEL_e;

typedef enum
{
    CAN_DBRP_1=0,
    CAN_DBRP_2=1,
    CAN_DBRP_3=2,
    CAN_DBRP_4=3,
    CAN_DBRP_5=4,
    CAN_DBRP_6=5,
    CAN_DBRP_7=6,
    CAN_DBRP_8=7,
    CAN_DBRP_9=8,
    CAN_DBRP_10=9,
    CAN_DBRP_20=19,
    
    CAN_DBRP_11=10,
    CAN_DBRP_51=50,
    CAN_DBRP_256=255
}CAN_BIT_TIME_DBRP_SEL_e;

// NSJW[4:0] Bits ,  Possible values are 1 to 32 Tq
typedef enum
{
    CAN_NSJW_1TQ=0,
    CAN_NSJW_2TQ,
    CAN_NSJW_3TQ,
    CAN_NSJW_4TQ,
    CAN_NSJW_5TQ,
    CAN_NSJW_6TQ,
    CAN_NSJW_7TQ,
    CAN_NSJW_8TQ,

    CAN_NSJW_9TQ,
    CAN_NSJW_10TQ,
    CAN_NSJW_11TQ,
    CAN_NSJW_12TQ,
    CAN_NSJW_13TQ,
    CAN_NSJW_14TQ,
    CAN_NSJW_15TQ,
    CAN_NSJW_16TQ,

    CAN_NSJW_17TQ,
    CAN_NSJW_18TQ,
    CAN_NSJW_19TQ,
    CAN_NSJW_20TQ,
    CAN_NSJW_21TQ,
    CAN_NSJW_22TQ,
    CAN_NSJW_23TQ,
    CAN_NSJW_24TQ,

    CAN_NSJW_25TQ,
    CAN_NSJW_26TQ,
    CAN_NSJW_27TQ,
    CAN_NSJW_28TQ,
    CAN_NSJW_29TQ,
    CAN_NSJW_30TQ,
    CAN_NSJW_31TQ,
    CAN_NSJW_32TQ,

}CAN_BIT_TIME_NSJW_SEL_e;

// NTSEG2[4:0] Bits , Possible values are 2 Tq to 32 Tq,
typedef enum
{
    CAN_NTSEG2_2TQ=1,
    CAN_NTSEG2_3TQ=2,
    CAN_NTSEG2_4TQ=3,
    CAN_NTSEG2_5TQ=4,
    CAN_NTSEG2_6TQ=5,
    CAN_NTSEG2_7TQ=6,
    CAN_NTSEG2_8TQ=7,
    CAN_NTSEG2_9TQ=8,
    CAN_NTSEG2_10TQ=9,
    CAN_NTSEG2_11TQ=10,
    CAN_NTSEG2_12TQ=11,
    CAN_NTSEG2_13TQ=12,
    CAN_NTSEG2_14TQ=13,
    CAN_NTSEG2_15TQ=14,
    CAN_NTSEG2_16TQ=15,
    CAN_NTSEG2_17TQ=16,
    CAN_NTSEG2_18TQ=17,
    CAN_NTSEG2_19TQ=18,
    CAN_NTSEG2_20TQ=19,
    CAN_NTSEG2_21TQ=20,
    CAN_NTSEG2_22TQ=21,
    CAN_NTSEG2_23TQ=22,
    CAN_NTSEG2_24TQ=23,
    CAN_NTSEG2_25TQ=24,
    CAN_NTSEG2_26TQ=25,
    CAN_NTSEG2_27TQ=26,
    CAN_NTSEG2_28TQ=27,
    CAN_NTSEG2_29TQ=28,
    CAN_NTSEG2_30TQ=29,
    CAN_NTSEG2_31TQ=30,
    CAN_NTSEG2_32TQ=31
}CAN_BIT_TIME_NTSEG2_SEL_e;

// NTSEG1[6:0] Bits , Possible values are 4 to 128 Tq. 
typedef enum
{
    CAN_NTSEG1_4TQ=3,
    CAN_NTSEG1_5TQ=4,
    CAN_NTSEG1_6TQ=5,
    CAN_NTSEG1_7TQ=6,
    CAN_NTSEG1_8TQ=7,
    CAN_NTSEG1_9TQ=8,
    CAN_NTSEG1_10TQ=9,
    CAN_NTSEG1_11TQ=10,
    CAN_NTSEG1_12TQ=11,
    CAN_NTSEG1_13TQ=12,
    CAN_NTSEG1_14TQ=13,
    CAN_NTSEG1_15TQ=14,
    CAN_NTSEG1_16TQ=15,
    CAN_NTSEG1_17TQ=16,
    CAN_NTSEG1_18TQ=17,
    CAN_NTSEG1_19TQ=18,
    CAN_NTSEG1_20TQ=19,
    CAN_NTSEG1_21TQ=20,
    CAN_NTSEG1_22TQ=21,
    CAN_NTSEG1_23TQ=22,
    CAN_NTSEG1_24TQ=23,
    CAN_NTSEG1_25TQ=24,
    CAN_NTSEG1_26TQ=25,
    CAN_NTSEG1_27TQ=26,
    CAN_NTSEG1_28TQ=27,
    CAN_NTSEG1_29TQ=28,
    CAN_NTSEG1_30TQ=29,
    CAN_NTSEG1_31TQ=30,
    CAN_NTSEG1_32TQ=31,

    CAN_NTSEG1_59TQ=58,

    CAN_NTSEG1_63TQ=62,
    CAN_NTSEG1_128TQ=127,

}CAN_BIT_TIME_NTSEG1_SEL_e;

typedef enum
{
    CAN_NBRP_1=0,
    CAN_NBRP_2=1,
    CAN_NBRP_3=2,
    CAN_NBRP_4=3,
    CAN_NBRP_5=4,
    CAN_NBRP_6=5,
    CAN_NBRP_7=6,
    CAN_NBRP_8=7,
    CAN_NBRP_9=8,
    CAN_NBRP_10=9,
    CAN_NBRP_20=19,
    CAN_NBRP_11=10,
    CAN_NBRP_51=50,
    CAN_NBRP_1024=1023
}CAN_BIT_TIME_NBRP_SEL_e;

typedef enum
{
	CAN_DLC_DISABLE_CHECK=0,
	CAN_DLC_1BYTE=1,
	CAN_DLC_2BYTE=2,
	CAN_DLC_3BYTE=3,
	CAN_DLC_4BYTE=4,
	CAN_DLC_5BYTE=5,
	CAN_DLC_6BYTE=6,
	CAN_DLC_7BYTE=7,
	CAN_DLC_8BYTE=8,
	CAN_DLC_12BYTE=9,
	CAN_DLC_16BYTE=10,
	CAN_DLC_20BYTE=11,
	CAN_DLC_24BYTE=12,
	CAN_DLC_32BYTE=13,
	CAN_DLC_48BYTE=14,
	CAN_DLC_64BYTE=15,
}CAN_DLC_SEL_e;

//=========================================
typedef enum
{
    RX_STANDARD_ID=0,
    RX_EXTEND_ID
}CAN_RRT_ID_SET_e;

typedef enum
{
    RX_DATA_FRAME=0,
    RX_REMOTE_FRAME
}CAN_RRT_FRAME_SET_e;

typedef enum
{
    TX_STANDARD_ID=0,
    TX_EXTEND_ID
}CAN_TX_ID_SET_e;

typedef enum
{
    TX_DATA_FRAME=0,
    TX_REMOTE_FRAME
}CAN_TX_FRAME_SET_e;

typedef enum
{
    TX_DATA_HISTORY_NOT_STORE=0,
    TX_DATA_HISTORY_STORE
}CAN_TX_HISTROY_DATA_STORE_SET_e;

typedef enum
{
    TX_DATA_LENGTH_0 =0,
    TX_DATA_LENGTH_1 =1,
    TX_DATA_LENGTH_2 =2,
    TX_DATA_LENGTH_3 =3,
    TX_DATA_LENGTH_4 =4,
    TX_DATA_LENGTH_5 =5,
    TX_DATA_LENGTH_6 =6,
    TX_DATA_LENGTH_7 =7,
    TX_DATA_LENGTH_8 =8
}CAN_TX_DLC_SET_e;



typedef enum
{
    CAN_RX_RULE_TABLE_BUFFER_NUMBER0=0,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER1=1,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER2=2,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER3=3,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER4=4,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER5=5,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER6=6,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER7=7,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER8=8,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER9=9,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER10=10,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER11=11,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER12=12,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER13=13,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER14=14,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER15=15,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER16=16,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER17=17,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER18=18,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER19=19,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER20=20,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER21=21,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER22=22,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER23=23,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER24=24,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER25=25,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER26=26,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER27=27,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER28=28,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER29=29,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER30=30,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER31=31,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER32=32,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER33=33,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER34=34,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER35=35,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER36=36,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER37=37,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER38=38,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER39=39,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER40=40,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER41=41,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER42=42,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER43=43,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER44=44,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER45=45,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER46=46,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER47=47,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER48=48,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER49=49,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER50=50,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER51=51,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER52=52,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER53=53,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER54=54,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER55=55,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER56=56,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER57=57,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER58=58,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER59=59,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER60=60,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER61=61,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER62=62,
    CAN_RX_RULE_TABLE_BUFFER_NUMBER63=63,
}CAN_RRT_BUFFER_SET_e;


typedef enum
{
    CAN_RX_RULE_TABLE_PAGE0=0,
    CAN_RX_RULE_TABLE_PAGE1=1,
    CAN_RX_RULE_TABLE_PAGE2=2,
    CAN_RX_RULE_TABLE_PAGE3=3,
    CAN_RX_RULE_TABLE_PAGE4=4,
    CAN_RX_RULE_TABLE_PAGE5=5,
    CAN_RX_RULE_TABLE_PAGE6=6,
    CAN_RX_RULE_TABLE_PAGE7=7,
    CAN_RX_RULE_TABLE_PAGE8=8,
    CAN_RX_RULE_TABLE_PAGE9=9,
    CAN_RX_RULE_TABLE_PAGE10=10,
    CAN_RX_RULE_TABLE_PAGE11=11,
    CAN_RX_RULE_TABLE_PAGE12=12,
    CAN_RX_RULE_TABLE_PAGE13=13,
    CAN_RX_RULE_TABLE_PAGE14=14,
    CAN_RX_RULE_TABLE_PAGE15=15,
    CAN_RX_RULE_TABLE_PAGE63=63,
}CAN_RRT_PAGE_SET_e;

//=============================================
typedef enum
{
    CAN_RX_FIFO_BUFFER_DISABLE=0,
    CAN_RX_FIFO_BUFFER_ENABLE=1,
}CAN_RX_FIFO_BUFER_EN_e;

typedef enum
{
    CAN_RX_FIFO_BUFFER_INIERRUPT_DISABLE=0,
    CAN_RX_FIFO_BUFFER_INIERRUPT_ENABLE=1,
}CAN_RX_FIFO_BUFER_INT_EN_e;


typedef enum
{
    CAN_RX_FIFO_BUFFER_NUMBER0=0,
    CAN_RX_FIFO_BUFFER_NUMBER1=1,
    CAN_RX_FIFO_BUFFER_NUMBER2=2,
    CAN_RX_FIFO_BUFFER_NUMBER3=3,
    CAN_RX_FIFO_BUFFER_NUMBER4=4,
    CAN_RX_FIFO_BUFFER_NUMBER5=5,
    CAN_RX_FIFO_BUFFER_NUMBER6=6,
    CAN_RX_FIFO_BUFFER_NUMBER7=7
}CAN_RX_FIFO_BUFER_NUMBER_e;

typedef enum
{
    CAN_RX_FIFO_BUFFER_DEPTH0=0,
    CAN_RX_FIFO_BUFFER_DEPTH4=1,
    CAN_RX_FIFO_BUFFER_DEPTH8=2,
    CAN_RX_FIFO_BUFFER_DEPTH16=3,
    CAN_RX_FIFO_BUFFER_DEPTH32=4,
    CAN_RX_FIFO_BUFFER_DEPTH48=5,
    CAN_RX_FIFO_BUFFER_DEPTH64=6,
    CAN_RX_FIFO_BUFFER_DEPTH128=7
}CAN_RX_FIFO_BUFER_DEPTH_e;

typedef enum
{
    CAN_RX_FIFO_BUFFER_PAYLOAD_8bytes=0,
    CAN_RX_FIFO_BUFFER_PAYLOAD_12bytes=1,
    CAN_RX_FIFO_BUFFER_PAYLOAD_16bytes=2,
    CAN_RX_FIFO_BUFFER_PAYLOAD_20bytes=3,
    CAN_RX_FIFO_BUFFER_PAYLOAD_24bytes=4,
    CAN_RX_FIFO_BUFFER_PAYLOAD_32bytes=5,
    CAN_RX_FIFO_BUFFER_PAYLOAD_48bytes=6,
    CAN_RX_FIFO_BUFFER_PAYLOAD_64bytes=7
}CAN_RX_FIFO_BUFER_PAYLOAD_e;

typedef enum
{
    CAN_RX_FIFO_BUFFER_FIFO_INT_1D8=0,
    CAN_RX_FIFO_BUFFER_FIFO_INT_2D8=1,
    CAN_RX_FIFO_BUFFER_FIFO_INT_3D8=2,
    CAN_RX_FIFO_BUFFER_FIFO_INT_4D8=3,
    CAN_RX_FIFO_BUFFER_FIFO_INT_5D8=4,
    CAN_RX_FIFO_BUFFER_FIFO_INT_6D8=5,
    CAN_RX_FIFO_BUFFER_FIFO_INT_7D8=6,
    CAN_RX_FIFO_BUFFER_FIFO_INT_FULL=7

}CAN_RX_FIFO_BUFER_INT_TIMING_e;
//===============================================

typedef struct _can_bus_global_variable_t
{
    unsigned char can_tx1_flag;
    unsigned char can_tx2_flag;
    unsigned char can_tx3_flag;
    unsigned char can_tx4_flag;
    unsigned char can_rx_flag;
    unsigned char can_rx_polling_flag;
    unsigned int can_rx_fifo_cnt;
    unsigned short can_rx_isr_cnt;

}CAN_BUS_GLOBAL_VARIABLE_T;

typedef struct _can_bus_parameter_t
{
    CAN_CHANNEL_SEL_e CAN_CH;
    CAN_FD_MODE_e CAN_MODE;
    const CAN_RX_FIFO_BUFER_NUMBER_e CAN_RX_FIFO_BUFFER_NUM;

    unsigned short NBRP;        // NBRP[9:0] Bits 
    unsigned char NTSEG1;       // NTSEG1[6:0] Bits
    unsigned char NTSEG2;       // NTSEG2[4:0] Bits
    unsigned char NSJW;         // NSJW[4:0] Bits

    unsigned char DBRP;         // DBRP[7:0] Bits 
    unsigned char DTSEG1;       // DTSEG1[3:0] Bits
    unsigned char DTSEG2;       // DTSEG2[2:0] Bits
    unsigned char DSJW;         // DSJW[2:0] Bits

}CAN_BUS_PARAMETER_T;

typedef struct _can_bus_rx_fifo_flag_t
{
    unsigned char RxBuffer0;
    unsigned char RxBuffer1;
    unsigned char RxBuffer2;
    unsigned char RxBuffer3;
    unsigned char RxBuffer4;
    unsigned char RxBuffer5;
    unsigned char RxBuffer6;
    unsigned char RxBuffer7;
}CAN_BUS_RX_FIFO_FLAG_T;

enum
{
	OTHER_NODE_TRANSMIT_MSG_RECEIVED=0,
    OWN_TRANSMIT_MSG_RECEIVED
};

enum
{
	RTR_BIT_NOT_COMPARED=0,
    RTR_BIT_IS_COMPARED
};
enum
{
	IDE_BIT_NOT_COMPARED=0,
    IDE_BIT_IS_COMPARED
};
enum
{
	NO_RECEIVE_BUFFER_IS_USED=0,
    A_RECEIVE_BUFFER_IS_USED
};

#define TRANSMIT_RECEIVE_FIFO_BUFFER_IS_NOT_SELECTED    (0U)

#define ALL_ID_BIT_IS_NOT_COMPARED                      (0U)
#define STANDARD_ID_BIT_IS_COMPARED                     (0xFFFU)
#define EXTEND_ID_BIT_IS_COMPARED                       (0x1FFFFFFFU)

#define RESERVED_BIT                                    (0U)
typedef struct _can_rx_rule_table_t
{
    /*****************************/
    //RCFDCnCFDGAFLIDj
    /*
        ID 
        Set the ID of the receive rule. 
        For the standard ID, set the ID in bits b10 to b0 and set bits b28 to b11 to 0.     
    */
    unsigned long GAFLID:29;

    /*
        Receive Rule Target Message Select 
        0: When a message transmitted from another CAN node is received 
        1: When the own transmitted message is received     
    */
    unsigned char GAFLLB:1;

    /*
        RTR Select 
        0: Data frame 
        1: Remote frame     
    */
    unsigned char GAFLRTR:1;

    /*
        IDE Select 
        0: Standard ID 
        1: Extended ID      
    */
    unsigned char GAFLIDE:1;

    /*****************************/
    //RCFDCnCFDGAFLMj
    /*
        ID Mask 
        0: The corresponding ID bit is not compared. 
        1: The corresponding ID bit is compared.     
    */
    unsigned long GAFLIDM:29;
    unsigned char Reserved0:1;

    /*
        RTR Mask 
        0: The RTR bit is not compared. 
        1: The RTR bit is compared.     
    */
    unsigned char GAFLRTRM:1;

    /*
        IDE Mask 
        0: The IDE bit is not compared. 
        1: The IDE bit is compared.     
    */
    unsigned char GAFLIDEM:1;

    /*****************************/
    //RCFDCnCFDGAFLP0_j
    
    /*
        GAFLDLC[3:0]  Receive Rule DLC 

        b3  b2  b1  b0      Classical CAN Frame         CAN FD Frame 
        0   0   0   0       DLC check is disabled   
        0   0   0   1       1 data byte 
        0   0   1   0       2 data bytes 
        0   0   1   1       3 data bytes 
        0   1   0   0       4 data bytes 
        0   1   0   1       5 data bytes 
        0   1   1   0       6 data bytes 
        0   1   1   1       7 data bytes 
        1   0   0   0       8 data bytes 
        1   0   0   1       8 data bytes                12 data bytes 
        1   0   1   0                                   16 data bytes 
        1   0   1   1                                   20 data bytes 
        1   1   0   0                                   24 data bytes 
        1   1   0   1                                   32 data bytes 
        1   1   1   0                                   48 data bytes 
        1   1   1   1                                   64 data bytes 

    */
    unsigned char GAFLDLC:4;

    unsigned char Reserved1:4;

    /*
        Receive Buffer Number Select 
        Set the receive buffer number to store receive messages.     
    */
    unsigned char GAFLRMDP:7;

    /*
        Receive Buffer Enable 
        0: No receive buffer is used. 
        1: A receive buffer is used.     
    */
    unsigned char GAFLRMV:1;

    /*
        Receive Rule Label 
        Set the 16-bit label information.     
    */
    unsigned short GAFLPTR:16;

    /*****************************/
    //RCFDCnCFDGAFLP1_j
    /*
        Receive FIFO Buffer x Select 
        (Bit position = target receive FIFO buffer number x) 
        0: Receive FIFO buffer is not selected. 
        1: Receive FIFO buffer is selected.     
    */
    unsigned short GAFLFDP_x:16;

    /*
        Transmit/Receive FIFO Buffer k Select 
        (Bit position –8 = target transmit/receive FIFO buffer number k) 
        0: Transmit/receive FIFO buffer is not selected. 
        1: Transmit/receive FIFO buffer is selected.     
    */    
    unsigned short GAFLFDP_k:16;

}CAN_RX_RULE_TABLE_T;

//===============================================
typedef enum
{
	CAN_CH_COMM_MODE = 0,
	CAN_CH_RESET_MODE = 1,
	CAN_CH_HALT_MODE = 2,
	CAN_SETTING_PROHIBITED = 3,

}CAN_CH_MODE_SEL_e;

//===============================================
typedef enum
{
	CAN_COMM_STANDARD_TEST_MODE = 0,
	CAN_COMM_LISTON_ONLY_MODE,
	CAN_COMM_SELF_TEST_MODE_0,          //external loopback mode
	CAN_COMM_SELF_TEST_MODE_1,          //internal loopback mode

}CAN_COMM_TEST_MODE_SEL_e;

//===============================================

#define R_CLKC_RSCAN_DISABLE        0x00
#define R_CLKC_RSCAN_MOSC           0x01
#define R_CLKC_RSCAN_PPLLCLK        0x03
#define R_CLKC_RSCAN_DIV_DISABLE    0x00
#define R_CLKC_RSCAN_DIV_MOSC1      0x01
#define R_CLKC_RSCAN_DIV_MOSC2      0x02

extern CAN_BUS_HANDLE can_bus_handle_ch1;
extern volatile CAN_BUS_GLOBAL_VARIABLE_T can_bus_variable_ch1;
extern const CAN_BUS_PARAMETER_T can_bus_parameter_ch1;

extern CAN_BUS_HANDLE can_bus_handle_ch4;
extern volatile CAN_BUS_GLOBAL_VARIABLE_T can_bus_variable_ch4;
extern const CAN_BUS_PARAMETER_T can_bus_parameter_ch4;

void can_self_test_mode(CAN_REG_TYP * can , unsigned char en);

void R_CANFD_Global_error_Interrupt_Init(void);

extern void R_CANFD_Deinit(void);
extern void R_CANFD_Init(void);

extern void can_rx_interrupt_cbk(void);

extern void can_init(void);

// extern void can_tx_buf_data_send(CAN_REG_TYP * can,
//                                  CAN_BUS_HANDLE *p,
//                                  unsigned int p_number);
                                 
// extern void can_tx_buf_data_single_send(CAN_REG_TYP * can,
//                                         CAN_BUS_HANDLE *p,
//                                         unsigned int p_number);
 
extern void can_tx_buf_data_multi_send(CAN_REG_TYP * can,
                                        CAN_BUS_TX_INFO_HANDLE *p,
                                        CAN_CHANNEL_SEL_e channel,
										unsigned int tx_buf_cnt_max,
										unsigned int tx_buf_data_length_max);
                                        
extern void can_rx_fifo_buf_int_check(CAN_REG_TYP * can,
                                        CAN_BUS_HANDLE *p,
                                        CAN_RX_FIFO_BUFER_NUMBER_e rfi_number);

void can_reg_dump_log(void);

unsigned char can_global_error_interrupt_cbk(CAN_REG_TYP * can);
void can_rx_buffer_set(CAN_REG_TYP * can);

signed char can_fd_receive_buffer_decode(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number);
void can_fd_receive_fifo_buffer_decode(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number);
void can_fd_receive_fifo_buffer_status(CAN_REG_TYP * can, CAN_RX_FIFO_BUFER_NUMBER_e rfi_number);

void can_fd_loop_process(void);


#undef CAN_REG_TYP_HANDLE
