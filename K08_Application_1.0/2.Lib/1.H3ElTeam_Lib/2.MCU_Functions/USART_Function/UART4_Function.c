/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UART4_Function.c
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
/* Include global header file */
#include "UART4_Function.h"
#ifndef	_UART4_Function__C
#define _UART4_Function__C
#ifdef USE_UART4
/* Extern RINGBUF for UART4 */
extern RINGBUF UART4_RXRingBuff;
extern RINGBUF UART4_TXRingBuff;
extern uint8_t UART4_RXBuff[DEFAULT_UART4_NUMBER_BYTES_RX_BUFFER];
extern uint8_t UART4_TXBuff[DEFAULT_UART4_NUMBER_BYTES_TX_BUFFER];
/* UART4 Init function */
void UART4_Init( void )
{
	#ifdef USE_CMCIS_DRIVER
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	#ifdef USE_UART4
		#ifdef USE_DEFAULT_UART4_IO
		#warning: UART4 function use GPIOA_GPIO_Pin_9 and GPIOA_GPIO_Pin_10
        /* Enable UART4 clock */
		UART4_CLK_CMD(UART4_RCC,ENABLE);
		/* Enable clock IO */
        UART4_RX_CLK_CMD(UART4_RX_RCC_PORT,ENABLE);
		UART4_TX_CLK_CMD(UART4_TX_RCC_PORT,ENABLE);
        
        /* Configure UART4 Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = UART4_RX_PIN;
		GPIO_InitStructure.GPIO_Mode = UART4_RX_MODE;
		GPIO_InitStructure.GPIO_Speed = UART4_RX_SPEED;
        GPIO_Init( UART4_RX_PORT, &GPIO_InitStructure );
        
        /* Configure UART4 Tx as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = UART4_TX_PIN;
        GPIO_InitStructure.GPIO_Speed = UART4_TX_SPEED;
        GPIO_InitStructure.GPIO_Mode = UART4_TX_MODE;
        GPIO_Init( UART4_TX_PORT, &GPIO_InitStructure );
		#endif /* USE_DEFAULT_UART4_IO */

		/* DeInit UART4 */
//        USART_DeInit( UART4 );
        /* Init UART4 */
        USART_Init( UART4, &UART4_InitStructure );
        /* USART enable */
        USART_Cmd( UART4, ENABLE );	
			
        #ifdef USE_TX_INTERRUPT_UART4
		/* Enable IT TXE, use to get data bytes */
        USART_ITConfig( UART4, USART_IT_TXE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	UART4_TX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		UART4_TX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif
		#ifdef USE_RX_INTERRUPT_UART4
		/* Enable IT RXNE, use to get data bytes */
        USART_ITConfig( UART4, USART_IT_RXNE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	UART4_RX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		UART4_RX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif

		#ifdef USE_DMA_UART4
        /* Config DMA */
        /* DMA clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        
        /* UART4_TX_DMA_Channel (triggered by UART4 TX event) Config */
        DMA_DeInit(DMA_UART4_TX_CHANNEL);
        DMA_Init(DMA_UART4_TX_CHANNEL, &DMA_UART4_TX_InitStructure);
        DMA_Cmd(DMA_UART4_TX_CHANNEL, DISABLE);
        /* UART4_RX_DMA_Channel (triggered by UART4 RX event) Config */
        DMA_DeInit(DMA_UART4_RX_CHANNEL);
        DMA_Init(DMA_UART4_RX_CHANNEL, &DMA_UART4_RX_InitStructure);
        DMA_Cmd(DMA_UART4_RX_CHANNEL, DISABLE);
        
        /* Enable UART4 DMA TX request */
        USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
        /* Enable UART4 DMA RX request */
        USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
        
        /* Enable DMA_UART4_TX_CHANNEL */
        DMA_Cmd(DMA_UART4_TX_CHANNEL, ENABLE);
        /* Enable DMA_UART4_RX_CHANNEL */
        DMA_Cmd(DMA_UART4_RX_CHANNEL, ENABLE);
		
		/* While DMA transfer complete */
		while(DMA_GetFlagStatus(DMA_UART4_TX_COMPLETE_FLAG)==RESET);
		#endif /* USE_DMA_UART4 */
	#endif /* USE_CMCIS_DRIVER */
	
	#ifdef USE_ONLY_REGISTER
	#endif /* USE_ONLY_REGISTER */

	/* Init ringbuf buffer */
	RINGBUF_Init(&UART4_TXRingBuff,UART4_TXBuff,DEFAULT_UART4_NUMBER_BYTES_TX_BUFFER);
	RINGBUF_Init(&UART4_RXRingBuff,UART4_RXBuff,DEFAULT_UART4_NUMBER_BYTES_RX_BUFFER);
	#endif /* USE_UART4 */
}

/* Function close and deinit UART4 */
void UART4_Close( void )
{
	#ifdef USE_CMCIS_DRIVER
	USART_DeInit(UART4);
	#endif
}

/* Function start Send byte UART4 */
void UART4_StartSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_UART4
		USART_ITConfig( UART4, USART_IT_TXE, ENABLE );
		#endif /* USE_TX_INTERRUPT_UART4 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Send Byte UART4 */
void UART4_StopSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_UART4
		USART_ITConfig( UART4, USART_IT_TXE, DISABLE );
		#endif /* USE_TX_INTERRUPT_UART4 */
	#endif /* USE_CMCIS_DRIVER */
}
/* Function start Receive byte UART4 */
void UART4_StartReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_UART4
		USART_ITConfig( UART4, USART_IT_RXNE, ENABLE );
		#endif /* USE_TX_INTERRUPT_UART4 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Receive Byte UART4 */
void UART4_StopReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_UART4
		USART_ITConfig( UART4, USART_IT_RXNE, DISABLE );
		#endif /* USE_TX_INTERRUPT_UART4 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Send 1 Byte to TX RingBuf */
void UART4_Send_Byte_TX_RingBuf( unsigned char cChar )
{
	RINGBUF_Put(&UART4_TXRingBuff,cChar);
}

/* Get 1 Byte From RX RingBuf */
enumbool UART4_Get_Byte_RX_RingBuf( char *pChar )
{
	if(RINGBUF_Get(&UART4_RXRingBuff,pChar)==0)/* Success */
		return eTRUE;
	else 
		return eFALSE;
}

/* Interrupt handler function */
void UART4_IRQHandler( void )
{
#ifdef USE_UART4
	#ifdef USE_FREERTOS
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    #endif /* USE_FREERTOS */
	static unsigned char cChar = 0xFF ;
	#ifdef USE_CMCIS_DRIVER
	/* RX Interrupt handler */
    if( USART_GetITStatus( UART4, USART_IT_RXNE ) == SET )
    {
        cChar = USART_ReceiveData( UART4 );
		RINGBUF_Put(&UART4_RXRingBuff,cChar);
    }	
	/* TX Interrupt handler */
    else if( USART_GetITStatus( UART4, USART_IT_TXE ) == SET )
    {
        /* Read from ringbuf, send if have any data */
		if(RINGBUF_Get(&UART4_TXRingBuff,&cChar) == 0)
        {
			USART_SendData(UART4, cChar);
        }
        else /* Disable TXE when finish transfer */
        {
            UART4_StopSend();	
        }		
    }
	else cChar = USART_ReceiveData( UART4 );
	#endif
    #ifdef USE_FREERTOS
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	#endif /* USE_FREERTOS */
#endif
}

#endif /* UART4 */
#endif /* _UART4_Function__C */
/************************************************************************************************************/