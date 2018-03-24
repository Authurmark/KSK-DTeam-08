/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART2_Function.c
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
/* Include global header file */
#include "USART2_Function.h"
#ifndef	_USART2_Function__C
#define _USART2_Function__C
#ifdef USE_USART2
/* Extern RINGBUF for USART2 */
extern RINGBUF USART2_RXRingBuff;
extern RINGBUF USART2_TXRingBuff;
extern uint8_t USART2_RXBuff[DEFAULT_USART2_NUMBER_BYTES_RX_BUFFER];
extern uint8_t USART2_TXBuff[DEFAULT_USART2_NUMBER_BYTES_TX_BUFFER];
/* USART2 Init function */
void USART2_Init( void )
{
	#ifdef USE_CMCIS_DRIVER
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	#ifdef USE_USART2
		#ifdef USE_DEFAULT_USART2_IO
		#warning: USART2 function use GPIOA_GPIO_Pin_2 and GPIOA_GPIO_Pin_3
        /* Enable USART2 clock */
		USART2_CLK_CMD(USART2_RCC,ENABLE);
		/* Enable clock IO */
        USART2_RX_CLK_CMD(USART2_RX_RCC_PORT,ENABLE);
		USART2_TX_CLK_CMD(USART2_TX_RCC_PORT,ENABLE);
        
        /* Configure USART2 Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
		GPIO_InitStructure.GPIO_Mode = USART2_RX_MODE;
		GPIO_InitStructure.GPIO_Speed = USART2_RX_SPEED;
        GPIO_Init( USART2_RX_PORT, &GPIO_InitStructure );
        
        /* Configure USART2 Tx as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
        GPIO_InitStructure.GPIO_Speed = USART2_TX_SPEED;
        GPIO_InitStructure.GPIO_Mode = USART2_TX_MODE;
        GPIO_Init( USART2_TX_PORT, &GPIO_InitStructure );
		#endif /* USE_DEFAULT_USART2_IO */

		/* DeInit USART2 */
//        USART_DeInit( USART2 );
        /* Init USART2 */
        USART_Init( USART2, &USART2_InitStructure );
        /* USART enable */
        USART_Cmd( USART2, ENABLE );	
			
        #ifdef USE_TX_INTERRUPT_USART2
		/* Enable IT TXE, use to get data bytes */
        USART_ITConfig( USART2, USART_IT_TXE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	USART2_TX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		USART2_TX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif
		#ifdef USE_RX_INTERRUPT_USART2
		/* Enable IT RXNE, use to get data bytes */
        USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	USART2_RX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		USART2_RX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif

		#ifdef USE_DMA_USART2
        /* Config DMA */
        /* DMA clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        
        /* USART2_TX_DMA_Channel (triggered by USART2 TX event) Config */
        DMA_DeInit(DMA_USART2_TX_CHANNEL);
        DMA_Init(DMA_USART2_TX_CHANNEL, &DMA_USART2_TX_InitStructure);
        DMA_Cmd(DMA_USART2_TX_CHANNEL, DISABLE);
        /* USART2_RX_DMA_Channel (triggered by USART2 RX event) Config */
        DMA_DeInit(DMA_USART2_RX_CHANNEL);
        DMA_Init(DMA_USART2_RX_CHANNEL, &DMA_USART2_RX_InitStructure);
        DMA_Cmd(DMA_USART2_RX_CHANNEL, DISABLE);
        
        /* Enable USART2 DMA TX request */
        USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
        /* Enable USART2 DMA RX request */
        USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
        
        /* Enable DMA_USART2_TX_CHANNEL */
        DMA_Cmd(DMA_USART2_TX_CHANNEL, ENABLE);
        /* Enable DMA_USART2_RX_CHANNEL */
        DMA_Cmd(DMA_USART2_RX_CHANNEL, ENABLE);
		
		/* While DMA transfer complete */
		while(DMA_GetFlagStatus(DMA_USART2_TX_COMPLETE_FLAG)==RESET);
		#endif /* USE_DMA_USART2 */
	#endif /* USE_CMCIS_DRIVER */
	
	#ifdef USE_ONLY_REGISTER
	#endif /* USE_ONLY_REGISTER */

	/* Init ringbuf buffer */
	RINGBUF_Init(&USART2_TXRingBuff,USART2_TXBuff,DEFAULT_USART2_NUMBER_BYTES_TX_BUFFER);
	RINGBUF_Init(&USART2_RXRingBuff,USART2_RXBuff,DEFAULT_USART2_NUMBER_BYTES_RX_BUFFER);
	#endif /* USE_USART2 */
}

/* Function close and deinit USART2 */
void USART2_Close( void )
{
	#ifdef USE_CMCIS_DRIVER
	USART_DeInit(USART2);
	#endif
}

/* Function start Send byte USART2 */
void USART2_StartSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_USART2
		USART_ITConfig( USART2, USART_IT_TXE, ENABLE );
		#endif /* USE_TX_INTERRUPT_USART2 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Send Byte USART2 */
void USART2_StopSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_USART2
		USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
		#endif /* USE_TX_INTERRUPT_USART2 */
	#endif /* USE_CMCIS_DRIVER */
}
/* Function start Receive byte USART2 */
void USART2_StartReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_USART2
		USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
		#endif /* USE_TX_INTERRUPT_USART2 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Receive Byte USART2 */
void USART2_StopReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_USART2
		USART_ITConfig( USART2, USART_IT_RXNE, DISABLE );
		#endif /* USE_TX_INTERRUPT_USART2 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Send 1 Byte to TX RingBuf */
void USART2_Send_Byte_TX_RingBuf( unsigned char cChar )
{
	RINGBUF_Put(&USART2_TXRingBuff,cChar);
}

/* Get 1 Byte From RX RingBuf */
enumbool USART2_Get_Byte_RX_RingBuf(  char *pChar  )
{
	if(RINGBUF_Get(&USART2_RXRingBuff,pChar)==0)/* Success */
		return eTRUE;
	else 
		return eFALSE;
}

/* Interrupt handler function */
void USART2_IRQHandler( void )
{
#ifdef USE_USART2
	#ifdef USE_FREERTOS
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    #endif /* USE_FREERTOS */
	static unsigned char cChar = 0xFF ;
	#ifdef USE_CMCIS_DRIVER
	/* RX Interrupt handler */
    if( USART_GetITStatus( USART2, USART_IT_RXNE ) == SET )
    {
        cChar = USART_ReceiveData( USART2 );
		RINGBUF_Put(&USART2_RXRingBuff,cChar);
    }	
	/* TX Interrupt handler */
    else if( USART_GetITStatus( USART2, USART_IT_TXE ) == SET )
    {
        /* Read from ringbuf, send if have any data */
		if(RINGBUF_Get(&USART2_TXRingBuff,&cChar) == 0)
        {
			USART_SendData(USART2, cChar);
        }
        else /* Disable TXE when finish transfer */
        {
            USART2_StopSend();	
        }		
    }
	else cChar = USART_ReceiveData( USART2 );
	#endif
    #ifdef USE_FREERTOS
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	#endif /* USE_FREERTOS */
#endif
}

#endif /* USART2 */
#endif /* _USART2_Function__C */
/************************************************************************************************************/