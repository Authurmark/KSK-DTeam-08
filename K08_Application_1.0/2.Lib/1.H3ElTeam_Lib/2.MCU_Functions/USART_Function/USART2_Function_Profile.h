/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART2_Function_Profile.h
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
#ifndef	_USART2_Function_Profile__H
#define _USART2_Function_Profile__H
/* Include global header file */
#include "Common.h"
/* Define USART2 Function */ 
/* Uncomment to use USART2, comment to disable*/
#define USE_USART2  
/* Uncomment to use default, comment to use alternative*/
	#define USE_DEFAULT_USART2_IO
	/* Uncomment to use USE_CLOCK_USART2, comment to disable*/
	//#define USE_CLOCK_USART2
	/* Uncomment to use interrupt , comment to disable*/
	#define USE_TX_INTERRUPT_USART2
	#define USE_RX_INTERRUPT_USART2
	/* Uncomment to use DMA transmit, comment to disable*/
	//#define USE_DMA_USART2  
	/* Define size of RX, TX buffer */
	#define DEFAULT_USART2_NUMBER_BYTES_RX_BUFFER 400
	#define DEFAULT_USART2_NUMBER_BYTES_TX_BUFFER 800
	/* Uncomment to use TX_RINGBUF, comment to disable */
	#define USE_TX_RINGBUF
/* Define USART2 Function */	
#ifdef USE_USART2
	#define USART2_RCC			RCC_APB1Periph_USART2
	#define USART2_CLK_CMD		RCC_APB1PeriphClockCmd

	#ifdef	USE_DEFAULT_USART2_IO
		#define USART2_RX_PORT		GPIOA
		#define USART2_RX_PIN		GPIO_Pin_3
		#define USART2_RX_RCC_PORT	RCC_APB2Periph_GPIOA
		#define USART2_RX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define USART2_RX_MODE		GPIO_Mode_IN_FLOATING
		#define USART2_RX_SPEED		GPIO_Speed_50MHz

		#define USART2_TX_PORT		GPIOA
		#define USART2_TX_PIN		GPIO_Pin_2
		#define USART2_TX_RCC_PORT	RCC_APB2Periph_GPIOA
		#define USART2_TX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define USART2_TX_MODE		GPIO_Mode_AF_PP
		#define USART2_TX_SPEED		GPIO_Speed_50MHz
	#endif /* USE_DEFAULT_USART2_IO */

	/* Extern global variable */
	extern USART_InitTypeDef USART2_InitStructure;
	extern UART_Struct pUSART2;
	
	#ifdef USE_CLOCK_USART2 
	extern USART_ClockInitTypeDef USART2_ClockInitStructure;
	#endif /* USE_CLOCK_USART2 */

	#ifdef USE_TX_INTERRUPT_USART2
		#define USART2_TX_INTERRUPT_PRE_PRIORITY	2
		#define USART2_TX_INTERRUPT_SUB_PRIORITY	1
	#endif /* USE_TX_INTERRUPT_USART2 */
		
	#ifdef USE_RX_INTERRUPT_USART2
		#define USART2_RX_INTERRUPT_PRE_PRIORITY	2
		#define USART2_RX_INTERRUPT_SUB_PRIORITY	2
	#endif /* USE_TX_INTERRUPT_USART2 */
	
	#ifdef USE_DMA_USART2
        /* Queue to send and recieve data xQueue_TX_USART2_Buffer, xQueue_RX_USART2_Buffer*/
        #define DMA_USART2_TX_CHANNEL		DMA1_Channel4
    	#define DMA_USART2_TX_COMPLETE_FLAG	DMA1_FLAG_TC4
        #define DMA_USART2_RX_CHANNEL		DMA1_Channel5
    	#define DMA_USART2_RX_COMPLETE_FLAG	DMA1_FLAG_TC5
	extern DMA_InitTypeDef DMA_USART2_TX_InitStructure;
	extern DMA_InitTypeDef DMA_USART2_RX_InitStructure;
	#endif /* USE_DMA_USART2 */
#endif /* USE_USART2 */
#endif/* USART2_Function_Profile */
/************************************************************************************************************/