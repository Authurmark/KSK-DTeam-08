/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : IO_Kernel_Task.h
* Author             : OS Team
* Date First Issued  : 24/10/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 24/10/2014: v0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_IO_Kernel_Task__H
#define _IO_Kernel_Task__H
/* File includes. */
#include "IO_Function_Profile.h"
/*-----------------------------------------------------------*/
/* Funtion Prototype */
void vIO_Kernel_Task( void *pvParameters );
void fIO_Input_Task( void );
void fIO_Output_Task( void );

/* Function Device Prototype */
void fIO_Button_Process( void );
void fIO_Relay_Process( void );
void fIO_LED_Process( void );
void fIO_BELL_Process( void );

#endif /* _IO_Kernel_Task__H */