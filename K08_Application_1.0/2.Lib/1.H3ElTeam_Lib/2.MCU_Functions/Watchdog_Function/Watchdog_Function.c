/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Watchdog_Function.c
* Author             : Hai Dang, Hoang Dinh
* Date First Issued  : 10-Nov-2014
* Description        : global function struct to manage and pointer function.

********************************************************************************
* History: DD/MM/YY
* 29/01/2015: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_RTC_Function__C
#define _RTC_Function__C
/* Include header file */
#include "Watchdog_Function.h"

#ifdef USE_WATCHDOG
	#ifdef USE_IWDG
		/* IWDG init function */
		void IWDG_Init(void)
		{
			/* Enable write access */
			IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
			/* Set prescaler for watchdog */
			IWDG_SetPrescaler(IWDG_CLK_Prescaler);
			/* Set Counter for watchdog */
			IWDG_SetReload(IWDG_ReloadCounterValue);
		}
		/* Check reset by IWDG or not */
		enumbool IWDG_CheckReset(void)
		{
			if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
				return eTRUE;
			else 
                return eFALSE;
            RCC_ClearFlag();
		}
		/* Enable and reload Counter */
		void IWDG_Start_Countdown(void)
		{
            IWDG_Enable();
			IWDG_ReloadCounter();
		}
	#endif /* USE_IWDG */
	#ifdef USE_WWDG
		/* Variable counter */
		uint16_t bWatchdogCounter = 0;
		/* WWDG init function */
		void WWDG_Init(void)
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
			WWDG_SetPrescaler(WWDG_PrescalerCLK);
			WWDG_SetWindowValue(WWDG_SetWindowCounter);
		}
		/* Enable and reload Counter */
		void WWDG_Start_Countdown(void)
		{
            WWDG_Enable(WWDG_EnableCounter);
		}
		/* Set/Reload counter */
		void WWDG_Set_Reload_Counter(void)
		{
			WWDG_SetCounter(WWDG_SetCounterValue);
		}
	#endif /* USE_IWDG */
#endif /* USE_WATCHDOG */
#endif /*_RTC_Function__C*/
