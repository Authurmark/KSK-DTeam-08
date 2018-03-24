/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UART4_Function.h
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
#ifndef	_UART4_Function__H
#define _UART4_Function__H
/* Include global header file */
#include "UART4_Function_Profile.h"
#ifdef USE_UART4
	/* Prototype function */
	void UART4_Init( void );
	void UART4_Close( void );
	void UART4_StartSend( void );
	void UART4_StopSend( void );
	void UART4_StartReceive( void );
	void UART4_StopReceive( void );
	void UART4_Send_Byte_TX_RingBuf( unsigned char cChar );
	enumbool UART4_Get_Byte_RX_RingBuf( char *pChar );
	void UART4_IRQHandler( void );
#endif /* USE_UART4 */
#endif /* _UART4_Function__H */