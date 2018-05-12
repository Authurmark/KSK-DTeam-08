/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : ComFunction.h
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
#include "Project_Function.h"

/*--------------------------------------------------------------------*/
//----------------------DEFINE FOR PROTOTYPE-------------------------//
/*------------------------------------------------------------------*/
#define pUART1_CONFIG           pUSART1
#define UART1_Send_BUF          pUSART1.send_buf
#define UART1_Send_STRING       pUSART1.send_str

#define pUART2_CONFIG           pUSART2
#define UART2_Send_BUF          pUSART2.send_buf
#define UART2_Send_STRING       pUSART2.send_str

#define pUART3_CONFIG           pUSART3
#define UART3_Send_BUF          pUSART3.send_buf
#define UART3_Send_STRING       pUSART3.send_str

#define T1Us_Tick1Ms vGetCurrentCounterTimeBaseMs()

/*--------------------------------------------------------------------*/
//-------------------PROTOCOL USART WITH DATA FRAME------------------//
/*------------------------------------------------------------------*/

#define PACKAGE_SIZE 128
#define BLOCK_SIZE 0x80
#define NUMBER_PACKAGE_WRITE_FLASH (FLASH_PAGE_SIZE/PACKAGE_SIZE)
#define iUART_PRE_1         0
#define iUART_PRE_2         1
#define iUART_PRE_3         2
#define iUART_PRE_4         3
#define iUART_PRE_5         4
#define iUART_PRE_6         5

//#define iUART_CMD           6
//#define iUART_SIZE_LOW      7
//#define iUART_SIZE_HIGH     8
//#define iUART_IDX_LOW       9                           
//#define iUART_IDX_HIGH      10                          
//#define iUART_DATA          11

#define iUART_CMD_TYPE      6
#define iUART_CMD           7
#define iUART_SIZE_LOW      8
#define iUART_SIZE_HIGH     9
#define iUART_IDX_LOW       10
#define iUART_IDX_HIGH      11
#define iUART_DATA          12
#define iUART_END_DATA      18

#define i_MAX_UART          500
#define UART_INTERVAL       10  /*115200kbs=14400Bs~14byte/s*/
#define FBCODE_OK           0xC0C7/* Bootloader main */
//#define FBCODE_OK           0x7C7C/* FW main */
#define FBCODE_ERR          0xFFFF
#define FBCODE_RETURN       0x7E7E
#define FBCODE_WRONG_IDX
#define NORMAL_FEEDBACK_LENGTH	2

/* 13-08-2015 Change Preamble from 0x520xEA to 0x180x07 */
#define PREMABLE_BYTE_1 'A'
#define PREMABLE_BYTE_2 'T'
#define PREMABLE_BYTE_3 'K'
#define PREMABLE_BYTE_4 'S'
#define PREMABLE_BYTE_5 'K'
#define PREMABLE_BYTE_6 '+'

#define END_DATA_BYTE   '~'

/* Define TYPE COMMAND*/
#define STRING_COMMAND
#define FORMAT_COMMAND

/* For USART Bootloader */
typedef enum Cmd_Type {
    P2TCMD_TEST                 = 0xA0,
    
    P2TCMD_SPINDLE              = 0x10,
    P2TCMD_MACHINE              = 0x12,
    P2TCMD_HOLE_DATA            = 0x13,
    P2TCMD_ENCODER_HOME         = 0x14,
    P2TCMD_AXIS_PROCESS         = 0x15,
    
    P2TCMD_X_AXIS_CONTROL       = 0x16,
    P2TCMD_Y_AXIS_CONTROL       = 0x17,
    P2TCMD_Z_AXIS_CONTROL       = 0x18,
    
    P2TCM_CURRENT_MEASURE       = 0x19,

    
    P2TCM_FEEDBACK_DATA         = 0xA1,
    P2TCM_ERROR_GET             = 0xA2,
    /*Common command*/
    P2TCMD_CLOSE                = 0x01,
    P2TCMD_CONNECT              = 0x02,
    
    /* COMMAND FOR RELAY */
    P2TCMD_SET_RELAY_PARA       = 0xC1,
    P2TCMD_SET_LED_PARA         = 0xC2,
    P2TCMD_SET_RELAY_DIRECT     = 0xC3,
    P2TCMD_SET_LED_DIRECT       = 0xC4,
    
    /* Info */
    P2TCMD_INFO                 = '?',
}cmd_type;



/*--------------------------------------------------------------------*/
//-----------------------FUNCTION PROTOTYPE--------------------------//
/*------------------------------------------------------------------*/

/* RX BUFFER HANDLE */
enumbool vComDataProcess_USART1(void);
enumbool vComDataProcess_USART2(void);
enumbool vComDataProcess_USART3(void);

void vComDivideBlockData(uint8 *UART_BUFFER_RX, uint8 *UART_BUFFER_TX,UART_Struct pUART );

/* FEEDBACK HANDLE */
void UART_Comm_Feedback_Command_Content(UART_Struct pUSART_USE,uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE,uint8 CMD, uint16 CODE);
void vFeedBack_info_sys(void);

extern void vInitFeedBackDetectOverTime(void);
extern void vFeedBackDetectOverTime(void);

/* MAKE DATA*/
void UART_MakeData(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE, uint8 CMD,uint16 PARA1, uint16 PARA2, uint16 PARA3, uint16 PARA4, uint16 PARA5, uint16 PARA6);

void UART_MakeData_Head(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE);
void UART_MakeData_8bit(uint8 *UART_BUFFER_TX,uint8 iIndex, uint8 DATA);
void UART_MakeData_16bit(uint8 *UART_BUFFER_TX,uint8 iIndex, uint16 DATA);
void UART_MakeData_Tail(uint8 *UART_BUFFER_TX);

/* TX BUFFER HANDLE*/
extern uint8 CntUartBufferTx;
void vMakeBufferTXTask( void *pvParameters );

/* Extern flag */
extern enumbool bFlagGetCommandLEDConfigUART1;
extern structIO_Manage_Output bLEDConfigCommand;


/*------------------------------------------------------------------------*/
//--------------------DEFINE AND PROTOTYPE BUFFER DATA-------------------//
/*----------------------------------------------------------------------*/

/* CONFIG PARAMATER */




/* TEST DATA */
#define TEST_1  0x01
#define TEST_2  0x02

/*Buffer Data for Pause/Run and stop*/
typedef struct{
uint8 bflag_Stop;                                    //send trang thái nut nhan Stop and Pause/Run cho slave1 và slave2
uint8 bflag_Pause; 	
}buffer_StateButton;
extern buffer_StateButton BUFFER_STATEBUTTON ;







/*Hole data*/
#define NUM_HOLE     40

typedef enum {
  UNKNOW        = 0x01,
  COUNTERBORE   = 0x02,
  COUNTERSINK   = 0x03,
  STRAIGHTHOLE  = 0x04,
  THREADHOLE    = 0x05,  
}bType_hole;

typedef enum {
  NOTTHREAD     = 0x00,
  M3            = 0x01,
  M4            = 0x02,
  M5            = 0x03,
  M6            = 0x04,
  M8            = 0x05,
}bType_ThreadHole;

typedef enum {
  GOOD          = 0x01,
  POOR          = 0x02,
}bQualityHole;

typedef struct{
  bType_hole            bType_hole[NUM_HOLE];                   //
  bType_ThreadHole      Dia_Hole[NUM_HOLE];                     //recieve from PC
  uint16                Position_X[NUM_HOLE];                   //recieve from PC
  uint16                Position_Y[NUM_HOLE];                   //recieve from PC
  bQualityHole          Hole_Check_Value[NUM_HOLE];             //send    to   PC
  uint16                Hole_Check_Height[NUM_HOLE];            //send    to   PC
  enumbool              Detect_Thread_Hole[NUM_HOLE];           //send    to   PC
  uint8                 iIdex_hole_data;                        //recieve/send with PC
  uint8                 Counter_Hole[5];                        //send    to   PC ???
}buffer_hole_data;

extern buffer_hole_data BUFFER_HOLE_DATA;


















/* Control Process */
#define iIndex_M3       0
#define iIndex_M4       1
#define iIndex_M5       2
#define iIndex_M6       3
#define iIndex_M8       4

typedef enum {
  Exist         = 0x01,
  Not_Exist     = 0x00,
}bstatus_cutter;

typedef enum {
  PMachine_ResetHome            =0x01,
  PMachine_ResetCutter          =0x02,
  PMachine_ScanHole             =0x04,
  PMachine_ThreadChecker        =0x05,
  PMachine_Ready                =0x06,
  PMachine_Reset                =0x07,
  PMachine_Pause                =0x08,
  PMachine_Disable              =0x09,
  PMachine_FinishProcess        =0x0A,
}bProcess_Machine;

typedef enum {
  Cutter_M3            = 0x01,
  Cutter_M4            = 0x02,
  Cutter_M5            = 0x03,
  Cutter_M6            = 0x04,
  Cutter_M8            = 0x05,
}bType_Cutter;

typedef enum {
    eMachine_OverLoad           = 0x01,
    eMachine_OverTime           = 0x02,
    eMachine_OverJourney        = 0x03,
}bErrorMachine;

typedef struct {
  bProcess_Machine      bProcess_Control_Machine;               //recieve from PC ---- only receive data from PC, not modify
  bProcess_Machine      bProcess_Feedback_Machine;              //send    to   PC ---- only feedback status process machine, not use to control
  uint8                 iIdex_hole_check;                       //recieve from PC
  bstatus_cutter        sStatus_Cutter[5];                      //send    to   PC
  bType_Cutter          sHold_Cutter;                           //send    to   PC
  uint16                PositionCutter_X[5];
  uint16                PositionCutter_Y[5];
  bErrorMachine         bErrorMachine;                          //send    to   PC
  enumbool              eFlag_Process_Update;                   //        ---- update when system receive new command from PC
}strbuffer_machine_control;

extern strbuffer_machine_control BUFFER_MACHINE_CONTROL;           //buffer send to PC once in a second










/*Motor Control 3-Axis Process*/
typedef enum {
    eMotor_OverLoad           = 0x01,
    eMotor_OverTime           = 0x02,
    eMotor_OverJourney        = 0x03,
}bErrorMotor;

typedef enum {
    eAxis_InProcess     =0x01,
    eAxis_Finsish       =0x02,
}bFeedBackAxis;

typedef enum {
    RESETHOME           = 0x01,
    RELEASECUTTER       = 0x02,
    GETCUTTER           = 0x03,
    RUNTOSCAN           = 0x04,
    RUNTOPOINT          = 0x05,
}state_axis_process;

typedef struct {
  state_axis_process    bProcess;                               //send    to   Slave1
  bFeedBackAxis         bFeedBackAxis;                          //recieve from Slave1
  bErrorMotor           bError;                                 //recieve from Slave1
  enumbool              Flag_Update;                            //tick Flag when bProcess change
  enumbool              bFlag_Process_Info; 
}Buffer_Axis_Process;


extern Buffer_Axis_Process BUFFER_AXIS_PROCESS;












/* Control X-Y-Z Axis */
#define Z_CheckThread   50
#define Z_SafePosition  100

typedef enum {
    AXIS_FORWARD = 0x01,
    AXIS_REVERSE = 0x02,
    AXIS_BREAK   = 0x03,
    AXIS_DISABLE = 0x04,
}state_axis_direction;

typedef struct{
  uint16                        Axis_PositionControl;           //send    to   Slave1
  uint8                         Axis_Speed;                     //send    to   Slave1
  state_axis_direction          Axis_bDirection;                //send    to   Slave1
  uint16                        Axis_PositionGet;               //recieve from Slave1
  enumbool                      bFlag_Process_Info;
  enumbool                      bFlag_Update;
}Buffer_Control_Axis;

extern Buffer_Control_Axis BUFFER_X_AXIS_CONTROL;
extern Buffer_Control_Axis BUFFER_Y_AXIS_CONTROL;
extern Buffer_Control_Axis BUFFER_Z_AXIS_CONTROL;









/* Control DC Spindle */
typedef enum {
    SPINDLE_RORATY = 0x01,
    SPINDLE_RESET  = 0x02,
    SPINDLE_STOP   = 0x03,
}state_spindle_process;

typedef enum {
    SPINDLE_FORWARD = 0x01,
    SPINDLE_REVERSE = 0x02,
    SPINDLE_BREAK   = 0x03,
    SPINDLE_DISABLE = 0x04,
}state_DC_Spindle;

typedef enum {
    eSpindle_OverLoad = 0x01,
    eSpindle_OverTime = 0x02,
}bErrorSpindle;

typedef struct{
  state_spindle_process         bProcess;                       //send    to   Slave2
  uint8                         Speed_DC;                       //send    to   Slave2
  state_DC_Spindle              bDC_Driection;                  //send    to   Slave2
  bErrorSpindle                 bError;                         //recieve from Slave2
  enumbool                      Flag_Update;                    //tick Flag when bProcess change
}Buffer_Control_DC_Spindle;

extern Buffer_Control_DC_Spindle BUFFER_CONTROL_DC_SPINDLE;











/* Current Measure Value */

typedef struct{
  uint16        Current_Value;                                  //recieve from Slave2
  uint16        Current_Max;                                    //send    to   Slave2
  enumbool      Flag_QualityPoor;                               //recieve from Slave2
  enumbool      Flag_Update;
  enumbool      bFlag_Process_Info;
}Buffer_Current_Measure;

extern Buffer_Current_Measure BUFFER_CURRENT_MEASURE;











/* Encoder Home Value */

typedef struct{
  enumbool      Flag_Home;                                      //recieve from Slave1
  enumbool      bFlag_Process_Info;
}Buffer_Encoder;

extern Buffer_Encoder  BUFFER_ENCODER;