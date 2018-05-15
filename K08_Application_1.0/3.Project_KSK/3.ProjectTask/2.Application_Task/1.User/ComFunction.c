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

uint8 UART3_BUFFER_RX[i_MAX_UART];
uint8 UART3_BUFFER_TX[i_MAX_UART];



uint8 PACKAGE_BUFF[PACKAGE_SIZE];
uint8_t bLast_Idx_Low = 0, bLast_Idx_High = 0;
uint16   i_UART_TX=0, i_UART1_RX=0, i_UART2_RX=0, i_UART3_RX=0;
uint16   uPackageLeng=0;

uint32   UART1_Tick, UART2_Tick, UART3_Tick;

enumbool eUART1DetectEnCMD=eFALSE;
enumbool eUART2DetectEnCMD=eFALSE;
enumbool eUART3DetectEnCMD=eFALSE;

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

/*Config Machine Parameter*/

/*---- Define Value for Debug ---*/
#define Config_X_Axis_Speed 3          //cm/s   
#define Config_Y_Axis_Speed 3          //cm/s
#define Config_Z_Axis_Speed 3          //cm/s

uint16 Cutter_Position_X[5] = {200,200,200,200,200};
uint16 Cutter_Position_Y[5] = {110,140,170,200,230};
uint16 Cutter_Position_Z    = 70;

#define Config_Speed_DC        80         // %
#define Config_Current_Max     16         // 1.6V
#define Config_Height_Check    20         //mm








/*Hole data*/
buffer_hole_data BUFFER_HOLE_DATA;







/* Control Process */


strbuffer_machine_control BUFFER_MACHINE_CONTROL;










/*Motor Control 3-Axis Process*/
Buffer_Axis_Process BUFFER_AXIS_PROCESS;












/* Control X-Y-Z Axis */
Buffer_Control_Axis BUFFER_X_AXIS_CONTROL;
Buffer_Control_Axis BUFFER_Y_AXIS_CONTROL;
Buffer_Control_Axis BUFFER_Z_AXIS_CONTROL;













/* Control DC Spindle */
Buffer_Control_DC_Spindle BUFFER_CONTROL_DC_SPINDLE;










/* Current Measure Value */
Buffer_Current_Measure BUFFER_CURRENT_MEASURE;
















/* Encoder Home Value */
Buffer_Encoder  BUFFER_ENCODER;









/*Status Button Pause/Run and Stop*/
buffer_StateButton BUFFER_STATEBUTTON ;




/* BUFFER SEND FEEDBACK */
typedef struct{
  //to PC
  enumbool      bFlag_ReNew_DataHole;   
  enumbool      bFlag_ReNew_MachineProcess;
  
  //to Slave1
  
  //to Slave2
}strBuffSendFeedBack;

strBuffSendFeedBack BUFF_SEND_FEEDBACK;


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
        if (i_UART1_RX>=i_MAX_UART)       i_UART1_RX=0;
        UART1_BUFFER_RX[i_UART1_RX++] = bUSARTReceive;
        UART1_Tick = T1Us_Tick1Ms;
        eUART1DetectEnCMD = eTRUE;
        bReturn = eFALSE;
    }
    if(eUART1DetectEnCMD&&(T1Us_Tick1Ms>(UART1_Tick+UART_INTERVAL)))
    {
        /*Receive sequence*/
        i_UART1_RX=0;
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
        if (i_UART2_RX>=i_MAX_UART)       i_UART2_RX=0;
        UART2_BUFFER_RX[i_UART2_RX++] = bUSARTReceive;
        UART2_Tick = T1Us_Tick1Ms;
        eUART2DetectEnCMD = eTRUE;
        bReturn = eFALSE;
    }
    if(eUART2DetectEnCMD&&(T1Us_Tick1Ms>(UART2_Tick+UART_INTERVAL)))
    {
        /*Receive sequence*/
        i_UART2_RX=0;
        eUART2DetectEnCMD = eFALSE;
        vComDivideBlockData(UART2_BUFFER_RX,UART2_BUFFER_TX,pUSART2);
        bReturn = eTRUE;
    }
    return bReturn;
}

enumbool vComDataProcess_USART3(void)
{
    static uint8 bUSARTReceive;
    enumbool bReturn;
    bReturn = eFALSE;
    if(pUART3_CONFIG.read_byte(&bUSARTReceive)==eTRUE)
    {
        if (i_UART3_RX>=i_MAX_UART)       i_UART3_RX=0;
        UART3_BUFFER_RX[i_UART3_RX++] = bUSARTReceive;
        UART3_Tick = T1Us_Tick1Ms;
        eUART3DetectEnCMD = eTRUE;
        bReturn = eFALSE;
    }
    if(eUART3DetectEnCMD&&(T1Us_Tick1Ms>(UART3_Tick+UART_INTERVAL)))
    {
        /*Receive sequence*/
        i_UART3_RX=0;
        eUART3DetectEnCMD = eFALSE;
        vComDivideBlockData(UART3_BUFFER_RX,UART3_BUFFER_TX,pUSART3);
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
uint16 DataGet=0;

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
          //Recieve Data from PC - USART3
          case P2TCMD_HOLE_DATA:
              
              BUFFER_HOLE_DATA.iIdex_hole_data                                  = UART_BUFFER_RX[iUART_CMD];
              
              DataGet                                                           = UART_BUFFER_RX[iUART_DATA];
              DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+1]<<8);
              BUFFER_HOLE_DATA.Position_X[BUFFER_HOLE_DATA.iIdex_hole_data]     = DataGet;
              
              DataGet                                                           = UART_BUFFER_RX[iUART_DATA+2];
              DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+3]<<8);
              BUFFER_HOLE_DATA.Position_Y[BUFFER_HOLE_DATA.iIdex_hole_data]     = DataGet;
              
              BUFFER_HOLE_DATA.Dia_Hole  [BUFFER_HOLE_DATA.iIdex_hole_data]     = UART_BUFFER_RX[iUART_DATA+4];
              
              //Feedback to PC - USART3 :
              BUFF_SEND_FEEDBACK.bFlag_ReNew_DataHole                           = eTRUE;
//              UART_Comm_Feedback_Command_Content(pUSART3,UART3_BUFFER_RX,P2TCM_FEEDBACK_DATA,P2TCMD_HOLE_DATA,0);
            break;

          case P2TCMD_MACHINE:
              BUFFER_MACHINE_CONTROL.bProcess_Control_Machine                   = UART_BUFFER_RX[iUART_CMD];
              BUFFER_MACHINE_CONTROL.iIdex_hole_check                           = UART_BUFFER_RX[iUART_DATA];
              BUFFER_MACHINE_CONTROL.eFlag_Process_Update                       = eTRUE;
              
              //Feedback to PC - USART3 :
              BUFF_SEND_FEEDBACK.bFlag_ReNew_MachineProcess                     = eTRUE;
            break;
          
          
            
            
          
            
          //Recieve Data from Slave1 - USART1  
          case P2TCMD_AXIS_PROCESS :
              BUFFER_AXIS_PROCESS.bFeedBackAxis                                 = UART_BUFFER_RX[iUART_CMD];
              if(UART_BUFFER_RX[iUART_DATA]!=0)
              {
                BUFFER_AXIS_PROCESS.bError                                      = UART_BUFFER_RX[iUART_DATA];
                //Feedback have got error
                UART_Comm_Feedback_Command_Content(pUSART3,UART3_BUFFER_RX,P2TCM_FEEDBACK_DATA,P2TCM_ERROR_GET,0);
              }
              
              //Follow Feedback Process Info
              BUFFER_AXIS_PROCESS.bFlag_Process_Info							=eTRUE;
            break;
            
          case P2TCMD_X_AXIS_CONTROL:
              DataGet                                                           = UART_BUFFER_RX[iUART_DATA];
              DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+1]<<8);
              BUFFER_X_AXIS_CONTROL.Axis_PositionGet                            = DataGet;
              
              //Follow Feedback Process Info
              BUFFER_X_AXIS_CONTROL.bFlag_Process_Info     						=eTRUE;
            break;
            
          case P2TCMD_Y_AXIS_CONTROL:
              DataGet                                                           = UART_BUFFER_RX[iUART_DATA];
              DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+1]<<8);
              BUFFER_Y_AXIS_CONTROL.Axis_PositionGet                            = DataGet;
              
              //Follow Feedback Process Info
              BUFFER_Y_AXIS_CONTROL.bFlag_Process_Info                          =eTRUE;
            break;
            
          case P2TCMD_Z_AXIS_CONTROL:
              DataGet                                                           = UART_BUFFER_RX[iUART_DATA];
              DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+1]<<8);
              BUFFER_Z_AXIS_CONTROL.Axis_PositionGet                            = DataGet;
              
              //Follow Feedback Process Info
              BUFFER_Z_AXIS_CONTROL.bFlag_Process_Info							=eTRUE;
            break;
            
            
            
            
            
            
            
           
            
          //Recieve Data from Slave2 - USART2
          case P2TCMD_ENCODER_HOME:
              BUFFER_ENCODER.Flag_Home                                          = UART_BUFFER_RX[iUART_CMD]; 
              
              //Follow Feedback Process Info
              //BUFFER_ENCODER.bFlag_Process_Info=eTRUE;
          break;
          
          case P2TCM_CURRENT_MEASURE:
              BUFFER_CURRENT_MEASURE.Flag_QualityPoor                           = UART_BUFFER_RX[iUART_CMD];
              DataGet                                                           = UART_BUFFER_RX[iUART_DATA];
              DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+1]<<8);
              BUFFER_CURRENT_MEASURE.Current_Value                              = DataGet;
              
              //Follow Feedback Process Info
              BUFFER_CURRENT_MEASURE.bFlag_Process_Info=eTRUE;
            break;
          
          case P2TCMD_SPINDLE:
              BUFFER_CONTROL_DC_SPINDLE.bProcess                                = UART_BUFFER_RX[iUART_CMD];
              BUFFER_CONTROL_DC_SPINDLE.Speed_DC                                = UART_BUFFER_RX[iUART_DATA];
              BUFFER_CONTROL_DC_SPINDLE.bDC_Driection                           = UART_BUFFER_RX[iUART_DATA+1];
              BUFFER_CONTROL_DC_SPINDLE.bError                                  = UART_BUFFER_RX[iUART_DATA+2];
            break;
          
          
            
            
            
            
          //Recieve Feedback
          case P2TCM_FEEDBACK_DATA:
              switch(UART_BUFFER_RX[iUART_CMD])
              {
              case P2TCMD_SPINDLE:
                BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eFALSE;
                break;
                
              case P2TCMD_AXIS_PROCESS:
                BUFFER_AXIS_PROCESS.Flag_Update=eFALSE;
                break;
                
              case P2TCMD_X_AXIS_CONTROL:
                BUFFER_X_AXIS_CONTROL.bFlag_Update=eFALSE;
                break;
                
              case P2TCMD_Y_AXIS_CONTROL:
                BUFFER_Y_AXIS_CONTROL.bFlag_Update=eFALSE;
                break;
                
              case P2TCMD_Z_AXIS_CONTROL:
                BUFFER_Z_AXIS_CONTROL.bFlag_Update=eFALSE;
                break; 
              default:
                break;
              }

                
          break;
          
          case P2TCMD_TEST:
              if(UART_BUFFER_RX[iUART_CMD]==1)
              {
                BUFFER_MACHINE_CONTROL.sHold_Cutter                             = UART_BUFFER_RX[iUART_DATA];
              
              }
              
              if(UART_BUFFER_RX[iUART_CMD]==2)
              {
                uint16 DataGet=0;
                DataGet                                                           = UART_BUFFER_RX[iUART_DATA];
                DataGet                                                           = DataGet | (UART_BUFFER_RX[iUART_DATA+1]<<8);
                Buffer_LinearScale.spindle_position                               = DataGet;                
              }
            break;
          
          
          default :
          break;
          }
            
        
        
        
//          UART_BUFFER_RX_HEAD[UART_BUFFER_RX_HEAD_INDEX]=iIndex;                        
//          UART_BUFFER_RX_HEAD_INDEX++;
         
          
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

void UART_Comm_Feedback_Command_Content(UART_Struct pUSART_USE,uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE,uint8 CMD, uint16 CODE)
{
	/* Feedback Content */
        UART_MakeData_Head(UART_BUFFER_TX,CMD_TYPE);
        UART_MakeData_8bit(UART_BUFFER_TX,iUART_CMD,CMD);
        UART_MakeData_16bit(UART_BUFFER_TX,iUART_DATA,CODE);
        UART_MakeData_Tail(UART_BUFFER_TX);
        
        pUSART_USE.send_buf(UART_BUFFER_TX,i_UART_TX);
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

timer t_DetectOverTime_Axis_Process;
timer t_DetectOverTime_X_Axis_Control;
timer t_DetectOverTime_Y_Axis_Control;
timer t_DetectOverTime_Z_Axis_Control;
timer t_DetectOverTime_Spindle_Control;
timer t_DetectOverTime_CurrentMeasure;

uint8 cnt_timeover_Axis_Process;
uint8 cnt_timeover_X_Axis_Control;
uint8 cnt_timeover_Y_Axis_Control;
uint8 cnt_timeover_Z_Axis_Control;
uint8 cnt_timeover_Spindle_Control;
uint8 cnt_timeover_CurrentMeasure;

void vInitFeedBackDetectOverTime(void)
{
  timer_set(&t_DetectOverTime_Axis_Process,   50 ,CLOCK_TYPE_MS);         /*50ms */
  timer_set(&t_DetectOverTime_X_Axis_Control, 50 ,CLOCK_TYPE_MS);       /*50ms */
  timer_set(&t_DetectOverTime_Y_Axis_Control, 50 ,CLOCK_TYPE_MS);       /*50ms */
  timer_set(&t_DetectOverTime_Z_Axis_Control, 50 ,CLOCK_TYPE_MS);       /*50ms */
}

void vFeedBackDetectOverTime(void)
{
  //Detect Axis Process Error OverTime
  if(timer_expired(&t_DetectOverTime_Axis_Process))
  {
    timer_restart(&t_DetectOverTime_Axis_Process);
    if(BUFFER_AXIS_PROCESS.bFlag_Process_Info==eTRUE)
    {
      BUFFER_AXIS_PROCESS.bFlag_Process_Info=eFALSE;
    }
    else
    {
      cnt_timeover_Axis_Process++;
      if(cnt_timeover_Axis_Process>5)                           /*200ms*/
        {
          //Detect Error OverTime
		  BUFFER_MACHINE_CONTROL.bErrorMachine = eMachine_OverTime;
        }  
    }
  }

  
//  //Detect X Axis Control Error OverTime
//  if(timer_expired(&t_DetectOverTime_X_Axis_Control))
//  {
//    timer_restart(&t_DetectOverTime_X_Axis_Control);
//    if(BUFFER_X_AXIS_CONTROL.bFlag_Process_Info==eTRUE)
//      BUFFER_X_AXIS_CONTROL.bFlag_Process_Info=eFALSE;
//    else
//    {
//      cnt_timeover_X_Axis_Control++;
//      if(cnt_timeover_X_Axis_Control>5)                           /*200ms*/
//        {
//          //Detect Error OverTime
//		  BUFFER_MACHINE_CONTROL.bErrorMachine = eMachine_OverTime;
//		  
//        }  
//    }
//  }
//  
//  
//  
//  
//  //Detect Y Axis Control Error OverTime
//  if(timer_expired(&t_DetectOverTime_Y_Axis_Control))
//  {
//    timer_restart(&t_DetectOverTime_Y_Axis_Control);
//    if(BUFFER_Y_AXIS_CONTROL.bFlag_Process_Info==eTRUE)
//      BUFFER_Y_AXIS_CONTROL.bFlag_Process_Info=eFALSE;
//    else
//    {
//      cnt_timeover_Y_Axis_Control++;
//      if(cnt_timeover_Y_Axis_Control>5)                           /*200ms*/
//        {
//          //Detect Error OverTime
//			BUFFER_MACHINE_CONTROL.bErrorMachine = eMachine_OverTime;
//        }  
//    }
//  }
//  
//  
//  
//  
//  
//  
//  //Detect Z Axis Control Error OverTime
//  if(timer_expired(&t_DetectOverTime_Z_Axis_Control))
//  {
//    timer_restart(&t_DetectOverTime_Z_Axis_Control);
//    if(BUFFER_Z_AXIS_CONTROL.bFlag_Process_Info==eTRUE)
//      BUFFER_Z_AXIS_CONTROL.bFlag_Process_Info=eFALSE;
//    else
//    {
//      cnt_timeover_Z_Axis_Control++;
//      if(cnt_timeover_Z_Axis_Control>5)                           /*200ms*/
//        {
//          //Detect Error OverTime
//		  BUFFER_MACHINE_CONTROL.bErrorMachine = eMachine_OverTime;
//        }  
//    }
//  }
//  
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
void UART_MakeData(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE, uint8 CMD,uint16 PARA1, uint16 PARA2, uint16 PARA3, uint16 PARA4, uint16 PARA5, uint16 PARA6)
{    
  UART_MakeData_Head(UART_BUFFER_TX,CMD_TYPE);
  UART_MakeData_8bit(UART_BUFFER_TX,iUART_CMD,CMD);
  //PARA 1-2
  if(PARA2!=0)
  {
      UART_MakeData_8bit(UART_BUFFER_TX,iUART_DATA, PARA1);
      UART_MakeData_8bit(UART_BUFFER_TX,iUART_DATA+1, PARA2);
  }
  else
  {
      UART_MakeData_16bit(UART_BUFFER_TX,iUART_DATA, PARA1);
  }
  
  //PARA 3-4
  if(PARA4!=0)
  {
      UART_MakeData_8bit(UART_BUFFER_TX,iUART_DATA+2, PARA3);
      UART_MakeData_8bit(UART_BUFFER_TX,iUART_DATA+3, PARA4);
  }
  else
  {
      UART_MakeData_16bit(UART_BUFFER_TX,iUART_DATA+2, PARA3);
  }
  
  //PARA 5-6
    if(PARA6!=0)
  {
      UART_MakeData_8bit(UART_BUFFER_TX,iUART_DATA+4, PARA5);
      UART_MakeData_8bit(UART_BUFFER_TX,iUART_DATA+5, PARA6);
  }
  else
  {
      UART_MakeData_16bit(UART_BUFFER_TX,iUART_DATA+4, PARA5);
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

uint8 PARA8_CMD_TYPE;
uint8 PARA8_CMD;
uint32 PARA32_1;
uint32 PARA32_2;
uint32 PARA32_3;
uint32 PARA32_4;

uint16 PARA16_1 ;
uint16 PARA16_2 ;
uint16 PARA16_3 ;

uint8 PARA8_1;
uint8 PARA8_2;
uint8 PARA8_3;
uint8 PARA8_4;
uint8 PARA8_5;
uint8 PARA8_6;

//USART 3 - send to PC
typedef enum {
    Bf_Send_FeedBack_PC         = 0x00,
    Bf_Machine_Process          = 0x01,
    Bf_Hole_Data                = 0x02,
}state_make_uart3_buffer_tx;

//USART 1 - send to Slave1
typedef enum {
    Bf1_ConfigParamater         = 0x00,
    Bf_Axis_Process             = 0x01,
    Bf_X_Axis_Control           = 0x02,
    Bf_Y_Axis_Control           = 0x03,
    Bf_Z_Axis_Control           = 0x04,
	Bf_StateButton1   			= 0x05,
}state_make_uart1_buffer_tx;

//USART 2 - send to Slave2
typedef enum {
    Bf2_ConfigParamater         = 0x00,
    Bf_Control_DC_Spindle       = 0x01,
    Bf_Current_Measure          = 0x02,
	Bf_StateButton2   			= 0x03,
}state_make_uart2_buffer_tx;


timer tUSART_Make_Buffer;                       //Timer

void vMakeBufferTXTask( void *pvParameters )
{
	/* Delay before begin task */
	OS_vTaskDelay(50);
	/* Set prequency */
	portTickType xLastWakeTime;
	const portTickType xUser_Task_Frequency = 5;/* 1 tick slice ~ 5ms */
	xLastWakeTime = xTaskGetTickCount();
	
       
//        timer_set(&tUSART_Make_Buffer, 1 ,CLOCK_TYPE_MS);/* 1 ms */
        
	
      /* Task process */
      for(;;)
      {	
          /* Delay Exactly Frequency */
          OS_vTaskDelayUntil(&xLastWakeTime,xUser_Task_Frequency);

          
          CntUartBufferTx = (CntUartBufferTx+1)%6;
          //USART 3 - send to PC
          switch (CntUartBufferTx)
          {
              case Bf_Send_FeedBack_PC:
                  if(BUFF_SEND_FEEDBACK.bFlag_ReNew_DataHole==eTRUE)
                  {
                    BUFF_SEND_FEEDBACK.bFlag_ReNew_DataHole=eFALSE;
                    UART_Comm_Feedback_Command_Content(pUSART3,UART3_BUFFER_RX,P2TCM_FEEDBACK_DATA,P2TCMD_HOLE_DATA,0);
                  }
                  if(BUFF_SEND_FEEDBACK.bFlag_ReNew_MachineProcess==eTRUE)
                  {
                    BUFF_SEND_FEEDBACK.bFlag_ReNew_MachineProcess=eFALSE;  
                    UART_Comm_Feedback_Command_Content(pUSART3,UART3_BUFFER_RX,P2TCM_FEEDBACK_DATA,P2TCMD_MACHINE,0);
                  }
              break;
            
              case Bf_Machine_Process :
                  PARA8_CMD_TYPE=P2TCMD_MACHINE;
                  PARA8_CMD     =BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine;             
                  PARA8_2       =BUFFER_MACHINE_CONTROL.sStatus_Cutter[4];
                  PARA8_2       =(PARA8_2<1) | (BUFFER_MACHINE_CONTROL.sStatus_Cutter[3]);
                  PARA8_2       =(PARA8_2<1) | (BUFFER_MACHINE_CONTROL.sStatus_Cutter[2]);
                  PARA8_2       =(PARA8_2<1) | (BUFFER_MACHINE_CONTROL.sStatus_Cutter[1]);
                  PARA8_2       =(PARA8_2<1) | (BUFFER_MACHINE_CONTROL.sStatus_Cutter[0]);
                  PARA8_3       =BUFFER_MACHINE_CONTROL.sHold_Cutter;
                  PARA8_4       =BUFFER_MACHINE_CONTROL.bErrorMachine;
                  
                  UART_MakeData(UART3_BUFFER_TX,PARA8_CMD_TYPE,PARA8_CMD,PARA8_1,PARA8_2,PARA8_3,PARA8_4,0,0);
                  UART3_Send_BUF(UART3_BUFFER_TX,i_UART_TX);
              break;
              
              case Bf_Hole_Data :
                  PARA8_CMD_TYPE=P2TCMD_HOLE_DATA;
                  PARA8_1       =BUFFER_HOLE_DATA.iIdex_hole_data;
                  PARA8_2       =BUFFER_HOLE_DATA.Hole_Check_Value[BUFFER_HOLE_DATA.iIdex_hole_data];
                  PARA16_2      =BUFFER_HOLE_DATA.Hole_Check_Height[BUFFER_HOLE_DATA.iIdex_hole_data];
                  PARA8_5       =BUFFER_HOLE_DATA.Detect_Thread_Hole[BUFFER_HOLE_DATA.iIdex_hole_data];
                  PARA8_6       =BUFFER_HOLE_DATA.Counter_Hole[4];
                  PARA8_6       =(PARA8_6<1) | (BUFFER_HOLE_DATA.Counter_Hole[3]);
                  PARA8_6       =(PARA8_6<1) | (BUFFER_HOLE_DATA.Counter_Hole[2]);
                  PARA8_6       =(PARA8_6<1) | (BUFFER_HOLE_DATA.Counter_Hole[1]);
                  PARA8_6       =(PARA8_6<1) | (BUFFER_HOLE_DATA.Counter_Hole[0]);
                  
                  UART_MakeData(UART3_BUFFER_TX,PARA8_CMD_TYPE,0,PARA8_1,PARA8_2,PARA16_2,0,PARA8_5,PARA8_6);
                  UART3_Send_BUF(UART3_BUFFER_TX,i_UART_TX);                  
              break;            
              default :
              break;
          }
          
          //USART 1 - send to Slave1
          switch (CntUartBufferTx)
          {
              case Bf_Axis_Process :
                  if(BUFFER_AXIS_PROCESS.Flag_Update==eTRUE)
                  {
                    PARA8_CMD_TYPE=P2TCMD_AXIS_PROCESS;
                    PARA8_CMD     =BUFFER_AXIS_PROCESS.bProcess;
                    
                    UART_MakeData(UART1_BUFFER_TX,PARA8_CMD_TYPE,PARA8_CMD,0,0,0,0,0,0);
                    UART1_Send_BUF(UART1_BUFFER_TX,i_UART_TX); 
                  }
              break;
            
              case Bf_X_Axis_Control :
                  if(BUFFER_X_AXIS_CONTROL.bFlag_Update==eTRUE)
                  {
                      PARA8_CMD_TYPE=P2TCMD_X_AXIS_CONTROL;
                      PARA16_1      =BUFFER_X_AXIS_CONTROL.Axis_PositionControl;
                      PARA8_3       =BUFFER_X_AXIS_CONTROL.Axis_Speed;
    //                  PARA8_4       =BUFFER_X_AXIS_CONTROL.Axis_bDirection; //??? khong biet co nen send khong!!!
                      
                      UART_MakeData(UART1_BUFFER_TX,PARA8_CMD_TYPE,0,PARA16_1,0,PARA8_3,PARA8_4,0,0);
                      UART1_Send_BUF(UART1_BUFFER_TX,i_UART_TX);  
                  }
              break;
          
              case Bf_Y_Axis_Control :
                  if(BUFFER_Y_AXIS_CONTROL.bFlag_Update==eTRUE)
                  {
                      PARA8_CMD_TYPE=P2TCMD_Y_AXIS_CONTROL;
                      PARA16_1      =BUFFER_Y_AXIS_CONTROL.Axis_PositionControl;
                      PARA8_3       =BUFFER_Y_AXIS_CONTROL.Axis_Speed;
    //                  PARA8_4       =BUFFER_Y_AXIS_CONTROL.Axis_bDirection; //??? khong biet co nen send khong!!!
                      
                      UART_MakeData(UART1_BUFFER_TX,PARA8_CMD_TYPE,0,PARA16_1,0,PARA8_3,PARA8_4,0,0);
                      UART1_Send_BUF(UART1_BUFFER_TX,i_UART_TX);      
                  }
              break;
            
              case Bf_Z_Axis_Control:
                  if(BUFFER_Z_AXIS_CONTROL.bFlag_Update==eTRUE)
                  {
                      PARA8_CMD_TYPE=P2TCMD_Z_AXIS_CONTROL;
                      PARA16_1      =BUFFER_Z_AXIS_CONTROL.Axis_PositionControl;
                      PARA8_3       =BUFFER_Z_AXIS_CONTROL.Axis_Speed;
    //                  PARA8_4       =BUFFER_Z_AXIS_CONTROL.Axis_bDirection; //??? khong biet co nen send khong!!!
                      
                      UART_MakeData(UART1_BUFFER_TX,PARA8_CMD_TYPE,0,PARA16_1,0,PARA8_3,PARA8_4,0,0);
                      UART1_Send_BUF(UART1_BUFFER_TX,i_UART_TX); 
                  }
              break; 
				
			  case Bf_StateButton1 :
				 if(BUFFER_STATEBUTTON.Flag_Stop_Update == eTRUE)
					{
					  BUFFER_STATEBUTTON.Flag_Stop_Update == eFALSE;
					  PARA8_1 = BUFFER_STATEBUTTON.bflag_Stop;
					  UART_MakeData(UART2_BUFFER_TX,0,0,PARA8_1,0,0,0,0,0);
					  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);
					}
				if(BUFFER_STATEBUTTON.Flag_Pause_Update == eTRUE)
					{
					  BUFFER_STATEBUTTON.Flag_Pause_Update == eFALSE;
					  PARA8_1 = BUFFER_STATEBUTTON.bflag_Pause;
					  UART_MakeData(UART2_BUFFER_TX,0,0,PARA8_1,0,0,0,0,0);
					  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX); 
					} 

			  break;
			  default :
			  break;
          }
          
          //USART 2 - send to Slave2
          switch (CntUartBufferTx)
          {
              case Bf_Control_DC_Spindle:
                  PARA8_CMD_TYPE=P2TCMD_SPINDLE;
                  if(BUFFER_CONTROL_DC_SPINDLE.Flag_Update==eTRUE)
                      PARA8_1   =BUFFER_CONTROL_DC_SPINDLE.bProcess;
                  
                  PARA8_2       =BUFFER_CONTROL_DC_SPINDLE.Speed_DC;
                  PARA8_3       =BUFFER_CONTROL_DC_SPINDLE.bDC_Driection;//??? khong biet co nen send khong!!!
                  
                  UART_MakeData(UART2_BUFFER_TX,PARA8_CMD_TYPE,0,PARA8_1,PARA8_2,PARA8_3,0,0,0);
                  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);    
              break;
            
              case Bf_Current_Measure :
                  if(BUFFER_CURRENT_MEASURE.Flag_Update==eTRUE)
                  {
                      PARA8_CMD_TYPE= P2TCM_CURRENT_MEASURE;
                      PARA16_1      = BUFFER_CURRENT_MEASURE.Current_Max;
                      UART_MakeData(UART2_BUFFER_TX,PARA8_CMD_TYPE,0,PARA16_1,0,0,0,0,0);
                      UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);
                  }
              break;
              
			  case Bf_StateButton2:
				 if(BUFFER_STATEBUTTON.Flag_Stop_Update == eTRUE)
					{
					  BUFFER_STATEBUTTON.Flag_Stop_Update == eFALSE;
					  PARA8_1 = BUFFER_STATEBUTTON.bflag_Stop;
					  UART_MakeData(UART2_BUFFER_TX,0,0,PARA8_1,0,0,0,0,0);
					  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX);
					}
				if(BUFFER_STATEBUTTON.Flag_Pause_Update == eTRUE)
					{
					  BUFFER_STATEBUTTON.Flag_Pause_Update == eFALSE;
					  PARA8_1 = BUFFER_STATEBUTTON.bflag_Pause;
					  UART_MakeData(UART2_BUFFER_TX,0,0,PARA8_1,0,0,0,0,0);
					  UART2_Send_BUF(UART2_BUFFER_TX,i_UART_TX); 
					}
			  break;
			
              default :
              break;
          }
          
          
          
          

          
//          switch (CntUartBufferTx)
//          {
//              //send to Slave1-USART1 and Slave2-USART2
//              case Bf_ConfigParamater:
//              break;
//            
//              //send to PC-USART3
//
//            
//            
//              //send to Slave1-USART1
//              
//              
//              //send to Slave2-USART2
//
//
//
//          }
   
      }
}



