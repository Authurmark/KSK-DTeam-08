/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Hardware_Function_Profile.c
* Author             : OS Team
* Date First Issued  : 13/11/2013
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 13/11/2013: V0.1
* 23/09/2014: V1.0
* 21/12/2017: V1.1: Renew
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Hardware_Function_Profile__C
#define _Hardware_Function_Profile__C
#include "Hardware_Function_Profile.h"

/* Define USART1 Function */
#ifdef USE_USART1
	/* Global struct manage USART1 */
	UART_Struct pUSART1;
	/* Global Variable USART1 Buffer */
	uint8_t USART1_RXBuff[DEFAULT_USART1_NUMBER_BYTES_RX_BUFFER] = {0};
	#ifdef USE_TX_RINGBUF
	uint8_t USART1_TXBuff[DEFAULT_USART1_NUMBER_BYTES_TX_BUFFER] = {0};
	#endif
	/* Define RINGBUF for USART1 */
	RINGBUF USART1_RXRingBuff;
	#ifdef USE_TX_RINGBUF
	RINGBUF USART1_TXRingBuff;
	#endif
	/* Structure Init*/
	/* Config parameter to USART1*/
	USART_InitTypeDef USART1_InitStructure =
	{
		.USART_BaudRate				=	115200,							//toc do gioi han o 512kb/s ~ 64bytes/ms
		.USART_WordLength			=	USART_WordLength_8b,			//USART_WordLength_8b | USART_WordLength_9b
		.USART_StopBits				=	USART_StopBits_1, 				//USART_StopBits_1 | USART_StopBits_0_5 | USART_StopBits_2 | USART_StopBits_1_5
		.USART_Parity				=	USART_Parity_No, 				//USART_Parity_No | USART_Parity_Even | USART_Parity_Odd
		.USART_Mode					=	USART_Mode_Rx | USART_Mode_Tx, 	//USART_Mode_Rx | USART_Mode_Tx
		.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None, //USART_HardwareFlowControl_None | USART_HardwareFlowControl_RTS | USART_HardwareFlowControl_CTS | USART_HardwareFlowControl_RTS_CTS
	};
	
	#ifdef USE_CLOCK_USART1 /* Use clock to sync */
	USART_ClockInitTypeDef USART1_ClockInitStructure;
	#endif /* USE_CLOCK_USART1 */

	#ifdef USE_DMA_USART1 /* Use DMA */
	/* DMA_USART1_TX Init*/
	DMA_InitTypeDef DMA_USART1_TX_InitStructure =
	{
		.DMA_PeripheralBaseAddr =	USART1_BASE + 04,			//USART1 DR Address
        .DMA_MemoryBaseAddr		=	Null,						//Buffer Data Address, will change when send data, put the pointer to this parameter
        .DMA_DIR 				= 	DMA_DIR_PeripheralDST,		//DMA_DIR_PeripheralDST | DMA_DIR_PeripheralSRC
        .DMA_BufferSize			= 	1,							//Buffer Size Transmit Data
        .DMA_PeripheralInc		= 	DMA_PeripheralInc_Disable,	//DMA_PeripheralInc_Enable | DMA_PeripheralInc_Disable
        .DMA_MemoryInc			= 	DMA_MemoryInc_Enable,		//DMA_MemoryInc_Enable | DMA_MemoryInc_Disable
        .DMA_PeripheralDataSize	= 	DMA_PeripheralDataSize_Byte,//DMA_PeripheralDataSize_Byte | DMA_PeripheralDataSize_HalfWord | DMA_PeripheralDataSize_Word
        .DMA_MemoryDataSize 	= 	DMA_MemoryDataSize_Byte,	//DMA_MemoryDataSize_Byte | DMA_MemoryDataSize_HalfWord | DMA_MemoryDataSize_Word
        .DMA_Mode 				= 	DMA_Mode_Normal,			//DMA_Mode_Circular | DMA_Mode_Normal
        .DMA_Priority 			= 	DMA_Priority_VeryHigh,		//DMA_Priority_VeryHigh | DMA_Priority_High | DMA_Priority_Medium | DMA_Priority_Low
        .DMA_M2M 				= 	DMA_M2M_Disable,			//DMA_M2M_Enable | DMA_M2M_Disable
	};
	/* DMA_USART1_RX Init*/
	DMA_InitTypeDef DMA_USART1_RX_InitStructure =
	{
		.DMA_PeripheralBaseAddr	=	USART1_BASE + 04,			//USART1 DR Address
        .DMA_MemoryBaseAddr		=	(uint32_t)USART1_RXBuff,	//Buffer Data Address
        .DMA_DIR				= 	DMA_DIR_PeripheralSRC,		//DMA_DIR_PeripheralDST | DMA_DIR_PeripheralSRC
        .DMA_BufferSize 		= 	DEFAULT_USART1_NUMBER_BYTES_RX_BUFFER,//Buffer Size Transmit Data
        .DMA_PeripheralInc 		= 	DMA_PeripheralInc_Disable,	//DMA_PeripheralInc_Enable | DMA_PeripheralInc_Disable
        .DMA_MemoryInc 			= 	DMA_MemoryInc_Enable,		//DMA_MemoryInc_Enable | DMA_MemoryInc_Disable
        .DMA_PeripheralDataSize	= 	DMA_PeripheralDataSize_Byte,//DMA_PeripheralDataSize_Byte | DMA_PeripheralDataSize_HalfWord | DMA_PeripheralDataSize_Word
        .DMA_MemoryDataSize		= 	DMA_MemoryDataSize_Byte,	//DMA_MemoryDataSize_Byte | DMA_MemoryDataSize_HalfWord | DMA_MemoryDataSize_Word
        .DMA_Mode 				= 	DMA_Mode_Circular,			//DMA_Mode_Circular | DMA_Mode_Normal
        .DMA_Priority 			= 	DMA_Priority_VeryHigh,		//DMA_Priority_VeryHigh | DMA_Priority_High | DMA_Priority_Medium | DMA_Priority_Low
        .DMA_M2M 				= 	DMA_M2M_Disable,			//DMA_M2M_Enable | DMA_M2M_Disable
	};
	#endif /* USE_DMA_USART1 */
#endif /* USE_USART1 */
	
/* Define USART2 Function */
#ifdef USE_USART2
	/* Global struct manage USART2 */
	UART_Struct pUSART2;
	/* Global Variable USART2 Buffer */
	uint8_t USART2_RXBuff[DEFAULT_USART2_NUMBER_BYTES_RX_BUFFER] = {0};
	#ifdef USE_TX_RINGBUF
	uint8_t USART2_TXBuff[DEFAULT_USART2_NUMBER_BYTES_TX_BUFFER] = {0};
	#endif
	/* Define RINGBUF for USART2 */
	RINGBUF USART2_RXRingBuff;
	#ifdef USE_TX_RINGBUF
	RINGBUF USART2_TXRingBuff;
	#endif
	/* Structure Init*/
	/* Config parameter to USART2*/
	USART_InitTypeDef USART2_InitStructure =
	{
		.USART_BaudRate				=	115200,							//toc do gioi han o 512kb/s ~ 64bytes/ms
		.USART_WordLength			=	USART_WordLength_8b,			//USART_WordLength_8b | USART_WordLength_9b
		.USART_StopBits				=	USART_StopBits_1, 				//USART_StopBits_1 | USART_StopBits_0_5 | USART_StopBits_2 | USART_StopBits_1_5
		.USART_Parity				=	USART_Parity_No, 				//USART_Parity_No | USART_Parity_Even | USART_Parity_Odd
		.USART_Mode					=	USART_Mode_Rx | USART_Mode_Tx, 	//USART_Mode_Rx | USART_Mode_Tx
		.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None, //USART_HardwareFlowControl_None | USART_HardwareFlowControl_RTS | USART_HardwareFlowControl_CTS | USART_HardwareFlowControl_RTS_CTS
	};
#endif /* USE_USART2 */

/* Define USART3 Function */
#ifdef USE_USART3
	/* Global struct manage USART3 */
	UART_Struct pUSART3;
	/* Global Variable USART3 Buffer */
	uint8_t USART3_RXBuff[DEFAULT_USART3_NUMBER_BYTES_RX_BUFFER] = {0};
	#ifdef USE_TX_RINGBUF
	uint8_t USART3_TXBuff[DEFAULT_USART3_NUMBER_BYTES_TX_BUFFER] = {0};
	#endif
	/* Define RINGBUF for USART3 */
	RINGBUF USART3_RXRingBuff;
	#ifdef USE_TX_RINGBUF
	RINGBUF USART3_TXRingBuff;
	#endif
	/* Structure Init*/
	/* Config parameter to USART3*/
	USART_InitTypeDef USART3_InitStructure =
	{
		.USART_BaudRate				=	115200,							//toc do gioi han o 512kb/s ~ 64bytes/ms
		.USART_WordLength			=	USART_WordLength_8b,			//USART_WordLength_8b | USART_WordLength_9b
		.USART_StopBits				=	USART_StopBits_1, 				//USART_StopBits_1 | USART_StopBits_0_5 | USART_StopBits_2 | USART_StopBits_1_5
		.USART_Parity				=	USART_Parity_No, 				//USART_Parity_No | USART_Parity_Even | USART_Parity_Odd
		.USART_Mode					=	USART_Mode_Rx | USART_Mode_Tx, 	//USART_Mode_Rx | USART_Mode_Tx
		.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None, //USART_HardwareFlowControl_None | USART_HardwareFlowControl_RTS | USART_HardwareFlowControl_CTS | USART_HardwareFlowControl_RTS_CTS
	};
#endif /* USE_USART3 */

/* Define UART4 Function */
#ifdef USE_UART4
	/* Global struct manage UART4 */
	UART_Struct pUART4;
	/* Global Variable UART4 Buffer */
	uint8_t UART4_RXBuff[DEFAULT_UART4_NUMBER_BYTES_RX_BUFFER] = {0};
	#ifdef USE_TX_RINGBUF
	uint8_t UART4_TXBuff[DEFAULT_UART4_NUMBER_BYTES_TX_BUFFER] = {0};
	#endif
	/* Define RINGBUF for UART4 */
	RINGBUF UART4_RXRingBuff;
	#ifdef USE_TX_RINGBUF
	RINGBUF UART4_TXRingBuff;
	#endif
	/* Structure Init*/
	/* Config parameter to UART4*/
	USART_InitTypeDef UART4_InitStructure =
	{
		.USART_BaudRate				=	115200,							//toc do gioi han o 512kb/s ~ 64bytes/ms
		.USART_WordLength			=	USART_WordLength_8b,			//USART_WordLength_8b | USART_WordLength_9b
		.USART_StopBits				=	USART_StopBits_1, 				//USART_StopBits_1 | USART_StopBits_0_5 | USART_StopBits_2 | USART_StopBits_1_5
		.USART_Parity				=	USART_Parity_No, 				//USART_Parity_No | USART_Parity_Even | USART_Parity_Odd
		.USART_Mode					=	USART_Mode_Rx | USART_Mode_Tx, 	//USART_Mode_Rx | USART_Mode_Tx
		.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None, //USART_HardwareFlowControl_None | USART_HardwareFlowControl_RTS | USART_HardwareFlowControl_CTS | USART_HardwareFlowControl_RTS_CTS
	};
	
	#ifdef USE_CLOCK_UART4 /* Use clock to sync */
	USART_ClockInitTypeDef UART4_ClockInitStructure;
	#endif /* USE_CLOCK_UART4 */
#endif /* USE_UART4 */
	
#ifdef USE_SPI1
	/* Global struct manage SPI1 */
	SPI_Struct pSPI1;
	/* Global Variable SPI1 Buffer */
	uint8_t SPI1_RXBuff[DEFAULT_SPI1_NUMBER_BYTES_RX_BUFFER];
	uint8_t SPI1_TXBuff[DEFAULT_SPI1_NUMBER_BYTES_TX_BUFFER];
	/* Define RINGBUF for SPI1 */
	RINGBUF SPI1_RXRingBuff;
	RINGBUF SPI1_TXRingBuff;
	SPI_InitTypeDef SPI1_InitStructure =
	{
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,	//SPI_Direction_2Lines_FullDuplex | SPI_Direction_2Lines_RxOnly | SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx
		.SPI_Mode = SPI_Mode_Master,						//SPI_Mode_Master | SPI_Mode_Slave
		.SPI_DataSize = SPI_DataSize_8b,					//SPI_DataSize_16b | SPI_DataSize_8b
		.SPI_CPOL = SPI_CPOL_High,							//SPI_CPOL_Low | SPI_CPOL_High
	  	.SPI_CPHA = SPI_CPHA_2Edge,							//SPI_CPHA_1Edge | SPI_CPHA_2Edge
	  	.SPI_NSS = SPI_NSS_Soft,							//SPI_NSS_Soft | SPI_NSS_Hard
	  	.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16,	//SPI_BaudRatePrescaler_2 | SPI_BaudRatePrescaler_4 ...256
	  	.SPI_FirstBit = SPI_FirstBit_MSB,
	  	.SPI_CRCPolynomial = 7,
	};
#endif /* USE_SPI1 */
#ifdef USE_SPI2
	/* Global struct manage SPI2 */
	SPI_Struct pSPI2;
	/* Global Variable SPI2 Buffer */
	uint8_t SPI2_RXBuff[DEFAULT_SPI2_NUMBER_BYTES_RX_BUFFER];
	uint8_t SPI2_TXBuff[DEFAULT_SPI2_NUMBER_BYTES_TX_BUFFER];
	/* Define RINGBUF for USART1 */
	RINGBUF SPI2_RXRingBuff;
	RINGBUF SPI2_TXRingBuff;
	SPI_InitTypeDef SPI2_InitStructure =
	{
		.SPI_Direction = SPI_Direction_2Lines_FullDuplex,	//SPI_Direction_2Lines_FullDuplex | SPI_Direction_2Lines_RxOnly | SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx
		.SPI_Mode = SPI_Mode_Master,						//SPI_Mode_Master | SPI_Mode_Slave
		.SPI_DataSize = SPI_DataSize_8b,					//SPI_DataSize_16b | SPI_DataSize_8b
		.SPI_CPOL = SPI_CPOL_Low,							//SPI_CPOL_Low | SPI_CPOL_High
	  	.SPI_CPHA = SPI_CPHA_1Edge,							//SPI_CPHA_1Edge | SPI_CPHA_2Edge
	  	.SPI_NSS = SPI_NSS_Soft,							//SPI_NSS_Soft | SPI_NSS_Hard
	  	.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64,	//SPI_BaudRatePrescaler_2 | SPI_BaudRatePrescaler_4 ...256
	  	.SPI_FirstBit = SPI_FirstBit_MSB,
	  	.SPI_CRCPolynomial = 7,
	};
#endif /* USE_SPI2 */
/************************************************************************************************************/
/* Project define */
/************************************************************************************************************/
/* Define for SI4432_Project */
#ifdef SI4432_Project
	/* Define global variable for SI4432 module */
#endif /*SI4432_Project*/
#endif /* _Hardware_Function_Profile__C */
