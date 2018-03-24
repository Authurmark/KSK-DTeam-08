/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UART4_Appcall_Function.c
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
#ifndef	_UART4_Appcall_Function__C
#define _UART4_Appcall_Function__C
/* Include global header file */
#include "UART4_AppCall_Function.h"
#include <stdio.h>
/* UART4 AppCall function */
#ifdef USE_UART4
/* UART4 AppCall Init */
void UART4_AppCall_Init(UART_Struct *pUART)
{
	/* Pointer function */
	pUART->open 				= &UART4_Init;
	pUART->close 				= &UART4_Close;
	pUART->start_send			= &UART4_StartSend;
	pUART->stop_send			= &UART4_StopSend;
	pUART->start_receive		= &UART4_StartReceive;
	pUART->stop_receive			= &UART4_StopReceive;
	pUART->read_byte 			= &UART4_AppCall_GetByte;
	pUART->send_byte 			= &UART4_AppCall_SendByte;
	pUART->send_buf				= &UART4_AppCall_SendBuf;
	pUART->send_str				= &UART4_AppCall_SendString;
	/* Init UART1*/
	pUART->open();
}
/* UART4 AppCall Send Buf */
void UART4_AppCall_SendString( unsigned char *str )
{
	/* Get Strlen and send buf */
	UART4_AppCall_SendBuf(str,strlen(str));
}
/* UART4 AppCall Send Buf */
void UART4_AppCall_SendBuf( unsigned char *buf, uint16_t uNumberByteSend )
{
	uint16_t utemp;
	/* Put all content to Ringbuf TX */
	for(utemp=0;utemp<uNumberByteSend;utemp++)
	{
		UART4_Send_Byte_TX_RingBuf(buf[utemp]);
	}
	/* Start Send UART4 */
	UART4_StartSend();
}
/* UART4 AppCall Get buf */
void UART4_AppCall_GetBuf( unsigned char *buf, uint16_t uNumberByteGet)
{
	uint16_t utemp;
	/* Get content from Ringbuf RX */
	for(utemp=0;utemp<uNumberByteGet;utemp++)
	{
		//buf[utemp]=UART4_Get_Byte_RX_RingBuf();
		UART4_Get_Byte_RX_RingBuf(&buf[utemp]);
	}
}
/* UART4 AppCall Send Byte */
void UART4_AppCall_SendByte( unsigned char cChar)
{
	/* Put char to RingBuf */
	UART4_Send_Byte_TX_RingBuf(cChar);
	/* Start Send UART4 */
	UART4_StartSend();
}
/* UART4 AppCall Get Byte */
enumbool UART4_AppCall_GetByte( char *pChar )
{
	/* Get char from RingBuf */
	return UART4_Get_Byte_RX_RingBuf(pChar);
}
#endif /* USE_UART4 */
#endif /* UART4_Appcall_Function */