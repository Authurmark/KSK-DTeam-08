/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : ComFunction.c
* Author             : OS Team
* Date First Issued  : 03/11/2015
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 03/11/2015: v0.1	- Start 
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Project includes. */
#include "ComFunction.h"
#include "Project_Function.h"
#include "Time_Manage_Function.h"
#include "IO_Kernel_Function.h"

/* Variable */
uint16 CRCData,cntData;
uint8 UART1_BUFFER_RX[i_MAX_UART];
uint8 UART1_BUFFER_TX[i_MAX_UART];
uint8 UART2_BUFFER_RX[i_MAX_UART];
uint8 UART2_BUFFER_TX[i_MAX_UART];
uint8 PACKAGE_BUFF[PACKAGE_SIZE];
uint8_t bLast_Idx_Low = 0, bLast_Idx_High = 0;
uint16   i_UART_TX=0, i_UART_RX=0;
uint16   uPackageLeng=0;
uint32   UART1_Tick;
uint32   UART2_Tick;
enumbool eUART1DetectEnCMD=eFALSE;
enumbool eUART2DetectEnCMD=eFALSE;
enumbool EnableUARTComm=eFALSE;
extern enumbool bFlag_USART_RX;
uint16 FirmwareSize;
uint16 FirmwareBlockIndex;
uint16 FirmwareBlock;


#ifdef USE_LED_SEGMENT
extern uint8_t bLED7Value[NUMBER_LED_7_SEGMENT];
#endif /* NUMBER_LED_7_SEGMENT */
extern Struct_Flash_Config_Parameter	StrConfigPara;
enumbool bFlagGetCommandLEDConfigUART1;
structIO_Manage_Output bLEDConfigCommand;
/************************************************/
//------------------------- USER COMMUNICATION FUNCTION -----------------------
/* New Variable */
#define NUMBER_INDEX_CHECK                      25
#define NUMBER_INDEX_CLEAR                      100




typedef enum {
    SPINDLE_RORATY = 0x01,
    SPINDLE_RESET  = 0x02,
}state_process;

typedef enum {
    SPINDLE_FORWARD = 0x01,
    SPINDLE_REVERSE = 0x02,
    SPINDLE_BREAK   = 0x03,
    SPINDLE_DISABLE = 0x04,
}state_DC_Spindle;

typedef struct{
  state_process         bProcess;
  uint8                 Speed_DC;
  state_DC_Spindle      bDC_Driection;
}BufferRX_Control_DC_Spindle;

BufferRX_Control_DC_Spindle BUFFER_RX_CONTROL_DC_SPINDLE;

#define NUM_MEMBER_ADC_Current_Measure          10
typedef struct{
  uint16        Buffer_ADC_Current_Measure[NUM_MEMBER_ADC_Current_Measure];
  uint16        Current_Value;
  uint16        Current_Max;
  enumbool      Flag_Update;
}Buffer_Current_Measure;

Buffer_Current_Measure BUFFER_CURRENT_MEASURE;

typedef struct{
  uint16        pulse_encoder;
  uint16        angle_motor;
  enumbool      Flag_Update;
}Buffer_Encoder;

Buffer_Encoder  BUFFER_ENCODER;

//---------RX DATA AND FEEDBACK ----------//
/** HAM XU LY RX DATA
  * 1. Lay data tu RXRingBuff (moi lan chay lay 1 byte) de vao UART1_BUFFER_RX
  * 2. Doi sau 1 thoi gian (sau nhieu lan chay, da lay du so byte moi) hoac lay du 1 goi du lieu
  * 3. Chay ham chia tach du lieu vComDivideBlockData() (kiem tra PREMABLE_BYTE ----> nhan dang cmd_type ---> Xu ly du lieu theo tung kien cmd_type ---> Xoa du lieu trong )
  * 4. Gui feedback
**/

enumbool vComDataProcess_USART1(void)
{
    static uint8 bUSARTReceive;
    enumbool bReturn;
    bReturn = eFALSE;
    if(pUART1_CONFIG.read_byte(&bUSARTReceive)==eTRUE)
    {
        if (i_UART_RX>=i_MAX_UART)       i_UART_RX=0;
        UART1_BUFFER_RX[i_UART_RX++] = bUSARTReceive;
        UART1_Tick = T1Us_Tick1Ms;
        eUART1DetectEnCMD = eTRUE;
        bReturn = eFALSE;
    }
    if(eUART1DetectEnCMD&&(T1Us_Tick1Ms>(UART1_Tick+UART_INTERVAL)))
    {
        /*Receive sequence*/
        eUART1DetectEnCMD = eFALSE;
        //vComDataHandle();
        vComDivideBlockData(UART1_BUFFER_RX,UART1_BUFFER_TX,pUSART1);
        bReturn = eTRUE;
    }
    return bReturn;
}

enumbool vComDataProcess_USART2(void)
{
    static uint8 bUSARTReceive;
    enumbool bReturn;
    bReturn = eFALSE;
    if(pUART2_CONFIG.read_byte(&bUSARTReceive)==eTRUE)
    {
        if (i_UART_RX>=i_MAX_UART)       i_UART_RX=0;
        UART2_BUFFER_RX[i_UART_RX++] = bUSARTReceive;
        UART2_Tick = T1Us_Tick1Ms;
        eUART2DetectEnCMD = eTRUE;
        bReturn = eFALSE;
    }
    if(eUART2DetectEnCMD&&(T1Us_Tick1Ms>(UART2_Tick+UART_INTERVAL)))
    {
        /*Receive sequence*/
        eUART2DetectEnCMD = eFALSE;
        //vComDataHandle();
        vComDivideBlockData(UART2_BUFFER_RX,UART2_BUFFER_TX,pUSART2);
        bReturn = eTRUE;
    }
    return bReturn;
}
/** HAM CHIA TACH GOI DU LIEU
  * Quet tim chuoi ky tu PREMABLE_BYTE 1-6
  * Kiem tra ky tu ket thuc co dung khong, khong dung thi bo goi du lieu nay
  * Xu ly data, dua vao cac buffer khac nhau, tuy thuoc vao iUART_CMD_TYPE
  * Reset value cho UART_BUFFER_RX, nhan gia tri moi
**/
void vComDivideBlockData(uint8 *UART_BUFFER_RX, uint8 *UART_BUFFER_TX,UART_Struct pUART )
{
  for(uint8 iIndex=0; iIndex<NUMBER_INDEX_CHECK; iIndex++)                              
  {
      //Quet tim chuoi ky tu PREMABLE_BYTE 1-6
      if((UART_BUFFER_RX[iIndex+iUART_PRE_1]==PREMABLE_BYTE_1)
        &&(UART_BUFFER_RX[iIndex+iUART_PRE_2]==PREMABLE_BYTE_2)
            &&(UART_BUFFER_RX[iIndex+iUART_PRE_3]==PREMABLE_BYTE_3)
               &&(UART_BUFFER_RX[iIndex+iUART_PRE_4]==PREMABLE_BYTE_4)
                  &&(UART_BUFFER_RX[iIndex+iUART_PRE_5]==PREMABLE_BYTE_5)
                    &&(UART_BUFFER_RX[iIndex+iUART_PRE_6]==PREMABLE_BYTE_6)
                      &&(UART_BUFFER_RX[iIndex+iUART_END_DATA]==END_DATA_BYTE))         //Kiem tra ky tu ket thuc co dung khong, khong dung thi bo goi du lieu nay
      {
          //Xu ly data, dua vao cac buffer khac nhau, tuy thuoc vao iUART_CMD_TYPE
          switch ((cmd_type)UART_BUFFER_RX[iIndex+iUART_CMD_TYPE])
          {
          case P2TCMD_INFO :
              vFeedBack_info_sys();
            break;
  
          case P2TCMD_SPINDLE:
              BUFFER_RX_CONTROL_DC_SPINDLE.bProcess             =UART_BUFFER_TX[iUART_CMD];
              BUFFER_RX_CONTROL_DC_SPINDLE.Speed_DC             =UART_BUFFER_TX[iUART_DATA];
              BUFFER_RX_CONTROL_DC_SPINDLE.bDC_Driection        =UART_BUFFER_TX[iUART_DATA+1];
              UART_Comm_Feedback_Command_Content(UART_BUFFER_TX,0,FBCODE_OK);
            break;
          
          case P2TCMD_TEST:
              pUART.send_str("[SYSTEM DEBUG]: NHAN LENH CMD P2TCMD!\r\n");
              uint16 PARA1 = 0xE1A0;
              //uint8 PARA2 = 0xA1;
              uint8 PARA5  = 0xB1;
              uint8 PARA6  = 0xB0;
              UART_MakeData(UART_BUFFER_TX,0,PARA1,0,0,0,PARA5,PARA6);
              //UART1_Send_BUF(UART_BUFFER_TX,i_UART_TX);
              pUART.send_buf(UART_BUFFER_TX,i_UART_TX);
            break;
          
          default :
          break;
          }
            
        
        
        
//          UART_BUFFER_RX_HEAD[UART_BUFFER_RX_HEAD_INDEX]=iIndex;                        
//          UART_BUFFER_RX_HEAD_INDEX++;
          break;
          
      }
  }
  
  // Clear value cho UART_BUFFER_RX, nhan gia tri moi
  memset(UART1_BUFFER_RX,0,i_MAX_UART);                                                                 

}

//---------MARE DATA----------//
/** TAO DATA VOI DATAFRAME DUNG CHUAN
1. DUNG CHO FEEDBACK DU LIEU VE DOI TUONG NHAN
2. DUNG DE TAO DATAFRAME CHO DU LIEU CAN TRUYEN
**/

void UART_MakeData_Head(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE)
{
    memset(UART_BUFFER_TX,0,i_MAX_UART);
    UART_BUFFER_TX[iUART_PRE_1] = PREMABLE_BYTE_1;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_2] = PREMABLE_BYTE_2;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_3] = PREMABLE_BYTE_3;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_4] = PREMABLE_BYTE_4;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_5] = PREMABLE_BYTE_5;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_6] = PREMABLE_BYTE_6;   /*data size LB*/
    
    UART_BUFFER_TX[iUART_CMD_TYPE] = CMD_TYPE;/*command*/
  
}

void UART_MakeData_8bit(uint8 *UART_BUFFER_TX,uint8 iIndex, uint8 DATA)
{
  UART_BUFFER_TX[iIndex] 		= DATA;
}

void UART_MakeData_16bit(uint8 *UART_BUFFER_TX,uint8 iIndex, uint16 DATA)
{
  UART_BUFFER_TX[iIndex] 		= DATA;                 //BYTE LOW
  UART_BUFFER_TX[iIndex+1] 		= DATA>>8;              //BYTE HIGH
}

void UART_MakeData_Tail(uint8 *UART_BUFFER_TX)
{
  UART_BUFFER_TX[iUART_END_DATA] = END_DATA_BYTE;
  i_UART_TX=iUART_END_DATA;
}

/* Ham ghep noi data, phan biet data 8 bit hay 16 bit*/
void UART_MakeData(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE, uint16 PARA1, uint16 PARA2, uint16 PARA3, uint16 PARA4, uint16 PARA5, uint16 PARA6)
{    
  UART_MakeData_Head(UART_BUFFER_TX,CMD_TYPE);
  
  //PARA 1-2
  if(PARA2!=0)
  {
      UART_MakeData_8bit(UART_BUFFER_TX,12, PARA1);
      UART_MakeData_8bit(UART_BUFFER_TX,13, PARA2);
  }
  else
  {
      UART_MakeData_16bit(UART_BUFFER_TX,12, PARA1);
  }
  
  //PARA 3-4
  if(PARA4!=0)
  {
      UART_MakeData_8bit(UART_BUFFER_TX,14, PARA3);
      UART_MakeData_8bit(UART_BUFFER_TX,15, PARA4);
  }
  else
  {
      UART_MakeData_16bit(UART_BUFFER_TX,14, PARA3);
  }
  
  //PARA 5-6
    if(PARA6!=0)
  {
      UART_MakeData_8bit(UART_BUFFER_TX,16, PARA5);
      UART_MakeData_8bit(UART_BUFFER_TX,17, PARA6);
  }
  else
  {
      UART_MakeData_16bit(UART_BUFFER_TX,16, PARA5);
  }
  
  UART_MakeData_Tail(UART_BUFFER_TX);
}

//---------FEEDBACK DATA----------//
/** 
1. FEEDBACK KHONG CAN NOI DUNG
2. FEEDBACK CO NOI DUNG
3. FEEDBACK THONG TIN CAU HINH

UART_Comm_Feedback_Command_Content -> MAKE DATA -> SENDBUFF / SENDSTRING
**/

void UART_Comm_Feedback_Command_Content(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE, uint16 CODE)
{
	/* Feedback Content */
        UART_MakeData_Head(UART_BUFFER_TX,CMD_TYPE);
        UART_MakeData_16bit(UART_BUFFER_TX,12,CODE);
        UART_MakeData_Tail(UART_BUFFER_TX);
        
        UART1_Send_BUF(UART1_BUFFER_TX,i_UART_TX);
}

void vFeedBack_info_sys(void)
{
    /* Send Info command */
    UART1_Send_STRING("\r\n\r\nINFO COMMAND: OK\r\n");
    UART1_Send_STRING("PRODUCT: ");
    UART1_Send_BUF(StrConfigPara.ProductName,sizeof(StrConfigPara.ProductName));
    UART1_Send_STRING("\r\n");
    UART1_Send_STRING("PRODUCT ID: ");
    UART1_Send_BUF(StrConfigPara.ProductID,sizeof(StrConfigPara.ProductID));
    UART1_Send_STRING("\r\n");
    UART1_Send_STRING("HARDWARE: ");
    UART1_Send_BUF(StrConfigPara.HW_Version,sizeof(StrConfigPara.HW_Version));
    UART1_Send_STRING("\r\n");
    UART1_Send_STRING("BOOTLOADER: ");
    UART1_Send_BUF(StrConfigPara.BL_Version,sizeof(StrConfigPara.BL_Version));
    UART1_Send_STRING("\r\n");
    UART1_Send_STRING("FIRMWARE: ");
    UART1_Send_BUF(StrConfigPara.FW_Version,sizeof(StrConfigPara.FW_Version));
    UART1_Send_STRING("\r\n");
}

//---------Make UART_BUFFER_TX----------//
/** 
  1. Ham tuan tu tao UART_BUFFER_TX cho moi buffer can truyen
  2. Chu ky hoat dong 1 tick slice he thong ~ 1ms
  3. Nhuong quyen uu tien chay cho IO Task va User Task
**/
uint8 CntUartBufferTx;

typedef enum {
    Bf_Current_Measure  = 0x00,
    Bf_Encoder          = 0x01,
}state_make_uart_buffer_tx;


timer tUSART_Make_Buffer;                       //Timer

void vMakeBufferTXTask( void *pvParameters )
{
	/* Delay before begin task */
	OS_vTaskDelay(50);
	/* Set prequency */
	portTickType xLastWakeTime;
	const portTickType xUser_Task_Frequency = 1;/* 1 tick slice ~ 1ms */
	xLastWakeTime = xTaskGetTickCount();
	
       
//        timer_set(&tUSART_Make_Buffer, 1 ,CLOCK_TYPE_MS);/* 1 ms */
        
	
      /* Task process */
      for(;;)
      {	
          /* Delay Exactly Frequency */
          OS_vTaskDelayUntil(&xLastWakeTime,xUser_Task_Frequency);

          uint16 PARA1 ;
          uint16 PARA2 ;

          BUFFER_CURRENT_MEASURE.Current_Value          =0xAE00;
          BUFFER_ENCODER.pulse_encoder                  =0xAF00;
          BUFFER_ENCODER.angle_motor                    =0x9D9D;

          CntUartBufferTx = (CntUartBufferTx+1)%2;

          switch (CntUartBufferTx)
          {
              case Bf_Current_Measure :
                  PARA1 = BUFFER_CURRENT_MEASURE.Current_Value;
                  UART_MakeData(UART2_BUFFER_TX,0,PARA1,0,0,0,0,0);
                  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);
              break;

              case Bf_Encoder :
                  PARA1 = BUFFER_ENCODER.pulse_encoder;
                  PARA2 = BUFFER_ENCODER.angle_motor;
                  UART_MakeData(UART2_BUFFER_TX,0,PARA1,0,PARA2,0,0,0);
                  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);        
              break;

              default :
              break;
          }
      }
}

//void vMake_UART_BUFFER_TX()
//{
//  uint16 PARA1 ;
//  uint16 PARA2 ;
//  
//  BUFFER_CURRENT_MEASURE.Current_Value          =0xC2A1;
//  BUFFER_ENCODER.pulse_encoder                  =0xC1E1;
//  BUFFER_ENCODER.angle_motor                    =0x7777;
//  
//  if(timer_expired(&tUSART_Make_Buffer))
//  {
//      CntUartBufferTx = (CntUartBufferTx+1)%2;
//    
//      switch (CntUartBufferTx)
//      {
//        case Bf_Current_Measure :
//            PARA1 = BUFFER_CURRENT_MEASURE.Current_Value;
//            UART_MakeData(0,PARA1,0,0,0,0,0);
//            UART_Send_BUF(UART_BUFFER_TX,i_UART_TX);
//        break;
//        
//        case Bf_Encoder :
//            PARA1 = BUFFER_ENCODER.pulse_encoder;
//            PARA2 = BUFFER_ENCODER.angle_motor;
//            UART_MakeData(0,PARA1,0,PARA2,0,0,0);
//            UART_Send_BUF(UART_BUFFER_TX,i_UART_TX);        
//        break;
//
//        default :
//        break;
//      }
//
//  }
//}


