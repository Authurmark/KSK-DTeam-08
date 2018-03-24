/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART3_Appcall_Function.c
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
#ifndef	_USART3_Appcall_Function__C
#define _USART3_Appcall_Function__C
/* Include global header file */
#include "USART3_AppCall_Function.h"
#include <stdio.h>
/* USART3 AppCall function */
#ifdef USE_USART3
/* USART3 AppCall Init */
void USART3_AppCall_Init(UART_Struct *pUART)
{
	/* Pointer function */
	pUART->open 				= &USART3_Init;
	pUART->close 				= &USART3_Close;
	pUART->start_send			= &USART3_StartSend;
	pUART->stop_send			= &USART3_StopSend;
	pUART->start_receive		= &USART3_StartReceive;
	pUART->stop_receive			= &USART3_StopReceive;
	pUART->read_byte 			= &USART3_AppCall_GetByte;
	pUART->send_byte 			= &USART3_AppCall_SendByte;
	pUART->send_buf				= &USART3_AppCall_SendBuf;
	pUART->send_str				= &USART3_AppCall_SendString;
	/* Init UART1*/
	pUART->open();
}
/* USART3 AppCall Send Buf */
void USART3_AppCall_SendString( unsigned char *str )
{
	/* Get Strlen and send buf */
	USART3_AppCall_SendBuf(str,strlen(str));
}
/* USART3 AppCall Send Buf */
void USART3_AppCall_SendBuf( unsigned char *buf, uint16_t uNumberByteSend )
{
	uint16_t utemp;
	/* Put all content to Ringbuf TX */
	for(utemp=0;utemp<uNumberByteSend;utemp++)
	{
		USART3_Send_Byte_TX_RingBuf(buf[utemp]);
	}
	/* Start Send USART3 */
	USART3_StartSend();
}
/* USART3 AppCall Get buf */
void USART3_AppCall_GetBuf( unsigned char *buf, uint16_t uNumberByteGet)
{
	uint16_t utemp;
	/* Get content from Ringbuf RX */
	for(utemp=0;utemp<uNumberByteGet;utemp++)
	{
		USART3_Get_Byte_RX_RingBuf(&buf[utemp]);
	}
}
/* USART3 AppCall Send Byte */
void USART3_AppCall_SendByte( unsigned char cChar)
{
	/* Put char to RingBuf */
	USART3_Send_Byte_TX_RingBuf(cChar);
	/* Start Send USART3 */
	USART3_StartSend();
}
/* USART3 AppCall Get Byte */
enumbool USART3_AppCall_GetByte( char *pChar )
{
	/* Get char from RingBuf */
	return USART3_Get_Byte_RX_RingBuf(pChar);
}
#endif /* USE_USART3 */
#endif /* USART3_Appcall_Function */