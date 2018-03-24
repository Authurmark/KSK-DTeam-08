/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART3_Function.h
* Author             : OS Team
* Date First Issued  : 23/09/2014
* Description        : 

********************************************************************************
* History: DD/MM/YY
* 23/09/2014: V0.1
* 01/10/2015: V0.2 Change for USART3
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_USART3_Function__H
#define _USART3_Function__H
/* Include global header file */
#include "USART3_Function_Profile.h"
#ifdef USE_USART3
	/* Prototype function */
	void USART3_Init( void );
	void USART3_Close( void );
	void USART3_StartSend( void );
	void USART3_StopSend( void );
	void USART3_StartReceive( void );
	void USART3_StopReceive( void );
	void USART3_Send_Byte_TX_RingBuf( unsigned char cChar );
	//unsigned char USART3_Get_Byte_RX_RingBuf( void );
	enumbool USART3_Get_Byte_RX_RingBuf(  char *pChar  );
	void USART3_IRQHandler( void );
#endif /* USE_USART3 */
#endif /* _USART3_Function__H */