/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Timer_Function.h
* Author             : Hai Dang
* Date First Issued  : 21-Sep-2014
* Description        : global prototype, extern variable to init, manage timer function

********************************************************************************
* History: DD/MM/YY
* 21/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Timer_Function__H
#define _Timer_Function__H
/* Include Header file */
#include "Common.h"
#include "Timer_Function_Profile.h"
	#ifdef USE_TIMER_BASE_FUNCTION
		/* Define interrupt frequency want to use, Hz -> 10Khz*/
		#define INT_Frequency	10000
		/* Define timer clock want to use, Hz -> 1000000Khz */
		#define TIMER_CLOCK		1000000
		/* The highest available interrupt priority. */
		#define TimerHIGHEST_PRIORITY			( 0 )
		
		/* Extern Global variable */
		extern volatile unsigned portLONG uTimeHighResolution;
		extern volatile unsigned portLONG uUnresetTimeHighResolution;
		extern volatile unsigned portLONG uTimeMsCounter;
		/* Init Timer base function */
		void vSetupTimerBaseFunction( void );
		/* Interrupt handler Hight Resolution Timer */
		void vTimerBaseInt_Handler( void );
		/* Get Current Time counter us */
		uint32_t vGetCurrentCounterTimeBaseUs( void );
		/* Get Current Time counter Ms */
		uint32_t vGetCurrentCounterTimeBaseMs( void );
		/* Get Current Time counter S */
		uint32_t vGetCurrentCounterTimeBaseS( void );
		/* Timer delay function */
		void vTimerBase_DelayMS( uint32_t uDelayMS );
		void vTimerBase_DelayUS( uint32_t uDelayUS );
	#endif /* USE_TIMER_BASE_FUNCTION */
#endif /* _Timer_Function__H*/