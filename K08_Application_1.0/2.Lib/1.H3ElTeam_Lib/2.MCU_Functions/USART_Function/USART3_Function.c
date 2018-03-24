/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : USART3_Function.c
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
/* Include global header file */
#include "USART3_Function.h"
#ifndef	_USART3_Function__C
#define _USART3_Function__C
#ifdef USE_USART3
/* Extern RINGBUF for USART3 */
extern RINGBUF USART3_RXRingBuff;
extern RINGBUF USART3_TXRingBuff;
extern uint8_t USART3_RXBuff[DEFAULT_USART3_NUMBER_BYTES_RX_BUFFER];
extern uint8_t USART3_TXBuff[DEFAULT_USART3_NUMBER_BYTES_TX_BUFFER];
/* USART3 Init function */
void USART3_Init( void )
{
	#ifdef USE_CMCIS_DRIVER
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	#ifdef USE_USART3
		#ifdef USE_DEFAULT_USART3_IO
		#warning: USART3 function use GPIOA_GPIO_Pin_9 and GPIOA_GPIO_Pin_10
        /* Enable USART3 clock */
		USART3_CLK_CMD(USART3_RCC,ENABLE);
		/* Enable clock IO */
        USART3_RX_CLK_CMD(USART3_RX_RCC_PORT,ENABLE);
		USART3_TX_CLK_CMD(USART3_TX_RCC_PORT,ENABLE);
        
        /* Configure USART3 Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = USART3_RX_PIN;
		GPIO_InitStructure.GPIO_Mode = USART3_RX_MODE;
		GPIO_InitStructure.GPIO_Speed = USART3_RX_SPEED;
        GPIO_Init( USART3_RX_PORT, &GPIO_InitStructure );
        
        /* Configure USART3 Tx as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN;
        GPIO_InitStructure.GPIO_Speed = USART3_TX_SPEED;
        GPIO_InitStructure.GPIO_Mode = USART3_TX_MODE;
        GPIO_Init( USART3_TX_PORT, &GPIO_InitStructure );
		#endif /* USE_DEFAULT_USART3_IO */

		/* DeInit USART3 */
//        USART_DeInit( USART3 );
        /* Init USART3 */
        USART_Init( USART3, &USART3_InitStructure );
        /* USART enable */
        USART_Cmd( USART3, ENABLE );	
			
        #ifdef USE_TX_INTERRUPT_USART3
		/* Enable IT TXE, use to get data bytes */
        USART_ITConfig( USART3, USART_IT_TXE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	USART3_TX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		USART3_TX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif
		#ifdef USE_RX_INTERRUPT_USART3
		/* Enable IT RXNE, use to get data bytes */
        USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 	USART3_RX_INTERRUPT_PRE_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 		USART3_RX_INTERRUPT_SUB_PRIORITY;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init( &NVIC_InitStructure );
		#endif

		#ifdef USE_DMA_USART3
        /* Config DMA */
        /* DMA clock enable */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        
        /* USART3_TX_DMA_Channel (triggered by USART3 TX event) Config */
        DMA_DeInit(DMA_USART3_TX_CHANNEL);
        DMA_Init(DMA_USART3_TX_CHANNEL, &DMA_USART3_TX_InitStructure);
        DMA_Cmd(DMA_USART3_TX_CHANNEL, DISABLE);
        /* USART3_RX_DMA_Channel (triggered by USART3 RX event) Config */
        DMA_DeInit(DMA_USART3_RX_CHANNEL);
        DMA_Init(DMA_USART3_RX_CHANNEL, &DMA_USART3_RX_InitStructure);
        DMA_Cmd(DMA_USART3_RX_CHANNEL, DISABLE);
        
        /* Enable USART3 DMA TX request */
        USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
        /* Enable USART3 DMA RX request */
        USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
        
        /* Enable DMA_USART3_TX_CHANNEL */
        DMA_Cmd(DMA_USART3_TX_CHANNEL, ENABLE);
        /* Enable DMA_USART3_RX_CHANNEL */
        DMA_Cmd(DMA_USART3_RX_CHANNEL, ENABLE);
		
		/* While DMA transfer complete */
		while(DMA_GetFlagStatus(DMA_USART3_TX_COMPLETE_FLAG)==RESET);
		#endif /* USE_DMA_USART3 */
	#endif /* USE_CMCIS_DRIVER */
	
	#ifdef USE_ONLY_REGISTER
	#endif /* USE_ONLY_REGISTER */

	/* Init ringbuf buffer */
	RINGBUF_Init(&USART3_TXRingBuff,USART3_TXBuff,DEFAULT_USART3_NUMBER_BYTES_TX_BUFFER);
	RINGBUF_Init(&USART3_RXRingBuff,USART3_RXBuff,DEFAULT_USART3_NUMBER_BYTES_RX_BUFFER);
	#endif /* USE_USART3 */
}

/* Function close and deinit USART3 */
void USART3_Close( void )
{
	#ifdef USE_CMCIS_DRIVER
	USART_DeInit(USART3);
	#endif
}

/* Function start Send byte USART3 */
void USART3_StartSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_USART3
		USART_ITConfig( USART3, USART_IT_TXE, ENABLE );
		#endif /* USE_TX_INTERRUPT_USART3 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Send Byte USART3 */
void USART3_StopSend( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_TX_INTERRUPT_USART3
		USART_ITConfig( USART3, USART_IT_TXE, DISABLE );
		#endif /* USE_TX_INTERRUPT_USART3 */
	#endif /* USE_CMCIS_DRIVER */
}
/* Function start Receive byte USART3 */
void USART3_StartReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_USART3
		USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );
		#endif /* USE_TX_INTERRUPT_USART3 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Function stop Receive Byte USART3 */
void USART3_StopReceive( void )
{
	#ifdef USE_CMCIS_DRIVER
		#ifdef USE_RX_INTERRUPT_USART3
		USART_ITConfig( USART3, USART_IT_RXNE, DISABLE );
		#endif /* USE_TX_INTERRUPT_USART3 */
	#endif /* USE_CMCIS_DRIVER */
}

/* Send 1 Byte to TX RingBuf */
void USART3_Send_Byte_TX_RingBuf( unsigned char cChar )
{
	RINGBUF_Put(&USART3_TXRingBuff,cChar);
}

/* Get 1 Byte From RX RingBuf */
enumbool USART3_Get_Byte_RX_RingBuf(  char *pChar  )
{
	if(RINGBUF_Get(&USART3_RXRingBuff,pChar)==0)/* Success */
		return eTRUE;
	else 
		return eFALSE;
}

/* Interrupt handler function */
void USART3_IRQHandler( void )
{
#ifdef USE_USART3
	#ifdef USE_FREERTOS
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    #endif /* USE_FREERTOS */
	static unsigned char cChar = 0xFF ;
	#ifdef USE_CMCIS_DRIVER
	/* RX Interrupt handler */
    if( USART_GetITStatus( USART3, USART_IT_RXNE ) == SET )
    {
        cChar = USART_ReceiveData( USART3 );
		RINGBUF_Put(&USART3_RXRingBuff,cChar);
    }	
	/* TX Interrupt handler */
    else if( USART_GetITStatus( USART3, USART_IT_TXE ) == SET )
    {
        /* Read from ringbuf, send if have any data */
		if(RINGBUF_Get(&USART3_TXRingBuff,&cChar) == 0)
        {
			USART_SendData(USART3, cChar);
        }
        else /* Disable TXE when finish transfer */
        {
            USART3_StopSend();	
        }		
    }
	else cChar = USART_ReceiveData( USART3 );
	#endif
    #ifdef USE_FREERTOS
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	#endif /* USE_FREERTOS */
#endif
}

#endif /* USART3 */
#endif /* _USART3_Function__C */
/************************************************************************************************************/