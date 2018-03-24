/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : IO_Kernel_Task.c
* Author             : OS Team
* Date First Issued  : 24/10/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 24/10/2014: v0.1
* 15/05/2015: v1.0: set frequency from 10 to 5ms
* 15/05/2015: v1.0: set frequency IO button from 20 to 10ms
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_IO_Kernel_Task__C
#define _IO_Kernel_Task__C
/* File includes. */
#include "IO_Kernel_Task.h"
#include "IO_Kernel_Function.h"
#include "Time_Manage_Function.h"

/* Global Variable */
timer tIO_Input_Task, tIO_Output_Task;
extern enumbool xFlag_IO_Task_Still_Running, xFlag_IO_Task_Init_Done;
/* Global manage variable base on Function Device */
#ifdef USE_BUTTON_IO
	/* Extern pointer button */
	extern IO_Struct pBUT_1, pBUT_2, pBUT_3, pBUT_4;
	static timer tIO_Button_Task;
	structIO_Button strIO_Button_Value, strOld_IO_Button_Value;
#endif
#ifdef USE_ADC_IO
	static timer tIO_ADC_But_Task, tIO_ADC_But_Task;
#endif
#ifdef USE_RELAY
	extern IO_Struct pRELAY_1, pRELAY_2, pRELAY_3;
	extern structIO_Manage_Output strRELAY_1, strRELAY_2, strRELAY_3;
	static timer tIO_Relay_Task;
#endif
#ifdef USE_LED
	extern IO_Struct pLED1;
	extern structIO_Manage_Output strLED_1;
	static timer tIO_LED_Task;
#endif
#ifdef USE_BELL
	extern IO_Struct pBELL,pTRUNK;
	extern structIO_Manage_Output strBELL,strTRUNK;
	static timer tIO_BELL_Task;
#endif
/*-----------------------------------------------------------*/
void vIO_Kernel_Task( void *pvParameters )
{
	/* Init Timer task IN & OUT */
	timer_set(&tIO_Input_Task, 5 ,CLOCK_TYPE_MS);/* 5ms */
	timer_set(&tIO_Output_Task, 5 ,CLOCK_TYPE_MS);/* 5ms */

	/********************************************************/
	/* Init function & start value for INPUT device */
	#ifdef USE_BUTTON_IO
	/* Reset Flag new button */
	strIO_Button_Value.bFlagNewButton =  eFALSE;
	/* Init timer frequency */
	timer_set(&tIO_Button_Task, 10 ,CLOCK_TYPE_MS);/* 10ms */
		#ifdef USE_BUTTON_IO_1
			strIO_Button_Value.bButtonState[eButton1] = eButtonRelease;
			strOld_IO_Button_Value.bButtonState[eButton1] = eButtonRelease;
		#endif /* USE_BUTTON_IO_1 */
		#ifdef USE_BUTTON_IO_2
			strIO_Button_Value.bButtonState[eButton2] = eButtonRelease;
			strOld_IO_Button_Value.bButtonState[eButton2] = eButtonRelease;
		#endif /* USE_BUTTON_IO_2 */
		#ifdef USE_BUTTON_IO_3
			strIO_Button_Value.bButtonState[eButton3] = eButtonRelease;
			strOld_IO_Button_Value.bButtonState[eButton3] = eButtonRelease;
		#endif /* USE_BUTTON_IO_3 */
		#ifdef USE_BUTTON_IO_4
			strIO_Button_Value.bButtonState[eButton4] = eButtonRelease;
			strOld_IO_Button_Value.bButtonState[eButton4] = eButtonRelease;
		#endif /* USE_BUTTON_IO_4 */
	#endif /* USE_BUTTON_IO */
	#ifdef USE_ADC_IO
	timer_set(&tIO_ADC_But_Task, 10 ,CLOCK_TYPE_MS);/* 10ms, ADC frequency sample */
	#endif /* USE_ADC_IO */
	/********************************************************/
	
	/********************************************************/
	/* Init function & start value for OUTPUT device */
	#ifdef USE_RELAY
	timer_set(&tIO_Relay_Task, 10 ,CLOCK_TYPE_MS);/* 10ms, Relay Process */
	#endif /*USE_RELAY*/
	#ifdef USE_LED
	timer_set(&tIO_LED_Task, 10 ,CLOCK_TYPE_MS);/* 10ms, LED Process */
	#endif /*USE_LED*/
	#ifdef USE_BELL
	timer_set(&tIO_BELL_Task, 10 ,CLOCK_TYPE_MS);/* 10ms, BELL Process */
	#endif /* USE_BELL */
	
	/********************************************************/
	
	/* Task BEGIN */
	/* Init frequency of IO_Kernel_Task */
	portTickType xLastWakeTime;
	const portTickType xIO_Kernel_Task_Frequency = 5;/* 5 tick slice ~ 5ms */
	xLastWakeTime = xTaskGetTickCount();
	/* Set flag Init done */
	xFlag_IO_Task_Init_Done = eTRUE;
	/* Task process */
	for(;;)
	{
		/* Delay Exactly Frequency */
		OS_vTaskDelayUntil(&xLastWakeTime,xIO_Kernel_Task_Frequency);
		
		/* SET xFlag_IO_Task_Still_Running */
		xFlag_IO_Task_Still_Running = eTRUE;
		/* Input Task: IO Button, ADC but, ADC sensor... process when timer expired */
		fIO_Input_Task();
		/* Output Task: Relay, LED... */
		fIO_Output_Task();
	}
}

/* Input Process Task, all function input will run here. */
void fIO_Input_Task( void )
{
	/* Check timer expired */
	if(timer_expired(&tIO_Input_Task))
	{
		/* Reset timer manage */
		//timer_reset(&tIO_Input_Task);
        timer_restart(&tIO_Input_Task);
		/* Process Function Device */
		#ifdef USE_BUTTON_IO
		fIO_Button_Process();
		#endif
	}
}

/* Output Process Task, all function Output will run here. */
void fIO_Output_Task( void )
{
	/* Check timer expired */
	if(timer_expired(&tIO_Output_Task))
	{
		/* Reset timer manage */
		//timer_reset(&tIO_Output_Task);
        timer_restart(&tIO_Output_Task);
		#ifdef USE_RELAY
			fIO_Relay_Process();
		#endif /* USE_RELAY */
		#ifdef USE_LED
			fIO_LED_Process();
		#endif /* USE_LED */
		#ifdef USE_BELL
			fIO_BELL_Process();
		#endif /* USE_BELL */
	}
}

/* IO Button Process */
void fIO_Button_Process(void)
{
	/* Check timer expired */
	if(timer_expired(&tIO_Button_Task))
	{
		/* Reset timer manage */
		//timer_reset(&tIO_Button_Task);
        timer_restart(&tIO_Button_Task);
	#ifdef USE_BUTTON_IO
		#ifdef USE_BUTTON_IO_1
			vGetIOButtonValue(pBUT_1.read(), &strOld_IO_Button_Value, &strIO_Button_Value, eButton1);
		#endif /* USE_BUTTON_IO_1 */
		#ifdef USE_BUTTON_IO_2
			vGetIOButtonValue(pBUT_2.read(), &strOld_IO_Button_Value, &strIO_Button_Value, eButton2);
		#endif /* USE_BUTTON_IO_2 */
		#ifdef USE_BUTTON_IO_3
			vGetIOButtonValue(pBUT_3.read(), &strOld_IO_Button_Value, &strIO_Button_Value, eButton3);
		#endif /* USE_BUTTON_IO_2 */
		#ifdef USE_BUTTON_IO_4
			vGetIOButtonValue(pBUT_4.read(), &strOld_IO_Button_Value, &strIO_Button_Value, eButton4);
		#endif /* USE_BUTTON_IO_2 */
		/* Process if get new value button. */
		if(memcmp(strOld_IO_Button_Value.bButtonState,strIO_Button_Value.bButtonState,NUMBER_IO_BUTTON_USE))
		{
			/* Update OLD data value */
			memcpy(strOld_IO_Button_Value.bButtonState,strIO_Button_Value.bButtonState, NUMBER_IO_BUTTON_USE);
			/* Set flag new button */
			strIO_Button_Value.bFlagNewButton = eTRUE;
		}
		else
		{
			/* Clear flag new button */
			strIO_Button_Value.bFlagNewButton = eFALSE;
		}
	#endif /* USE_BUTTON_IO */
	}
}

/* Output Relay Process */
void fIO_Relay_Process(void)
{	
	/* Check time expired */
	if(timer_expired(&tIO_Relay_Task))
	{
        timer_restart(&tIO_Relay_Task);
		/* Process Relay 1 */
		vIO_Output(&strRELAY_1, &pRELAY_1);
		/* Process Relay 2 */
		vIO_Output(&strRELAY_2, &pRELAY_2);
		/* Process Relay 3 */
		vIO_Output(&strRELAY_3, &pRELAY_3);
	}
}

/* Output LED Process */
void fIO_LED_Process(void)
{	
	/* Check time expired */
	if(timer_expired(&tIO_LED_Task))
	{
        timer_restart(&tIO_LED_Task);
		/* Process LED 1 */
		vIO_Output(&strLED_1, &pLED1);
	}
}

/* Output BELL & TRUNK Process */
void fIO_BELL_Process(void)
{	
  #ifdef USE_BELL
	/* Check time expired */
	if(timer_expired(&tIO_BELL_Task))
	{
        timer_restart(&tIO_BELL_Task);
		/* Process BELL */
		vIO_Output(&strBELL, &pBELL);
		vIO_Output(&strTRUNK, &pTRUNK);
	}
  #endif /* USE_BELL */
}
#endif /* _IO_Kernel_Task__C */