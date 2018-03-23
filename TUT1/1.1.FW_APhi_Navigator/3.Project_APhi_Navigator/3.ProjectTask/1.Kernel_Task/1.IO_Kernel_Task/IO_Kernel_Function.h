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
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_IO_Kernel_Function__H
#define _IO_Kernel_Function__H

/* Project includes. */
#include "Common.h"
#include "IO_Function_Profile.h"

/* Funtion Prototype */
void vResetButonValue(eIndexButton bIndex);
void vGetIOButtonValue(eInputState InputState, structIO_Button *OldValue, structIO_Button *NewValue, eIndexButton bIndex);
enumbool vIO_Output(structIO_Manage_Output *pOutput, IO_Struct *pControl);
enumbool vIO_ConfigOutput(structIO_Manage_Output *pOutput, uint32 uFrequency, uint32 uTimeline, uint32 uCountToggle, enumbool bStartState, enumbool bEndState, enumbool bFlagInterrupt);
enumbool vIO_Get_Current_Process(structIO_Manage_Output *pOutput);
#endif /* _IO_Kernel_Function__H */