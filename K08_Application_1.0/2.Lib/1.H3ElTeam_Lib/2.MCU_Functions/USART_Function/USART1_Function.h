/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART1_Function.h
* Author             : OS Team
* Date First Issued  : 23/09/2014
* Description        : 

********************************************************************************
* History: DD/MM/YY
* 23/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_USART1_Function__H
#define _USART1_Function__H
/* Include global header file */
#include "USART1_Function_Profile.h"
#ifdef USE_USART1
	/* Prototype function */
	void USART1_Init( void );
	void USART1_Close( void );
	void USART1_StartSend( void );
	void USART1_StopSend( void );
	void USART1_StartReceive( void );
	void USART1_StopReceive( void );
	void USART1_Send_Byte_TX_RingBuf( unsigned char cChar );
	//unsigned char USART1_Get_Byte_RX_RingBuf( void );
	enumbool USART1_Get_Byte_RX_RingBuf(  char *pChar  );
	void USART1_IRQHandler( void );
#endif /* USE_USART1 */
#endif /* _USART1_Function__H */