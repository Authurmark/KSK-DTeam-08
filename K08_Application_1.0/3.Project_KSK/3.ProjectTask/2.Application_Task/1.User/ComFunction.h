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
    P2TCMD_Control_X_axis     = 0x02,
    P2TCMD_Control_Y_axis     = 0x03,
    P2TCMD_Control_Z_axis     = 0x04,
   	P2TCMD_FEEDBACK		      = 0x41,
    P2TCMD_AXIS_PROCESS	  	  = 0x01,
	P2TCMD_StateButton        = 0x05,
  
    /*Common command*/
    P2TCMD_CLOSE = 0x01,
    P2TCMD_CONNECT = 0x02,
    
    /* COMMAND FOR RELAY */
    P2TCMD_SET_RELAY_PARA = 0xC1,
    P2TCMD_SET_LED_PARA = 0xC2,	
    P2TCMD_SET_RELAY_DIRECT = 0xC3,
    P2TCMD_SET_LED_DIRECT = 0xC4,
    
    /* Info */
    P2TCMD_INFO = '?',
}cmd_type;
/* stepmotor*/
typedef enum {
    ResetHome 	   	= 0x01,
   	ReleaseCutter   = 0x02,
	GetCutter		= 0x03,
	RunForScan	 	= 0x04,
	RunToPoint    	= 0x05,
	Stop		  	= 0x07,
    Ready         	= 0x06,
}state_process;

typedef enum {
    STEP_FORWARD = 0x01,
    STEP_REVERSE = 0x02,
    STEP_BREAK   = 0x03,
    STEP_DISABLE = 0x04,
}state_step_motor;

typedef enum{
	bFlag_EndStop1 	= 0x01,
	bFlag_EndStop2 	= 0x02,
	None			= 0x03,
}FlagEndStop;

typedef enum{
	E_OverLoad		= 0x01,
	E_OverJourney	= 0x02,
	E_OverTime		= 0x03,
}eError_Process;

typedef struct{
	state_process		bProcess_Axis;
	eError_Process		Error_Process;
	enumbool			bFlag_Process_Update;
	enumbool       		bFlag_Process_Info;	
}Buffer_Motor_Control_Process;
extern Buffer_Motor_Control_Process  BUFFER_MOTOR_CONTROL_PROCESS;

typedef struct{
uint16				PositionControl;				                                      	//Recieve from master
uint16				PositionGet;					                                     	//Send to master
uint8				Speed;																	//Recieve from master			
state_step_motor 	bDirection;	
 enumbool       	bFlag_Process_Info;														//Recieve from master
}Buffer_Control_Axis;
extern Buffer_Control_Axis BUFFER_CONTROL_X_AXIS;
extern Buffer_Control_Axis BUFFER_CONTROL_Y_AXIS;
extern Buffer_Control_Axis BUFFER_CONTROL_Z_AXIS;

/*Receive Status Button From Master*/
typedef struct{
uint8 bflag_Stop;                                    										//Receive from master
uint8 bflag_Pause; 																			//Receive from master	
}buffer_StateButton;
extern buffer_StateButton BUFFER_STATEBUTTON ;


/*--------------------------------------------------------------------*/
//-----------------------FUNCTION PROTOTYPE--------------------------//
/*------------------------------------------------------------------*/

/* RX BUFFER HANDLE */
enumbool vComDataProcess_USART1(void);
enumbool vComDataProcess_USART2(void);

void vComDivideBlockData(uint8 *UART_BUFFER_RX, uint8 *UART_BUFFER_TX,UART_Struct pUART );

/* FEEDBACK HANDLE */
void UART_Comm_Feedback_Command_Content(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE, uint16 CODE);
void vFeedBack_info_sys(void);

/* MAKE DATA*/

void UART_MakeData(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE, uint8 CMD,uint32 PARA1, uint32 PARA2, uint32 PARA3, uint32 PARA4, uint32 PARA5, uint32 PARA6);
void UART_MakeData_Head(uint8 *UART_BUFFER_TX,cmd_type CMD_TYPE);
void UART_MakeData_8bit(uint8 *UART_BUFFER_TX,uint8 iIndex, uint8 DATA);
void UART_MakeData_16bit(uint8 *UART_BUFFER_TX,uint8 iIndex, uint16 DATA);
void UART_MakeData_Tail(uint8 *UART_BUFFER_TX);
/*Detect FeedBack*/
void vInitFeedBackDetectOverTime(void);
void vFeedBackDetectOverTime(void);
/* TX BUFFER HANDLE*/
extern uint8 CntUartBufferTx;
void vMakeBufferTXTask( void *pvParameters );

/* Extern flag */
extern enumbool bFlagGetCommandLEDConfigUART1;
extern structIO_Manage_Output bLEDConfigCommand;
/*EXTERN VALUE OF AXIS X, Y ,Z*/
extern uint32 X_Axis_PositionGet;
extern uint32 Y_Axis_PositionGet;


