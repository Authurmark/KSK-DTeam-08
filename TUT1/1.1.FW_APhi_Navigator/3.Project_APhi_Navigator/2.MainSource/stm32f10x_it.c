/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Common.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
//  printf("[SYSTEM DEBUG]: HARDFAULT\r\n");
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/* SCV_Handler Interrupt */
void SVC_Handler (void)
{
#ifdef USE_FREERTOS
	#warning: [BUILD_WARNING]: use with vPortSVCHandler() in startup.s and portasm.s file;
#endif
}

/* PendSV_Handler Interrupt */
void PendSV_Handler(void)
{
#ifdef USE_FREERTOS
	#warning: [BUILD_WARNING]: use with xPortPendSVHandler() in startup.s and portasm.s file;
#endif
}

/* SysTick_Handler Interrupt */
void SysTick_Handler(void)
{
#ifdef USE_FREERTOS
	#warning: [BUILD_WARNING]: use with xPortSysTickHandler() in startup.s and portasm.s file;
#endif
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External lines 0 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern volatile enumbool bFlag_ACCE_Int;
void EXTI0_IRQHandler(void)
{
#ifdef USE_FREERTOS
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
#endif

#ifdef USE_FREERTOS
  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
}
/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External lines 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern volatile enumbool bFlag_RF_Int;
void NRF24L01_IRQ_HANDLER(void)
{
extern IO_Struct pNRF24L01_IO_IRQ;
#ifdef USE_FREERTOS
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
#endif

#ifdef USE_FREERTOS
  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
}

/*******************************************************************************
* Function Name  : BUTTON 1 Handler
* Description    : This function handles BUTTON 1 Handler interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern volatile enumbool bFlag_Key_Bike_Int;
void EXT_BUTTON_1_EXT_HANDLER(void)
{
#ifdef USE_FREERTOS
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
#endif
  if(EXTI_GetITStatus(EXT_BUTTON_1_LINE) != RESET)
  {  
    /* Clear the EXTI line 3 pending bit */
    EXTI_ClearITPendingBit(EXT_BUTTON_1_LINE);
    /* Set flag EXT line key */
    bFlag_Key_Bike_Int = eTRUE;
  }
#ifdef USE_FREERTOS
  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
}

/*******************************************************************************
* Function Name  : BUTTON 2 Handler
* Description    : This function handles BUTTON 2 Handler interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern volatile enumbool bFlag_Leg_Bike_Int;
void EXT_BUTTON_2_EXT_HANDLER(void)
{
#ifdef USE_FREERTOS
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
#endif
  if(EXTI_GetITStatus(EXT_BUTTON_2_LINE) != RESET)
  {  
    /* Clear the EXTI line 7 pending bit */
    EXTI_ClearITPendingBit(EXT_BUTTON_2_LINE);
    /* Set flag EXT line Leg */
    bFlag_Leg_Bike_Int = eTRUE;
  }
#ifdef USE_FREERTOS
  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
}

/*******************************************************************************
* Function Name  : BUTTON 1&2 Handler
* Description    : This function handles BUTTON 1&2 Handler interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_BUTTON_1_2_EXT_HANDLER(void)
{
extern IO_Struct pBUT_1, pBUT_2;
#ifdef USE_FREERTOS
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
#endif
  /* Check button 1*/
  if(EXTI_GetITStatus(EXT_BUTTON_1_LINE) != RESET)
  {  
    /* Clear the EXTI line 3 pending bit */
    EXTI_ClearITPendingBit(EXT_BUTTON_1_LINE);
    /* Check IO signal Low Logic */
    if(GPIO_ReadInputDataBit(BUTTON_1_PORT, BUTTON_1_PIN)==eFALSE)
    {
      /* Set flag EXT line key */
      bFlag_Key_Bike_Int = eTRUE;
    }
  }
  /* Check button 2*/
  if(EXTI_GetITStatus(EXT_BUTTON_2_LINE) != RESET)
  {  
    /* Clear the EXTI line 7 pending bit */
    EXTI_ClearITPendingBit(EXT_BUTTON_2_LINE);
    /* Check IO signal Low Logic */
    if(GPIO_ReadInputDataBit(BUTTON_2_PORT, BUTTON_2_PIN)==eFALSE)
    {
      /* Set flag EXT line Leg */
      bFlag_Leg_Bike_Int = eTRUE;
    }
  }
#ifdef USE_FREERTOS
  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
#endif
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
