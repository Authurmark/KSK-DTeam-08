/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART2_Function.h
* Author             : OS Team
* Date First Issued  : 23/09/2014
* Description        : 

********************************************************************************
* History: DD/MM/YY
* 23/09/2014: V0.1
* 01/10/2015: V0.2 Change for USART2
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_USART2_Function__H
#define _USART2_Function__H
/* Include global header file */
#include "USART2_Function_Profile.h"
#ifdef USE_USART2
	/* Prototype function */
	void USART2_Init( void );
	void USART2_Close( void );
	void USART2_StartSend( void );
	void USART2_StopSend( void );
	void USART2_StartReceive( void );
	void USART2_StopReceive( void );
	void USART2_Send_Byte_TX_RingBuf( unsigned char cChar );
	//unsigned char USART2_Get_Byte_RX_RingBuf( void );
	enumbool USART2_Get_Byte_RX_RingBuf(  char *pChar  );
	void USART2_IRQHandler( void );
#endif /* USE_USART2 */
#endif /* _USART2_Function__H */