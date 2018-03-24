/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : IO_Kernel_Function.c
* Author             : OS Team
* Date First Issued  : 24/10/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 24/10/2014: v0.1
* 10/03/2015:	Updating...
* 14/05/2015:	Update Double click
* 15/05/2015:	Update Double click, triple click, remove eButtonPressToRelease, eButtonReleaseToPress, eButtonNoise, eButtonHoldOff
* 11/06/2015:	Update Time of Hold Click
* 25/06/2015:	Update Time of Hold Off
* 28/06/2015:	Adding parameter of Output IO Struct: timeline
* 29/06/2015:	Adding return function
* 30/06/2015:	Fix output function
* 01/07/2015:	Adding antinoise for button IO 
* 02/07/2015:	Adding start state in Output Function 
* 12/07/2015:	Fix loi kernel
* 26/07/2015:	Change end state if uTimeLine < uFrequency
* 28/10/2015:	adding some type of variable 
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Project includes. */
#include "IO_Kernel_Function.h"
#include "PointerFunctionDefine.h"
/* CYCLE 20 ms */
//#define TIME_LONG_PRESS_T1 		100	/* 100 cycle of Button Process 2s */
//#define TIME_LONG_PRESS_T2 		200	/* 200 cycle of Button Process ~ 4000ms = 4s */
//#define TIME_HOLD_ON 			300	/* 300 cycle of Button Process ~ 6000ms = 6s */
//#define COUNT_TIME_SAMPLE		40
/* CYCLE 10 ms */
#define ANTI_NOISE_PRESS		1	/* 2 cycle of Button Process 20ms */
#define ANTI_NOISE_RELEASE		1	/* 2 cycle of Button Process 20ms */
#define TIME_SINGLE_PRESS		10	/* 10 cycle of Button Process 100ms */
#define TIME_LONG_PRESS_T1 		100	/* 100 cycle of Button Process 1s */
#define TIME_LONG_PRESS_T2 		200	/* 200 cycle of Button Process ~ 2000ms = 2s */
#define TIME_HOLD_ON 			600	/* 600 cycle of Button Process ~ 6000ms = 6s */
#define TIME_HOLD_OFF 			10	/* 10 cycle of Button Process ~ 100ms */
#define TIME_HOLD_OFF_LONG_T1   100	/* 100 cycle of Button Process ~ 1s */
#define TIME_HOLD_OFF_LONG_T2   200	/* 200 cycle of Button Process ~ 2s */
#define TIME_HOLD_OFF_LONG      400	/* 400 cycle of Button Process ~ 4s */
#define COUNT_TIME_SAMPLE		40
/* Static variable about time Press & Release */
static uint32	uTimeCheck[NUMBER_IO_BUTTON_USE];
static uint32	uTimePress[NUMBER_IO_BUTTON_USE];
static uint32	bClickCount[NUMBER_IO_BUTTON_USE];
static uint32	bHoldOff[NUMBER_IO_BUTTON_USE];
static enumbool flag_start_sample[NUMBER_IO_BUTTON_USE];
	
/*************************************************************************************/
void vResetButonValue(eIndexButton bIndex)
{
	uTimeCheck[bIndex]=0;
	bClickCount[bIndex]=0;
	uTimePress[bIndex]=0;
	bHoldOff[bIndex]=0;
}

/*************************************************/
void vGetIOButtonValue(eInputState InputState, structIO_Button *OldValue, structIO_Button *NewValue, eIndexButton bIndex)
{
/* Start get new state */
#ifdef USE_BUTTON_IO
	/* eLogicLow button press, eLogicHigh button release*/
	if(InputState == eLogicHigh)
	{
		/* Base on the last state, caculate to change state */
		switch(OldValue->bButtonState[bIndex])
		{
		case eButtonPress:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonSingleClick:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonDoubleClick:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonTripleClick:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonLongPressT1:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonLongPressT2:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonHoldOn:
			if(bHoldOff[bIndex]>=ANTI_NOISE_RELEASE)
				NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		case eButtonRelease:
			if(bHoldOff[bIndex]>=TIME_HOLD_OFF)
				NewValue->bButtonState[bIndex] = eButtonHoldOff;
			break;
		case eButtonHoldOff:
            if(bHoldOff[bIndex]>=TIME_HOLD_OFF_LONG_T1)
				NewValue->bButtonState[bIndex] = eButtonHoldOffLongT1;
			break;
        case eButtonHoldOffLongT1:
            if(bHoldOff[bIndex]>=TIME_HOLD_OFF_LONG_T2)
				NewValue->bButtonState[bIndex] = eButtonHoldOffLongT2;
            break;
        case eButtonHoldOffLongT2:
            if(bHoldOff[bIndex]>=TIME_HOLD_OFF_LONG)
				NewValue->bButtonState[bIndex] = eButtonHoldOffLong;
            break;
        case eButtonHoldOffLong:
            break;
		}
		/* Increase time Hold Off */
		if(bHoldOff[bIndex]<TIME_HOLD_OFF_LONG)
			bHoldOff[bIndex]++;
		/* Reset Time counter */
		uTimePress[bIndex] = 0;
		NewValue->bButtonTime[bIndex] = 0;
	}
	/* Button press */
	else if(InputState == eLogicLow)
	{
		/* Base on the last state, caculate to change state */
		switch(OldValue->bButtonState[bIndex])
		{
		case eButtonRelease:
			bHoldOff[bIndex]=0;
			if(uTimeCheck[bIndex]>=ANTI_NOISE_PRESS)
			{
				NewValue->bButtonState[bIndex] = eButtonPress;
				bClickCount[bIndex]++;
			}
			flag_start_sample[bIndex] = eTRUE;
			break;
		case eButtonHoldOff:
//			bHoldOff[bIndex]=0;
//			if(uTimeCheck[bIndex]>=ANTI_NOISE_PRESS)
//			{
//				NewValue->bButtonState[bIndex] = eButtonPress;
//				bClickCount[bIndex]++;
//			}
//			flag_start_sample[bIndex] = eTRUE;
            NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
        case eButtonHoldOffLongT1:
			NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
        case eButtonHoldOffLongT2:
			NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
        case eButtonHoldOffLong:
			NewValue->bButtonState[bIndex] = eButtonRelease;
			break;
		}
		/* Increase counter */
		++uTimePress[bIndex];
	}

	/* Time End period */
	if(flag_start_sample[bIndex]==eTRUE)
		uTimeCheck[bIndex]++;
	if(uTimeCheck[bIndex]>=COUNT_TIME_SAMPLE)/* 10*20=200ms */
	{
		if((bClickCount[bIndex]==1)&&(uTimePress[bIndex]<=TIME_SINGLE_PRESS))
			NewValue->bButtonState[bIndex] = eButtonSingleClick;
		if(bClickCount[bIndex]==2)
			NewValue->bButtonState[bIndex] = eButtonDoubleClick;
		if(bClickCount[bIndex]==3)
			NewValue->bButtonState[bIndex] = eButtonTripleClick;
		/* Reset counter */
		bClickCount[bIndex] = 0;
		/* Reset */
		uTimeCheck[bIndex] = 0;
		/* Set flag sample */
		flag_start_sample[bIndex] = eFALSE;
	}
	/* Change mode */
	if(uTimePress[bIndex]>=TIME_HOLD_ON)
		NewValue->bButtonState[bIndex] = eButtonHoldOn;
	else if(uTimePress[bIndex]>=TIME_LONG_PRESS_T2)
		NewValue->bButtonState[bIndex] = eButtonLongPressT2;
	else if(uTimePress[bIndex]>=TIME_LONG_PRESS_T1)
		NewValue->bButtonState[bIndex] = eButtonLongPressT1;
	/* Update button press time */
	NewValue->bButtonTime[bIndex] = uTimePress[bIndex];
#endif
}

/* Output with specific IO function */
enumbool vIO_Output(structIO_Manage_Output *pOutput, IO_Struct *pControl)
{
        enumbool bReturn = eFALSE;
	if(pOutput->bCurrentProcess == eTRUE)
	{
		/* Increase counter */
		pOutput->uCycleCounter++;
		/* Check Frequency */
		if(((pOutput->uCycleCounter)%(pOutput->uFrequency))==0)
		{
			/* Check bFlagStart */
			if(pOutput->bFlagStart==eTRUE)
			{
				pOutput->bFlagStart = eFALSE;
				pControl->write(pOutput->bStartState);
				if(pOutput->uCountToggle!=0)
				{
					pOutput->uCountToggle--;
				}
			}
			else
			{
				if(pOutput->uCountToggle!=0)
				{
					pOutput->uCountToggle--;
					pControl->write(eTRUE - pControl->writeSta());
				}
				else
				{
					pControl->write(pOutput->bEndState);
				}
			}
			/* Check timeline with frequency */
			if(pOutput->uTimeLine >= pOutput->uFrequency)
			{
				/* Decrease uTimeline */
				pOutput->uTimeLine-= pOutput->uFrequency;
			}
			else
			{
				pOutput->bCurrentProcess = eFALSE;
				pOutput->uTimeLine = 0;
				/* Set end state - 26/07/2015 */
				pControl->write(pOutput->bEndState);
			}
		}
        bReturn = eTRUE;
	}
    else
    {
        bReturn = eFALSE;
    }
    return bReturn;
}
/* Set config parameter to output device */
enumbool vIO_ConfigOutput(structIO_Manage_Output *pOutput, uint32 uFrequency, uint32 uTimeline, uint32 uCountToggle, enumbool bStartState, enumbool bEndState, enumbool bFlagInterrupt)
{
	enumbool bFlagSetParaSucess = eFALSE;
	if(bFlagInterrupt==eTRUE)
	{
		/* Enable Flag Process */
		pOutput->bCurrentProcess = eTRUE;
		/* Config parameter */
		pOutput->uTimeLine		= uTimeline;
		pOutput->uFrequency 	= uFrequency;
		pOutput->uCountToggle 	= uCountToggle;
		pOutput->bEndState 		= bEndState;
		pOutput->bStartState 	= bStartState;
		pOutput->bFlagStart		= eTRUE;
		/* Reset Counter */
		pOutput->uCycleCounter	= uFrequency - 1;
		/* Set Flag Success */
		bFlagSetParaSucess		= eTRUE;
	}
	else
	{
		/* Check flag process */
		if(pOutput->bCurrentProcess == eFALSE) 
		{
			/* Enable Flag Process */
			pOutput->bCurrentProcess = eTRUE;
			/* Config parameter */
			pOutput->uTimeLine		= uTimeline;
			pOutput->uFrequency 	= uFrequency;
			pOutput->uCountToggle 	= uCountToggle;
			pOutput->bEndState 		= bEndState;
			pOutput->bStartState 	= bStartState;
			pOutput->bFlagStart		= eTRUE;
			/* Reset Counter */
			pOutput->uCycleCounter	= uFrequency - 1;
			/* Set Flag Success */
			bFlagSetParaSucess		= eTRUE;
		}
	}
	return bFlagSetParaSucess;
}
/* Get Current process  */
enumbool vIO_Get_Current_Process(structIO_Manage_Output *pOutput)
{
	return pOutput->bCurrentProcess;
}