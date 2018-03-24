/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UART4_Function_Profile.h
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
#ifndef	_UART4_Function_Profile__H
#define _UART4_Function_Profile__H
/* Include global header file */
#include "Common.h"
/* Define UART4 Function */ 
/* Uncomment to use UART4, comment to disable*/
#define USE_UART4  
/* Uncomment to use default, comment to use alternative*/
	#define USE_DEFAULT_UART4_IO
	/* Uncomment to use USE_CLOCK_UART4, comment to disable*/
	//#define USE_CLOCK_UART4
	/* Uncomment to use interrupt , comment to disable*/
	#define USE_TX_INTERRUPT_UART4
	#define USE_RX_INTERRUPT_UART4
	/* Uncomment to use DMA transmit, comment to disable*/
	//#define USE_DMA_UART4  
	/* Define size of RX, TX buffer */
	#define DEFAULT_UART4_NUMBER_BYTES_RX_BUFFER 400
	#define DEFAULT_UART4_NUMBER_BYTES_TX_BUFFER 400
	/* Uncomment to use TX_RINGBUF, comment to disable */
	#define USE_TX_RINGBUF
/* Define UART4 Function */	
#ifdef USE_UART4
	#define UART4_RCC			RCC_APB1Periph_UART4
	#define UART4_CLK_CMD		RCC_APB1PeriphClockCmd

	#ifdef	USE_DEFAULT_UART4_IO
		#define UART4_RX_PORT		GPIOC
		#define UART4_RX_PIN		GPIO_Pin_11
		#define UART4_RX_RCC_PORT	RCC_APB2Periph_GPIOC
		#define UART4_RX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define UART4_RX_MODE		GPIO_Mode_IN_FLOATING
		#define UART4_RX_SPEED		GPIO_Speed_50MHz

		#define UART4_TX_PORT		GPIOC
		#define UART4_TX_PIN		GPIO_Pin_10
		#define UART4_TX_RCC_PORT	RCC_APB2Periph_GPIOC
		#define UART4_TX_CLK_CMD	RCC_APB2PeriphClockCmd
		#define UART4_TX_MODE		GPIO_Mode_AF_PP
		#define UART4_TX_SPEED		GPIO_Speed_50MHz
	#endif /* USE_DEFAULT_UART4_IO */

	/* Extern global variable */
	extern USART_InitTypeDef UART4_InitStructure;
	extern UART_Struct pUART4;
	
	#ifdef USE_CLOCK_UART4 
	extern USART_ClockInitTypeDef UART4_ClockInitStructure;
	#endif /* USE_CLOCK_UART4 */

	#ifdef USE_TX_INTERRUPT_UART4
		#define UART4_TX_INTERRUPT_PRE_PRIORITY	2
		#define UART4_TX_INTERRUPT_SUB_PRIORITY	1
	#endif /* USE_TX_INTERRUPT_UART4 */
		
	#ifdef USE_RX_INTERRUPT_UART4
		#define UART4_RX_INTERRUPT_PRE_PRIORITY	2
		#define UART4_RX_INTERRUPT_SUB_PRIORITY	2
	#endif /* USE_TX_INTERRUPT_UART4 */
	
	#ifdef USE_DMA_UART4
        /* Queue to send and recieve data xQueue_TX_UART4_Buffer, xQueue_RX_UART4_Buffer*/
        #define DMA_UART4_TX_CHANNEL		DMA1_Channel4
    	#define DMA_UART4_TX_COMPLETE_FLAG	DMA1_FLAG_TC4
        #define DMA_UART4_RX_CHANNEL		DMA1_Channel5
    	#define DMA_UART4_RX_COMPLETE_FLAG	DMA1_FLAG_TC5
	extern DMA_InitTypeDef DMA_UART4_TX_InitStructure;
	extern DMA_InitTypeDef DMA_UART4_RX_InitStructure;
	#endif /* USE_DMA_UART4 */
#endif /* USE_UART4 */
#endif/* UART4_Function_Profile */
/************************************************************************************************************/