/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART3_Function_Profile.h
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
#ifndef	_USART3_Function_Profile__H
#define _USART3_Function_Profile__H
/* Include global header file */
#include "Common.h"
/* Define USART3 Function */ 
/* Uncomment to use USART3, comment to disable*/
//#define USE_USART3  
/* Uncomment to use default, comment to use alternative*/
	#define USE_DEFAULT_USART3_IO
	/* Uncomment to use USE_CLOCK_USART3, comment to disable*/
	//#define USE_CLOCK_USART3
	/* Uncomment to use interrupt , comment to disable*/
	#define USE_TX_INTERRUPT_USART3
	#define USE_RX_INTERRUPT_USART3
	/* Uncomment to use DMA transmit, comment to disable*/
	//#define USE_DMA_USART3  
	/* Define size of RX, TX buffer */
	#define DEFAULT_USART3_NUMBER_BYTES_RX_BUFFER 400
	#define DEFAULT_USART3_NUMBER_BYTES_TX_BUFFER 800
	/* Uncomment to use TX_RINGBUF, comment to disable */
	#define USE_TX_RINGBUF
/* Define USART3 Function */	
#ifdef USE_USART3
	#define USART3_RCC			RCC_APB1Periph_USART3
	#define USART3_CLK_CMD		RCC_APB1PeriphClockCmd

	#ifdef	USE_DEFAULT_USART3_IO
		#define USART3_RX_PORT		GPIOB
		#define USART3_RX_PIN		GPIO_Pin_11
		#define USART3_RX_RCC_PORT	RCC_APB2Periph_GPIOB
		#define USART3_RX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define USART3_RX_MODE		GPIO_Mode_IN_FLOATING
		#define USART3_RX_SPEED		GPIO_Speed_50MHz

		#define USART3_TX_PORT		GPIOB
		#define USART3_TX_PIN		GPIO_Pin_10
		#define USART3_TX_RCC_PORT	RCC_APB2Periph_GPIOB
		#define USART3_TX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define USART3_TX_MODE		GPIO_Mode_AF_PP
		#define USART3_TX_SPEED		GPIO_Speed_50MHz
	#endif /* USE_DEFAULT_USART3_IO */

	/* Extern global variable */
	extern USART_InitTypeDef USART3_InitStructure;
	extern UART_Struct pUSART3;
	
	#ifdef USE_CLOCK_USART3 
	extern USART_ClockInitTypeDef USART3_ClockInitStructure;
	#endif /* USE_CLOCK_USART3 */

	#ifdef USE_TX_INTERRUPT_USART3
		#define USART3_TX_INTERRUPT_PRE_PRIORITY	2
		#define USART3_TX_INTERRUPT_SUB_PRIORITY	1
	#endif /* USE_TX_INTERRUPT_USART3 */
		
	#ifdef USE_RX_INTERRUPT_USART3
		#define USART3_RX_INTERRUPT_PRE_PRIORITY	2
		#define USART3_RX_INTERRUPT_SUB_PRIORITY	2
	#endif /* USE_TX_INTERRUPT_USART3 */
	
	#ifdef USE_DMA_USART3
        /* Queue to send and recieve data xQueue_TX_USART3_Buffer, xQueue_RX_USART3_Buffer*/
        #define DMA_USART3_TX_CHANNEL		DMA1_Channel4
    	#define DMA_USART3_TX_COMPLETE_FLAG	DMA1_FLAG_TC4
        #define DMA_USART3_RX_CHANNEL		DMA1_Channel5
    	#define DMA_USART3_RX_COMPLETE_FLAG	DMA1_FLAG_TC5
	extern DMA_InitTypeDef DMA_USART3_TX_InitStructure;
	extern DMA_InitTypeDef DMA_USART3_RX_InitStructure;
	#endif /* USE_DMA_USART3 */
#endif /* USE_USART3 */
#endif/* USART3_Function_Profile */
/************************************************************************************************************/