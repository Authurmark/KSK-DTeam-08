/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UART4_AppCall_Function.h
* Author             : OS Team
* Date First Issued  : 26/09/2014
* Description        : 

********************************************************************************
* History: DD/MM/YY
* 26/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_UART4_Appcall_Function__H
#define _UART4_Appcall_Function__H
/* Include global header file */
#include "UART4_Function.h"
	/* Prototype function */
	void UART4_AppCall_Init(UART_Struct *pUART);
	void UART4_AppCall_SendString( unsigned char *str);
	void UART4_AppCall_SendBuf( unsigned char *buf, uint16_t uNumberByteSend);
	void UART4_AppCall_GetBuf( unsigned char *buf, uint16_t uNumberByteGet);
	void UART4_AppCall_SendByte( unsigned char cChar);
	//unsigned char UART4_AppCall_GetByte( void );
	enumbool UART4_AppCall_GetByte( char *pChar );
#endif /* UART4_Appcall_Function */