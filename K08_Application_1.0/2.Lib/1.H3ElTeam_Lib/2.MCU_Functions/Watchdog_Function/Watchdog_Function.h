/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Watchdog_Function.h
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
#ifndef	_WATCHDOG_Function__H
#define _WATCHDOG_Function__H
/* Include global header file */
#include "Watchdog_Function_Profile.h"

#ifdef USE_WATCHDOG
	/* Prototype function */
	#ifdef USE_IWDG
		void IWDG_Init(void);
		enumbool IWDG_CheckReset(void);
		void IWDG_Start_Countdown(void);
	#endif /* USE_IWDG */
	#ifdef USE_WWDG
		void WWDG_Init(void);
		void WWDG_Start_Countdown(void);
		void WWDG_Set_Reload_Counter(void);
	#endif /* USE_WWDG */
#endif /* USE_WATCHDOG */
#endif /*_WATCHDOG_Function__H*/