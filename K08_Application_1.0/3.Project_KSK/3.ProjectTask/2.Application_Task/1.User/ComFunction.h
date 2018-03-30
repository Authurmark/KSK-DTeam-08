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

/*UART Frame*/
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
#define FBCODE_OK           0xC7C7/* Bootloader main */
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
/* Define for prototype */
#define pUART_CONFIG pUSART1
#define UART_Send_BUF pUSART1.send_buf
#define UART_Send_STRING pUSART1.send_str
#define T1Us_Tick1Ms vGetCurrentCounterTimeBaseMs()

/* Define TYPE COMMAND*/
#define STRING_COMMAND
#define FORMAT_COMMAND

/* For USART Bootloader */
typedef enum Cmd_Type {
    P2TCMD_SPINDLE      = 0x10,
    P2TCMD_TEST         = 0x11,


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
/* Function Prototype */
void vUserTaskProcess(void);
/* Prototype for function */
void UART_Comm(void);
void UART_Comm_Feedback_Command_Content(cmd_type CMD, uint16 CODE, uint16 uLengByte, char* pSend);
void UART_MakeData(cmd_type CMD, uint16 CODE, uint16 uLengByteContent, char* pSend);
void UART_MakeFeedback(cmd_type CMD, uint16 CODE, uint16 uLengByte);
enumbool vComPortProcess(void);
void vComCommandProcess(void);

enumbool vComDataProcess(void);
void vComDataHandle(void);
void vFeedBack_info_sys(void);
void vComDivideBlockData(void);

void UART_MakeData_Head(cmd_type CMD);
void UART_MakeData_8bit(Data);
void UART_MakeData_16bit(Data);
void UART_MakeData_Tail();



/* Extern flag */
extern enumbool bFlagGetCommandLEDConfigUART1;
extern structIO_Manage_Output bLEDConfigCommand;