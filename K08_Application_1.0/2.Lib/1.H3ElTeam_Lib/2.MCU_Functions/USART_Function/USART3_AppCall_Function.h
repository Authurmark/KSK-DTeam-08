/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART3_AppCall_Function.h
* Author             : OS Team
* Date First Issued  : 26/09/2014
* Description        : 

********************************************************************************
* History: DD/MM/YY
* 26/09/2014: V0.1
* 01/10/2015: V0.2 Change for USART3
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_USART3_Appcall_Function__H
#define _USART3_Appcall_Function__H
/* Include global header file */
#include "USART3_Function.h"
	/* Prototype function */
	void USART3_AppCall_Init(UART_Struct *pUART);
	void USART3_AppCall_SendString( unsigned char *str);
	void USART3_AppCall_SendBuf( unsigned char *buf, uint16_t uNumberByteSend);
	void USART3_AppCall_GetBuf( unsigned char *buf, uint16_t uNumberByteGet);
	void USART3_AppCall_SendByte( unsigned char cChar);
	//unsigned char USART3_AppCall_GetByte( void );
	enumbool USART3_AppCall_GetByte( char *pChar );
#endif /* USART3_Appcall_Function */