/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART2_Appcall_Function.c
* Author             : OS Team
* Date First Issued  : 26/09/2014
* Description        : 

********************************************************************************
* History: DD/MM/YY
* 26/09/2014: V0.1
* 01/10/2015: V0.2 Change for USART2
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_USART2_Appcall_Function__C
#define _USART2_Appcall_Function__C
/* Include global header file */
#include "USART2_AppCall_Function.h"
#include <stdio.h>
/* USART2 AppCall function */
#ifdef USE_USART2
/* USART2 AppCall Init */
void USART2_AppCall_Init(UART_Struct *pUART)
{
	/* Pointer function */
	pUART->open 				= &USART2_Init;
	pUART->close 				= &USART2_Close;
	pUART->start_send			= &USART2_StartSend;
	pUART->stop_send			= &USART2_StopSend;
	pUART->start_receive		= &USART2_StartReceive;
	pUART->stop_receive			= &USART2_StopReceive;
	pUART->read_byte 			= &USART2_AppCall_GetByte;
	pUART->send_byte 			= &USART2_AppCall_SendByte;
	pUART->send_buf				= &USART2_AppCall_SendBuf;
	pUART->send_str				= &USART2_AppCall_SendString;
	/* Init UART1*/
	pUART->open();
}
/* USART2 AppCall Send Buf */
void USART2_AppCall_SendString( unsigned char *str )
{
	/* Get Strlen and send buf */
	USART2_AppCall_SendBuf(str,strlen(str));
}
/* USART2 AppCall Send Buf */
void USART2_AppCall_SendBuf( unsigned char *buf, uint16_t uNumberByteSend )
{
	uint16_t utemp;
	/* Put all content to Ringbuf TX */
	for(utemp=0;utemp<uNumberByteSend;utemp++)
	{
		USART2_Send_Byte_TX_RingBuf(buf[utemp]);
	}
	/* Start Send USART2 */
	USART2_StartSend();
}
/* USART2 AppCall Get buf */
void USART2_AppCall_GetBuf( unsigned char *buf, uint16_t uNumberByteGet)
{
	uint16_t utemp;
	/* Get content from Ringbuf RX */
	for(utemp=0;utemp<uNumberByteGet;utemp++)
	{
		USART2_Get_Byte_RX_RingBuf(&buf[utemp]);
	}
}
/* USART2 AppCall Send Byte */
void USART2_AppCall_SendByte( unsigned char cChar)
{
	/* Put char to RingBuf */
	USART2_Send_Byte_TX_RingBuf(cChar);
	/* Start Send USART2 */
	USART2_StartSend();
}
/* USART2 AppCall Get Byte */
enumbool USART2_AppCall_GetByte( char *pChar )
{
	/* Get char from RingBuf */
	return USART2_Get_Byte_RX_RingBuf(pChar);
}
#endif /* USE_USART2 */
#endif /* USART2_Appcall_Function */