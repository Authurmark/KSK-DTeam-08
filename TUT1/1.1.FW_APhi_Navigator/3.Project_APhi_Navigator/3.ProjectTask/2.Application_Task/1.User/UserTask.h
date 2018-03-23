/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UserTask.c
* Author             : OS Team
* Date First Issued  : 16/8/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Project includes. */
#include "Common.h"
/*-----------------------------------------------------------*/
typedef enum
{ 
    eUserTask_State_Idle				= 1,
    eUserTask_State_CheckButton 		= 2,
	eUserTask_State_CheckPIR 			= 3,
	eUserTask_State_CheckRelayState 	= 4,
    eMainTask_State_MAIN,
    eMainNullState=0xff,
}eUserState;
/* Funtion Prototype */
void vUserTask( void *pvParameters );