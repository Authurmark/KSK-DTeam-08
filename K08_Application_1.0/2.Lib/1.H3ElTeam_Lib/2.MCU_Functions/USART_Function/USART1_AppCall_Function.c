/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART1_Appcall_Function.c
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
#ifndef	_USART1_Appcall_Function__C
#define _USART1_Appcall_Function__C
/* Include global header file */
#include "USART1_AppCall_Function.h"
#include <stdio.h>
/* USART1 AppCall function */
#ifdef USE_USART1
/* USART1 AppCall Init */
void USART1_AppCall_Init(UART_Struct *pUART)
{
	/* Pointer function */
	pUART->open 				= &USART1_Init;
	pUART->close 				= &USART1_Close;
	pUART->start_send			= &USART1_StartSend;
	pUART->stop_send			= &USART1_StopSend;
	pUART->start_receive		= &USART1_StartReceive;
	pUART->stop_receive			= &USART1_StopReceive;
	pUART->read_byte 			= &USART1_AppCall_GetByte;
	pUART->send_byte 			= &USART1_AppCall_SendByte;
	pUART->send_buf				= &USART1_AppCall_SendBuf;
	pUART->send_str				= &USART1_AppCall_SendString;
	/* Init UART1*/
	pUART->open();
}
/* USART1 AppCall Send Buf */
void USART1_AppCall_SendString( unsigned char *str )
{
	/* Get Strlen and send buf */
	USART1_AppCall_SendBuf(str,strlen(str));
}
/* USART1 AppCall Send Buf */
void USART1_AppCall_SendBuf( unsigned char *buf, uint16_t uNumberByteSend )
{
	uint16_t utemp;
	/* Put all content to Ringbuf TX */
	for(utemp=0;utemp<uNumberByteSend;utemp++)
	{
		USART1_Send_Byte_TX_RingBuf(buf[utemp]);
	}
	/* Start Send USART1 */
	USART1_StartSend();
}
/* USART1 AppCall Get buf */
void USART1_AppCall_GetBuf( unsigned char *buf, uint16_t uNumberByteGet)
{
	uint16_t utemp;
	/* Get content from Ringbuf RX */
	for(utemp=0;utemp<uNumberByteGet;utemp++)
	{
		USART1_Get_Byte_RX_RingBuf(&buf[utemp]);
	}
}
/* USART1 AppCall Send Byte */
void USART1_AppCall_SendByte( unsigned char cChar)
{
	/* Put char to RingBuf */
	USART1_Send_Byte_TX_RingBuf(cChar);
	/* Start Send USART1 */
	USART1_StartSend();
}
/* USART1 AppCall Get Byte */
enumbool USART1_AppCall_GetByte( char *pChar )
{
	/* Get char from RingBuf */
	return USART1_Get_Byte_RX_RingBuf(pChar);
}
/*@brief  Retargets the C library printf function to the USART.*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART1_AppCall_SendByte(ch);
	return ch;
}
#endif /* USE_USART1 */
#endif /* USART1_Appcall_Function */