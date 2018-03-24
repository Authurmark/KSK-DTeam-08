/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART1_Function.c
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
#include "USART1_Function.h"
#ifndef	_USART1_Function__C
#define _USART1_Function__C
#ifdef USE_USART1
/* Extern RINGBUF for USART1 */
extern RINGBUF USART1_RXRingBuff;
extern RINGBUF USART1_TXRingBuff;
extern uint8_t USART1_RXBuff[DEFAULT_USART1_NUMBER_BYTES_RX_BUFFER];
extern uint8_t USART1_TXBuff[DEFAULT_USART1_NUMBER_BYTES_TX_BUFFER];
/* USART1 Init function */
void USART1_Init( void )
{
	#ifdef USE_CMCIS_DRIVER
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	#ifdef USE_USART1
		#ifdef USE_DEFAULT_USART1_IO
		#warning: USART1 function use GPIOA_GPIO_Pin_9 and GPIOA_GPIO_Pin_10
        /* Enable USART1 clock */
		USART1_CLK_CMD(USART1_RCC,ENABLE);
		/* Enable clock IO */
        USART1_RX_CLK_CMD(USART1_RX_RCC_PORT,ENABLE);
		USART1_TX_CLK_CMD(USART1_TX_RCC_PORT,ENABLE);
        
        /* Configure USART1 Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
		GPIO_InitStructure.GPIO_Mode = USART1_RX_MODE;
		GPIO_InitStructure.GPIO_Speed = USART1_RX_SPEED;
        GPIO_Init( USART1_RX_PORT, &GPIO_InitStructure );
        
        /* Configure USART1 Tx as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
        GPIO_InitStructure.GPIO_Speed = USART1_TX_SPEED;
        GPIO_InitStructure.GPIO_Mode = USART1_TX_MODE;
        GPIO_Init( USART1_TX_PORT, &GPIO_InitStructure );
		#endif /* USE_DEFAULT_USART1_IO */

		/* DeInit USART1 */
        USART_DeInit( USART1 );
        /* Init USART1 */
        USART_Init( USART1, &USART1_InitStructure );
        /* USART enable */
        USART_Cmd( USART1, ENABLE );	
			
        #ifdef USE_TX_INTERRUPT_USART1
		/* Enable IT TXE, use to get data bytes */
        USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	USART1_TX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		USART1_TX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif
		#ifdef USE_RX_INTERRUPT_USART1
		/* Enable IT RXNE, use to get data bytes */
        USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	USART1_RX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		USART1_RX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif

		#ifdef USE_DMA_USART1
        /* Config DMA */
        /* DMA clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        
        /* USART1_TX_DMA_Channel (triggered by USART1 TX event) Config */
        DMA_DeInit(DMA_USART1_TX_CHANNEL);
        DMA_Init(DMA_USART1_TX_CHANNEL, &DMA_USART1_TX_InitStructure);
        DMA_Cmd(DMA_USART1_TX_CHANNEL, DISABLE);
        /* USART1_RX_DMA_Channel (triggered by USART1 RX event) Config */
        DMA_DeInit(DMA_USART1_RX_CHANNEL);
        DMA_Init(DMA_USART1_RX_CHANNEL, &DMA_USART1_RX_InitStructure);
        DMA_Cmd(DMA_USART1_RX_CHANNEL, DISABLE);
        
        /* Enable USART1 DMA TX request */
        USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
        /* Enable USART1 DMA RX request */
        USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
        
        /* Enable DMA_USART1_TX_CHANNEL */
        DMA_Cmd(DMA_USART1_TX_CHANNEL, ENABLE);
        /* Enable DMA_USART1_RX_CHANNEL */
        DMA_Cmd(DMA_USART1_RX_CHANNEL, ENABLE);
		
		/* While DMA transfer complete */
		while(DMA_GetFlagStatus(DMA_USART1_TX_COMPLETE_FLAG)==RESET);
		#endif /* USE_DMA_USART1 */
	#endif /* USE_CMCIS_DRIVER */
	
	#ifdef USE_ONLY_REGISTER
	#endif /* USE_ONLY_REGISTER */

	/* Init ringbuf buffer */
	RINGBUF_Init(&USART1_TXRingBuff,USART1_TXBuff,DEFAULT_USART1_NUMBER_BYTES_TX_BUFFER);
	RINGBUF_Init(&USART1_RXRingBuff,USART1_RXBuff,DEFAULT_USART1_NUMBER_BYTES_RX_BUFFER);
	#endif /* USE_USART1 */
}

/* Function close and deinit USART1 */
void USART1_Close( void )
{
	#ifdef USE_CMCIS_DRIVER
	USART_DeInit(USART1);
	#endif
}

/* Function start Send byte USART1 */
void USART1_StartSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_USART1
		USART_ITConfig( USART1, USART_IT_TXE, ENABLE );
		#endif /* USE_TX_INTERRUPT_USART1 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Send Byte USART1 */
void USART1_StopSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_USART1
		USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
		#endif /* USE_TX_INTERRUPT_USART1 */
	#endif /* USE_CMCIS_DRIVER */
}
/* Function start Receive byte USART1 */
void USART1_StartReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_USART1
		USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
		#endif /* USE_TX_INTERRUPT_USART1 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Receive Byte USART1 */
void USART1_StopReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_USART1
		USART_ITConfig( USART1, USART_IT_RXNE, DISABLE );
		#endif /* USE_TX_INTERRUPT_USART1 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Send 1 Byte to TX RingBuf */
void USART1_Send_Byte_TX_RingBuf( unsigned char cChar )
{
	RINGBUF_Put(&USART1_TXRingBuff,cChar);
}

/* Get 1 Byte From RX RingBuf */
enumbool USART1_Get_Byte_RX_RingBuf( char *pChar  )
{
	if(RINGBUF_Get(&USART1_RXRingBuff,pChar)==0)/* Success */
		return eTRUE;
	else 
		return eFALSE;
}

/* Interrupt handler function */
void USART1_IRQHandler( void )
{
#ifdef USE_USART1
	#ifdef USE_FREERTOS
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    #endif /* USE_FREERTOS */
	static unsigned char cChar = 0xFF ;
	#ifdef USE_CMCIS_DRIVER
	/* RX Interrupt handler */
    if( USART_GetITStatus( USART1, USART_IT_RXNE ) == SET )
    {
        cChar = USART_ReceiveData( USART1 );
		RINGBUF_Put(&USART1_RXRingBuff,cChar);
    }	
	/* TX Interrupt handler */
    else if( USART_GetITStatus( USART1, USART_IT_TXE ) == SET )
    {
        /* Read from ringbuf, send if have any data */
		if(RINGBUF_Get(&USART1_TXRingBuff,&cChar) == 0)
        {
			USART_SendData(USART1, cChar);
        }
        else /* Disable TXE when finish transfer */
        {
            USART1_StopSend();	
        }		
    }
	else cChar = USART_ReceiveData( USART1 );
	#endif
    #ifdef USE_FREERTOS
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	#endif /* USE_FREERTOS */
#endif
}

#endif /* USART1 */
#endif /* _USART1_Function__C */
/************************************************************************************************************/