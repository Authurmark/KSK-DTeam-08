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
/*
  Author :  Authur
  Date   :  26/03/2018
  Edited :  03/04/2018 
  1. Define Protocol USART with Data Frame                      in ComFunction.h
  2. Define Buffer Data
  3. Handle RX RingBuff by devide block data for Buffer Data    in Task vApplicationIdleHook        :   vComDivideBlockData
  4. Make Data Function by                                                                          :   UART_MakeData
  5. Feedback Function by                                                                           :   UART_Comm_Feedback_Command_Content       vFeedBack_info_sys
  6. Make TX Buff      in Task vMakeBufferTXTask    with frequency                                  :   1 tick slice ~ 1ms                                                
*/

/* Project includes. */
#include "ComFunction.h"
#include "Project_Function.h"
#include "Time_Manage_Function.h"
#include "IO_Kernel_Function.h"

/*--------------------------------------------------------------------*/
//-------------------Variable For RX and TX Buffer-------------------//
/*------------------------------------------------------------------*/
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

#define NUMBER_INDEX_CHECK                      25
#define NUMBER_INDEX_CLEAR                      100

#ifdef USE_LED_SEGMENT
extern uint8_t bLED7Value[NUMBER_LED_7_SEGMENT];
#endif /* NUMBER_LED_7_SEGMENT */
extern Struct_Flash_Config_Parameter	StrConfigPara;
enumbool bFlagGetCommandLEDConfigUART1;
structIO_Manage_Output bLEDConfigCommand;

/*--------------------------------------------------------------------*/
//--------------------------BUFFER DATA------------------------------//
/*------------------------------------------------------------------*/
Buffer_Control_DC_Spindle BUFFER_CONTROL_DC_SPINDLE;
/************************Current Measure Value*********************/
Buffer_Current_Measure BUFFER_CURRENT_MEASURE;
/* Encoder Home Value */
Buffer_EncoderHome  BUFFER_ENCODERHOME;
/*--------------------------------------------------------------------*/
//-------------------USER COMMUNICATION FUNCTION---------------------//
/*------------------------------------------------------------------*/


//---------RX DATA AND FEEDBACK ----------//
/**
  Author :  Authur
  Date   :  26/03/2018
  Edited :  03/04/2018 
  * 1. Get data from RXRingBuff (each time run get one byte) to UART1_BUFFER_RX
  * 2. Wait for a time (sau nhieu lan chay, da lay du so byte moi) hoac lay du 1 goi du lieu
  * 3. Run vComDivideBlockData() to devide a block data         
        + Check PREMABLE_BYTE and Detect END_DATA_BYTE
        + Detect cmd_type to handle
        + Clear - reset UART1_BUFFER_RX after handle
  * 4. Send feedback for detect error OverTime
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
        vComDivideBlockData(UART2_BUFFER_RX,UART2_BUFFER_TX,pUSART2);
        bReturn = eTRUE;
    }
    return bReturn;
}




/** HAM CHIA TACH GOI DU LIEU
  Author :  Authur
  Date   :  26/03/2018
  Edited :  03/04/2018 
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
                      
                      //Kiem tra ky tu ket thuc co dung khong, khong dung thi bo goi du lieu nay
                      &&(UART_BUFFER_RX[iIndex+iUART_END_DATA]==END_DATA_BYTE))         
      
      {
          //Xu ly data, dua vao cac buffer khac nhau, tuy thuoc vao iUART_CMD_TYPE
          switch ((cmd_type)UART_BUFFER_RX[iIndex+iUART_CMD_TYPE])
          {
          	case P2TCMD_INFO :
              vFeedBack_info_sys();
            break;
  
          	case P2TCMD_SPINDLE:
              /* CODE MAU GHI DU LIEU VAO BUFFER DATA */
              BUFFER_CONTROL_DC_SPINDLE.bProcess             =UART_BUFFER_RX[iUART_CMD];
             // BUFFER_CONTROL_DC_SPINDLE.Speed_DC             =UART_BUFFER_RX[iUART_DATA];
              BUFFER_CONTROL_DC_SPINDLE.bDC_Driection        =UART_BUFFER_RX[iUART_DATA+1];
              UART_Comm_Feedback_Command_Content(UART_BUFFER_TX,0,FBCODE_OK);
              /* END CODE MAU GHI DU LIEU VAO BUFFER */
            break;
          	case P2TCMD_Current_Measure:
               BUFFER_CURRENT_MEASURE.Current_Max			 = UART_BUFFER_RX[iUART_DATA];
			break;
          /* CODE MAU TAO BUFFER TX GUI */
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
          /* END CODE MAU TAO BUFFER TX GUI */  
          
          
          default :
          break;
          }
            
        
        
        
//          UART_BUFFER_RX_HEAD[UART_BUFFER_RX_HEAD_INDEX]=iIndex;                        
//          UART_BUFFER_RX_HEAD_INDEX++;
          break;
          
      }/*END IF*/
  }
  
  // Clear value cho UART_BUFFER_RX, nhan gia tri moi
  memset(UART1_BUFFER_RX,0,i_MAX_UART);                                                                 

}




















//---------FEEDBACK DATA----------//
/** 
  Author :  Authur
  Date   :  26/03/2018
  Edited :  03/04/2018 
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





















//---------MARE DATA----------//
/** TAO DATA VOI DATAFRAME DUNG CHUAN
  Author :  Authur
  Date   :  26/03/2018
  Edited :  03/04/2018 
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




























//---------Make UART_BUFFER_TX----------//
/** 
  Author :  Authur
  Date   :  26/03/2018
  Edited :  03/04/2018 
  1. Ham tuan tu tao UART_BUFFER_TX cho moi buffer can truyen
  2. Chu ky hoat dong 1 tick slice he thong ~ 1ms
  3. Nhuong quyen uu tien chay cho IO Task va User Task
**/
uint8 CntUartBufferTx;

typedef enum {
    Bf_Current_Measure  = 0x00,
    Bf_Encoder          = 0x01,
   	Bf_Control_Process 	= 0x02,
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

          /*TAO DU LIEU TEST */
          uint16 PARA1 ;
          uint16 PARA2 ;
          
          BUFFER_CURRENT_MEASURE.Current_Value          =0xAE00;
                 
          /*END TAO DU LIEU TEST */
          
          CntUartBufferTx = (CntUartBufferTx+1)%2;

          switch (CntUartBufferTx)
          {
              case Bf_Current_Measure :
                  PARA1 = BUFFER_CURRENT_MEASURE.Current_Value;
                  UART_MakeData(UART2_BUFFER_TX,0,PARA1,0,0,0,0,0);
                  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);
              break;
              case Bf_Encoder :
                  PARA1 = BUFFER_ENCODERHOME.Flag_Home;
                  UART_MakeData(UART2_BUFFER_TX,0,PARA1,0,0,0,0,0);
                  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);        
              break;
			  case Bf_Control_Process:
                   PARA1 = BUFFER_CONTROL_DC_SPINDLE.Error_Process;
				   UART_MakeData(UART2_BUFFER_TX,0,PARA1,0,0,0,0,0);
                   UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);
              default :
              break;
          }
   
      }
}


