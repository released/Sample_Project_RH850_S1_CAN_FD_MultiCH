
#define PTR_UL_ADDR_RW(a)       *((unsigned long*)(a))
#define PTR_UCH_ADDR_RW(a)      *((unsigned char*)(a))

#define r_CAN_TMC(can_base,p_number)    (can_base+0x0250+0x01*p_number)     //8bits

#define r_CAN_TMSTS(can_base,p_number)  (can_base+0x02D0+0x01*p_number)     //8bits
#define r_CAN_TMID(can_base,p_number)   (can_base+0x1000+0x10*p_number)     //32bits

#define r_CAN_TMPTR(can_base,p_number)  (can_base+0x1004+0x10*p_number)     //32bits
#define r_CAN_TMDF0(can_base,p_number)  (can_base+0x1008+0x10*p_number)     //32bits
#define r_CAN_TMDF1(can_base,p_number)  (can_base+0x100C+0x10*p_number)     //32bits


#define r_CAN_RMID(can_base,q_number)   (can_base+0x0600+0x10*q_number)
#define r_CAN_RMPTR(can_base,q_number)  (can_base+0x0604+0x10*q_number)
#define r_CAN_RDF0(can_base,q_number)   (can_base+0x0608+0x10*q_number)
#define r_CAN_RDF1(can_base,q_number)   (can_base+0x060C+0x10*q_number)


#define  CAN_REG_BIT0  0
#define  CAN_REG_BIT1  1
#define  CAN_REG_BIT2  2
#define  CAN_REG_BIT3  3
#define  CAN_REG_BIT4  4
#define  CAN_REG_BIT5  5
#define  CAN_REG_BIT6  6
#define  CAN_REG_BIT7  7
#define  CAN_REG_BIT8  8
#define  CAN_REG_BIT9  9
#define  CAN_REG_BIT10  10
#define  CAN_REG_BIT11  11
#define  CAN_REG_BIT12  12
#define  CAN_REG_BIT13  13
#define  CAN_REG_BIT14  14
#define  CAN_REG_BIT15  15
#define  CAN_REG_BIT16  16
#define  CAN_REG_BIT17  17
#define  CAN_REG_BIT18  18
#define  CAN_REG_BIT19  19
#define  CAN_REG_BIT20  20
#define  CAN_REG_BIT21  21
#define  CAN_REG_BIT22  22
#define  CAN_REG_BIT23  23
#define  CAN_REG_BIT24  24
#define  CAN_REG_BIT25  25
#define  CAN_REG_BIT26  26
#define  CAN_REG_BIT27  27
#define  CAN_REG_BIT28  28
#define  CAN_REG_BIT29  29
#define  CAN_REG_BIT30  30
#define  CAN_REG_BIT31  31

#define  CAN_REG_LENGTH_1 0x1
#define  CAN_REG_LENGTH_2 0x3
#define  CAN_REG_LENGTH_3 0x7
#define  CAN_REG_LENGTH_4 0xF
#define  CAN_REG_LENGTH_5 0x1F
#define  CAN_REG_LENGTH_6 0x3F
#define  CAN_REG_LENGTH_7 0x7F
#define  CAN_REG_LENGTH_8 0xFF


#define CAN_REG_SET(r,n,val) (r |= (((unsigned long)val)<<n))
#define CAN_REG_CLR(r,n,val) (r &= (0xFFFFFFFF -(val<<n)))

#define CAN_REG_READ(r,n,val) ((r &(val<<n))>>n)

#define RS_CANFD_REG32(addr)            (*(volatile uint32_t *)(addr))
#define RS_CANFD_REG32_CLR(addr,b)      (((*(volatile uint32_t *)(addr))) &= ~(1UL << (b)))

typedef struct
{
	unsigned long ID:29;
	unsigned char LB  :1;
	unsigned char RTR :1;
	unsigned char IDE :1;
}ID_BIT_FIELID;

typedef union
{
	unsigned long all;
	ID_BIT_FIELID bit;
}ID_MEMBER;

//===============================
typedef struct
{
	unsigned long MID:29;
	unsigned char rsvd:1;
	unsigned char MRTR :1;
	unsigned char MIDE :1;

}MASK_BIT_FIELID;

typedef union
{
	unsigned long all;
	MASK_BIT_FIELID bit;
}MASK_MEMBER;

//===============================
typedef struct
{
    unsigned char DLC :4;
    unsigned char rsvd :4;
    unsigned char RMDP :7;
    unsigned char RMV :1;
    unsigned int  PTR :16;
}PTR0_BIT_FIELID;

typedef union
{
	unsigned long all;
	PTR0_BIT_FIELID bit;
}PTR0_MEMBER;
//===============================
typedef struct
{
    unsigned int  FDPx  :8;
    unsigned int  FDPk  :24;
}PTR1_BIT_FIELID;

typedef union
{
	unsigned long all;
	PTR1_BIT_FIELID bit;
}PTR1_MEMBER;

//===============================
typedef struct
{
  ID_MEMBER id;
  MASK_MEMBER mask;
  PTR0_MEMBER ptr0;
  PTR1_MEMBER ptr1;
}CAN_BUS_RX_INFO_HANDLE;

//================================
typedef struct
{
	unsigned long tmid:29;
	unsigned char hlen:1;
	unsigned char rtr:1;
	unsigned char ide:1;
}TMID_BIT_FIELD;


typedef union
{
	unsigned long all;
	TMID_BIT_FIELD bit;

}TMID_MEMBER;
//====================================

typedef struct
{
	unsigned int rsvd:28;
	unsigned int dlc:4;
}TMPTR_BIT_FIELD;

typedef union
{
	unsigned long all;
	TMPTR_BIT_FIELD bit;

}TMPTR_MEMBER;
//===========================================

typedef struct
{
	unsigned char TMDB[4];

}TMDF0_BYTE_FIELD;

typedef union
{
	unsigned long all;
	TMDF0_BYTE_FIELD byte;

}TMDF_MEMBER;
//===========================================
typedef struct
{
	unsigned char TMDB[4];
}TMDF1_BYTE_FIELD;

typedef union
{
	unsigned long all;
	TMDF0_BYTE_FIELD byte;

}TMDF1_MEMBER;
//============================================
typedef struct
{
    unsigned char tmtr:1;
    unsigned char tmtar:1;
    unsigned char tmom:1;
    unsigned char rsvd:5;
}TMC_BIT_FIELD;

typedef union
{
    unsigned char all;
    TMC_BIT_FIELD bit;

}TMC_MEMBER;

typedef struct
{
    unsigned char  tmesi:1;
    unsigned char  tmbrs:1;
    unsigned char  tmfdf:1;
    unsigned short rsvd:13;
    unsigned short tmptr:16;
}TMFDCTR_BIT_FIELD;

typedef union
{
    unsigned long all;
    TMFDCTR_BIT_FIELD bit;
}TMFDCTR_MEMBER;
//==============================================

typedef struct
{
  TMID_MEMBER   id;
  TMPTR_MEMBER  ptr;
  TMDF_MEMBER   df[16];
  TMC_MEMBER    tmc;
  TMFDCTR_MEMBER tmfdctr;  
}CAN_BUS_TX_INFO_HANDLE;

//=================================



union __tagDAT
{                                                          /* IOR              */
    unsigned long  UINT32;                                 /* 32-bit Access    */
    unsigned short UINT16[2];                              /* 16-bit Access    */
    unsigned char  UINT8[4];                               /* 8-bit Access     */
};


typedef struct 
{                                                          /* Bit Access       */
    unsigned char  CRSTSTS:1;                              /* CRSTSTS          */
    unsigned char  CHLTSTS:1;                              /* CHLTSTS          */
    unsigned char  CSLPSTS:1;                              /* CSLPSTS          */
    unsigned char  EPSTS:1;                                /* EPSTS            */
    unsigned char  BOSTS:1;                                /* BOSTS            */
    unsigned char  TRMSTS:1;                               /* TRMSTS           */
    unsigned char  RECSTS:1;                               /* RECSTS           */
    unsigned char  COMSTS:1;                               /* COMSTS           */
    unsigned char  :8;                                     /* Reserved Bits    */
    unsigned char  REC:8;                                  /* REC[23:16]       */
    unsigned char  TEC:8;                                  /* TEC[31:24]       */
}CMSTS_BIT;


typedef union
{                                                          /* IOR              */
    unsigned long  UINT32;                                 /* 32-bit Access    */
    unsigned short UINT16[2];                              /* 16-bit Access    */
    unsigned char  UINT8[4];                               /* 8-bit Access     */
    CMSTS_BIT BIT;                                   /* Bit Access       */
}CMSTS_DW;


struct RRT_PAGE_SET
{
    unsigned long CFDGAFLID;                            
    unsigned long CFDGAFLM;                             
    unsigned long CFDGAFLP0_;                            
    unsigned long CFDGAFLP1_;                       
};


struct TM_SET
{
    union __tagDAT TMID;                               /* CFDTMID0         */
    union __tagDAT TMPTR;                              /* CFDTMPTR0        */
    union __tagDAT TMFDCTR;                            /* CFDTMFDCTR0      */
    union __tagDAT TMDF0;                             /* CFDTMDF0_0       */
    union __tagDAT TMDF1;                             /* CFDTMDF1_0       */
    union __tagDAT TMDF2;                             /* CFDTMDF2_0       */
    union __tagDAT TMDF3;                             /* CFDTMDF3_0       */
    union __tagDAT TMDF4;                             /* CFDTMDF4_0       */
    union __tagDAT TMDF5;                             /* CFDTMDF5_0       */
    union __tagDAT TMDF6;                             /* CFDTMDF6_0       */
    union __tagDAT TMDF7;                             /* CFDTMDF7_0       */
    union __tagDAT TMDF8;                             /* CFDTMDF8_0       */
    union __tagDAT TMDF9;                             /* CFDTMDF9_0       */
    union __tagDAT TMDF10;                            /* CFDTMDF10_0      */
    union __tagDAT TMDF11;                            /* CFDTMDF11_0      */
    union __tagDAT TMDF12;                            /* CFDTMDF12_0      */
    union __tagDAT TMDF13;                            /* CFDTMDF13_0      */
    union __tagDAT TMDF14;                            /* CFDTMDF14_0      */
    union __tagDAT TMDF15;                            /* CFDTMDF15_0      */
    unsigned char  dummy[52];                           /* Reserved         */            
};


struct RFCC_SET
{
    union __tagDAT RFID;                               /* CFDRFID0         */
    union __tagDAT RFPTR;                              /* CFDRFPTR0        */
    union __tagDAT RFFDSTS;                            /* CFDRFFDSTS0      */
    union __tagDAT RFDF0;                             /* CFDRFDF0_0       */
    union __tagDAT RFDF1;                             /* CFDRFDF1_0       */
    union __tagDAT RFDF2;                             /* CFDRFDF2_0       */
    union __tagDAT RFDF3;                             /* CFDRFDF3_0       */
    union __tagDAT RFDF4;                             /* CFDRFDF4_0       */
    union __tagDAT RFDF5;                             /* CFDRFDF5_0       */
    union __tagDAT RFDF6;                             /* CFDRFDF6_0       */
    union __tagDAT RFDF7;                             /* CFDRFDF7_0       */
    union __tagDAT RFDF8;                             /* CFDRFDF8_0       */
    union __tagDAT RFDF9;                             /* CFDRFDF9_0       */
    union __tagDAT RFDF10;                            /* CFDRFDF10_0      */
    union __tagDAT RFDF11;                            /* CFDRFDF11_0      */
    union __tagDAT RFDF12;                            /* CFDRFDF12_0      */
    union __tagDAT RFDF13;                            /* CFDRFDF13_0      */
    union __tagDAT RFDF14;                            /* CFDRFDF14_0      */
    union __tagDAT RFDF15;                            /* CFDRFDF15_0      */
    unsigned char  dummy[52];                              /* Reserved         */

};


struct CHANNEL_SET
{
    union __tagDAT CxNCFG;                              /* CFDCxNCFG        */
    union __tagDAT CxCTR;                               /* CFDCxCTR         */
    union __tagDAT CxSTS;                               /* CFDCxSTS         */
    union __tagDAT CxERFL;                              /* CFDCxERFL        */
};

struct CHANNEL_SET_2
{
    union __tagDAT CxDCFG;                              /* CFDC0DCFG        */
    union __tagDAT CxFDCFG;                             /* CFDC0FDCFG       */
    union __tagDAT CxFDCTR;                             /* CFDC0FDCTR       */
    union __tagDAT CxFDSTS;                             /* CFDC0FDSTS       */
    union __tagDAT CxFDCRC;                             /* CFDC0FDCRC       */
    unsigned char  dummy342[12];                           /* Reserved         */

};

//=========================================================================================

typedef union
{
   struct
   {
       unsigned char b0:1;
       unsigned char b1:1;
       unsigned char b2:1;
       unsigned char b3:1;
       unsigned char b4:1;
       unsigned char b5:1;
       unsigned char b6:1;
       unsigned char b7:1;
   }BIT;
   unsigned char bytes;
}CAN_RX_ACK_HANDLE;


typedef union
{
    unsigned char data_byte[4];
    unsigned int data_dw;
}CAN_BUS_DATA_UNION;

typedef struct
{
//  unsigned int data[8];
  CAN_BUS_DATA_UNION data_u[16];
  unsigned int dlc;
  ID_MEMBER id;
}CAN_BUS_DATA_HANDLE;

#define CAN_RX_RULE_TABLE_AMOUNT                    32

/*
    Sixteen receive rules can be set per page.
    each RX channel RULE , must align to 16 or need to copy one by one
*/
#define CAN_RX_RULE_CURRENT_AMOUNT                  (16)
#define CAN2_RX_RULE_CURRENT_AMOUNT                 (16)
#define CAN_TX_BUFFER_AMOUNT                        32
#define CAN_RX_FIFO_BUFFER_AMOUNT                   8               //fix to 8 for hardware resource limitation 
#define CAN_RX_FIFO_DEPTH                           16             //max fifo level : 128

typedef struct
{
  CAN_BUS_RX_INFO_HANDLE rrt_handle[CAN_RX_RULE_TABLE_AMOUNT];
  CAN_BUS_DATA_HANDLE rx_data[CAN_RX_FIFO_BUFFER_AMOUNT][CAN_RX_FIFO_DEPTH];
  unsigned int rx_data_cnt[CAN_RX_FIFO_BUFFER_AMOUNT];
  CAN_RX_ACK_HANDLE rx_data_ack;
  CAN_BUS_TX_INFO_HANDLE tx_handle[CAN_TX_BUFFER_AMOUNT];

}CAN_BUS_HANDLE;







