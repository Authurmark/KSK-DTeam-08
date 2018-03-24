/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART1_Function_Profile.h
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
#ifndef	_USART1_Function_Profile__H
#define _USART1_Function_Profile__H
/* Include global header file */
#include "Common.h"
/* Define USART1 Function */ 
/* Uncomment to use USART1, comment to disable*/
#define USE_USART1  
/* Uncomment to use default, comment to use alternative*/
	#define USE_DEFAULT_USART1_IO
	/* Uncomment to use USE_CLOCK_USART1, comment to disable*/
	//#define USE_CLOCK_USART1
	/* Uncomment to use interrupt , comment to disable*/
	#define USE_TX_INTERRUPT_USART1
	#define USE_RX_INTERRUPT_USART1
	/* Uncomment to use DMA transmit, comment to disable*/
	//#define USE_DMA_USART1  
	/* Define size of RX, TX buffer */
	#define DEFAULT_USART1_NUMBER_BYTES_RX_BUFFER 400
	#define DEFAULT_USART1_NUMBER_BYTES_TX_BUFFER 800
	/* Uncomment to use TX_RINGBUF, comment to disable */
	#define USE_TX_RINGBUF
/* Define USART1 Function */	
#ifdef USE_USART1
	#define USART1_RCC			RCC_APB2Periph_USART1
	#define USART1_CLK_CMD		RCC_APB2PeriphClockCmd

	#ifdef	USE_DEFAULT_USART1_IO
		#define USART1_RX_PORT		GPIOA
		#define USART1_RX_PIN		GPIO_Pin_10
		#define USART1_RX_RCC_PORT	RCC_APB2Periph_GPIOA
		#define USART1_RX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define USART1_RX_MODE		GPIO_Mode_IN_FLOATING
		#define USART1_RX_SPEED		GPIO_Speed_50MHz

		#define USART1_TX_PORT		GPIOA
		#define USART1_TX_PIN		GPIO_Pin_9
		#define USART1_TX_RCC_PORT	RCC_APB2Periph_GPIOA
		#define USART1_TX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define USART1_TX_MODE		GPIO_Mode_AF_PP
		#define USART1_TX_SPEED		GPIO_Speed_50MHz
	#endif /* USE_DEFAULT_USART1_IO */

	/* Extern global variable */
	extern USART_InitTypeDef USART1_InitStructure;
	extern UART_Struct pUSART1;
	
	#ifdef USE_CLOCK_USART1 
	extern USART_ClockInitTypeDef USART1_ClockInitStructure;
	#endif /* USE_CLOCK_USART1 */

	#ifdef USE_TX_INTERRUPT_USART1
		#define USART1_TX_INTERRUPT_PRE_PRIORITY	1
		#define USART1_TX_INTERRUPT_SUB_PRIORITY	1
	#endif /* USE_TX_INTERRUPT_USART1 */
		
	#ifdef USE_RX_INTERRUPT_USART1
		#define USART1_RX_INTERRUPT_PRE_PRIORITY	1
		#define USART1_RX_INTERRUPT_SUB_PRIORITY	2
	#endif /* USE_TX_INTERRUPT_USART1 */
	
	#ifdef USE_DMA_USART1
        /* Queue to send and recieve data xQueue_TX_USART1_Buffer, xQueue_RX_USART1_Buffer*/
        #define DMA_USART1_TX_CHANNEL		DMA1_Channel4
    	#define DMA_USART1_TX_COMPLETE_FLAG	DMA1_FLAG_TC4
        #define DMA_USART1_RX_CHANNEL		DMA1_Channel5
    	#define DMA_USART1_RX_COMPLETE_FLAG	DMA1_FLAG_TC5
	extern DMA_InitTypeDef DMA_USART1_TX_InitStructure;
	extern DMA_InitTypeDef DMA_USART1_RX_InitStructure;
	#endif /* USE_DMA_USART1 */
#endif /* USE_USART1 */
#endif/* USART1_Function_Profile */
/************************************************************************************************************/