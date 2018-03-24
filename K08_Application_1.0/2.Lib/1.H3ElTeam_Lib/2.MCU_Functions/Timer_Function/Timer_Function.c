/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Timer_Function.c
* Author             : Hai Dang
* Date First Issued  : 21-Sep-2014
* Description        : global function struct to init, manage timer function.

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
#ifndef	_Timer_Function__C
#define _Timer_Function__C
/* Include header file */
#include "Timer_Function.h"
#ifdef USE_TIMER_BASE_FUNCTION
	/* Global variable */
    #ifdef USE_OS
	volatile unsigned portLONG uTimeHighResolution=0;
	volatile unsigned portLONG uTimeMsCounter=0, uTimeSCounter=0;
	volatile unsigned portLONG uUnresetTimeHighResolution=0;
    #endif /* USE_OS */
    #ifndef USE_OS
    volatile unsigned uTimeHighResolution=0;
	volatile unsigned uTimeMsCounter=0;
	volatile unsigned uUnresetTimeHighResolution=0;
    #endif /* Not USE_OS */
#endif
#ifdef USE_TIMER_BASE_FUNCTION
	void vSetupTimerBaseFunction(void)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Temp variable */
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		/* Enable timer clocks */
	    TIMER_BASE_CLK_CMD( TIMER_BASE_RCC, ENABLE );
		/* Initialise data. */
	    TIM_DeInit( TIMER_BASE_USE );
	    TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
		/* Caculate timer */
		/* With system clock 8Mhz, Prescaler 79 will make 100Khz clock */
		/* Count for interrupt is 9 mean clock interrupt is 100Khz/(9+1) = 10Khz */
		/* Time base configuration for timer 2 - which generates the interrupts. */	
        #ifdef USE_OS
		TIM_TimeBaseStructure.TIM_Prescaler = configCPU_CLOCK_HZ/TIMER_CLOCK - 1;//0x0; 8Mhz/80=100Khz, 8Mhz/40=200Khz; 8Mhz/20=400Khz;
        #endif /* USE_OS */
        #ifndef USE_OS
        TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/TIMER_CLOCK - 1;//0x0; 8Mhz/80=100Khz, 8Mhz/40=200Khz; 8Mhz/20=400Khz;
        #endif /* Not use OS */
		TIM_TimeBaseStructure.TIM_Period = TIMER_CLOCK/INT_Frequency - 1 ;
		//TIM_TimeBaseStructure.TIM_Period = ( unsigned portSHORT ) 0xffff;
		//TIM_TimeBaseStructure.TIM_Prescaler = 799;//10Khz
		//TIM_TimeBaseStructure.TIM_Period = ( unsigned portSHORT ) 10000 - 1;;//100ms interrupt
	    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	    TIM_TimeBaseInit( TIMER_BASE_USE, &TIM_TimeBaseStructure );

		#ifdef USE_TIMER_BASE_INTERRUPT		
		NVIC_InitTypeDef NVIC_InitStructure;
		TIM_ITConfig( TIMER_BASE_USE, TIM_IT_Update, ENABLE );
	    NVIC_InitStructure.NVIC_IRQChannel = TIMER_BASE_IRQ;
	    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init( &NVIC_InitStructure );	
		#endif /* USE_TIMER_BASE_INTERRUPT */

	    /* Finally, enable timers. */
	    TIM_Cmd( TIMER_BASE_USE, ENABLE );
		#endif/* USE_CMCIS_DRIVER */

		#ifdef USE_ONLY_REGISTER
		#endif
	}

	#ifdef USE_TIMER_BASE_INTERRUPT
	void vTimerBaseInt_Handler( void )
	{
		#ifdef USE_COOCOXOS /* Stop process to in interrupt */
			
		#endif

		#ifdef USE_CMCIS_DRIVER
			#ifdef USE_FREERTOS
			portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
			#endif
			if(TIM_GetITStatus(TIMER_BASE_USE,TIM_IT_Update)==SET)
			{
				/* Reset counter register */
				TIM_SetCounter(TIMER_BASE_USE,0);
				/* Increase counter */
				uTimeHighResolution++;
				uUnresetTimeHighResolution++;
				/* Increase tick ms counter */
				if(uTimeHighResolution%10==0) 
				{
					uTimeMsCounter++;
					/* Increase tick s counter */
					if(uTimeMsCounter%1000==0) uTimeSCounter++;
				}
				/* Clear flag */
				TIM_ClearITPendingBit( TIMER_BASE_USE, TIM_IT_Update );
				
				#ifdef USE_WATCHDOG
					#ifdef USE_IWDG
						static uint16_t bCountReloadWatchdog = 0;
						if(bCountReloadWatchdog>1000)/*1000*100us=100ms*/
						{
							IWDG_ReloadCounter();
							bCountReloadWatchdog = 0;
						}
						else
						{
							bCountReloadWatchdog++;
						}
					#endif /* USE_IWDG */
					#ifdef USE_WWDG
						/* Extern for watchdog */
						extern uint16_t bWatchdogCounter;
						if(bWatchdogCounter>=200)/*440*100us=44ms???*/
						{
							WWDG_Set_Reload_Counter();
							bWatchdogCounter = 0;
						}
						else
						{
							bWatchdogCounter++;
						}
					#endif /* USE_WWDG */
				#endif /* USE_WATCHDOG */
			}
			#ifdef USE_FREERTOS
			portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
			#endif
		#endif
		
		#ifdef USE_ONLY_REGISTER
		#endif

		#ifdef USE_COOCOXOS /* Stop process to out interrupt */
			
		#endif
	}
	#endif /* USE_TIMER_BASE_INTERRUPT */
	
	/* Output Current Timer Counter, us */
	uint32_t vGetCurrentCounterTimeBaseUs( void )
	{
		#ifdef USE_CMCIS_DRIVER
			return uUnresetTimeHighResolution * 100 + TIM_GetCounter(TIMER_BASE_USE);
		#endif
		
		#ifdef USE_ONLY_REGISTER
			return (uUnresetTimeHighResolution * 100 + TIMER_BASE_USE->CNT);
		#endif
	}
	/* Output Current Timer Counter, Ms */
	uint32_t vGetCurrentCounterTimeBaseMs( void )
	{
		return uTimeMsCounter;
	}
	/* Output Current Timer Counter, s */
	uint32_t vGetCurrentCounterTimeBaseS( void )
	{
		return uTimeSCounter;
	}
	/* Timer delay MS function */
	void vTimerBase_DelayUS( uint32_t uDelayUS )
	{
		uint32_t uTempDelay;
		#ifdef USE_CMCIS_DRIVER
			#ifdef USE_TIMER_BASE_INTERRUPT
				/* Get Current Time */
				uTempDelay = vGetCurrentCounterTimeBaseUs() + uDelayUS;/* ms */
				while(uTempDelay>=vGetCurrentCounterTimeBaseUs());
			#endif
		#endif
	}
	/* Timer delay MS function */
	void vTimerBase_DelayMS( uint32_t uDelayMS )
	{
		uint32_t uTempDelay;
		#ifdef USE_CMCIS_DRIVER
			#ifdef USE_TIMER_BASE_INTERRUPT
				/* Get Current Time */
				uTempDelay = vGetCurrentCounterTimeBaseMs() + uDelayMS;/* ms */
				while(uTempDelay>=vGetCurrentCounterTimeBaseMs());
			#endif
		#endif
	}
#endif /* USE_TIMER_BASE_FUNCTION */
#endif /* _Timer_Function__C*/