/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Timer_Function_Profile.h
* Author             : OS Team
* Date First Issued  : 23/09/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

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
#include "Common.h"
#ifndef	_Timer_Function_Profile__H
#define _Timer_Function_Profile__H
/************************************************************************************************************/
/* Define Timer Base Function*/
/* Uncomment to use Timer Base function, comment to disable, not recommend DISABLE */
#define USE_TIMER_BASE_FUNCTION
	#define USE_TIMER_BASE_INTERRUPT
	#define USE_OS_TRACKING_MCU_LOAD

/************************************************************************************************************/
/* Detail Timer function */
#ifdef USE_TIMER_BASE_FUNCTION
	#define TIMER_BASE_USE				TIM2
	#define TIMER_BASE_RCC				RCC_APB1Periph_TIM2
	#define TIMER_BASE_CLK_CMD 			RCC_APB1PeriphClockCmd
	#ifdef USE_TIMER_BASE_INTERRUPT		
		#define TIMER_BASE_IRQ 			TIM2_IRQn
		#define vTimerBaseInt_Handler	TIM2_IRQHandler
	#endif
#endif
#endif