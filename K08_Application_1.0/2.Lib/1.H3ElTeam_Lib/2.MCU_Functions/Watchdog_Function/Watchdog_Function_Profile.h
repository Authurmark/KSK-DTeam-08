/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Watchdog_Function_Profile.h
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
#ifndef	_WATCHDOG_Function_Profile__H
#define _WATCHDOG_Function_Profile__H
/* Include global header file */
#include "Watchdog_Function_Profile.h"
/* Include global header file */
#include "Common.h"

/* Define Watchdog Function */
/* Uncomment to use Watchdog, comment to disable*/
	#define USE_WATCHDOG
	/* Uncomment to use Independent Watchdog */
	#define USE_IWDG
	#ifdef USE_IWDG
		#define IWDG_CLK_Prescaler 		IWDG_Prescaler_64
		#define IWDG_ReloadCounterValue ((uint16_t) 2000 ) //~3.2s
	#endif /* USE_IWDG */
	/* Uncomment to use Window Watchdog */
	//#define USE_WWDG
	#ifdef USE_WWDG
		#define WWDG_CLK				24000000		//36MHz
		#define WWDG_Periph				RCC_APB1Periph_WWDG
		#define WWDG_Periph_Cmd			RCC_APB1PeriphClockCmd
		#define WWDG_CLK_Prescale		WWDG_Prescaler_8
		#define WWDG_PrescalerCLK		( WWDG_CLK / 8 )	//4.5MHz (~0.22us)
		#define WWDG_CounterCLK			( WWDG_CLK_Prescale / 4096 )	//not change, ~1098.6Hz	(~0.91ms)
		#define WWDG_SetCounterValue	((uint8_t) 127)
		#define WWDG_EnableCounter		((uint8_t) 127)
		#define WWDG_SetWindowCounter	((uint8_t) 80)
		#define WWDG_MinTime			((float)(WWDG_SetCounter - WWDG_MinCounter) / WWDG_CounterCLK )
		#define WWDG_MaxTime			((float)(WWDG_SetCounter - WWDG_SetWindowCounter) / WWDG_CounterCLK )
	#endif /* USE_WWDG */
#endif /*_WATCHDOG_Function_Profile__H*/