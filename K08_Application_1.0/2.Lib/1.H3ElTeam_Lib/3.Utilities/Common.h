/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Common.h
* Author             : Hai Dang
* Date First Issued  : 20-Sep-2014
* Description        : include global function .

********************************************************************************
* History: DD/MM/YY
* 
* 20/09/2014: V0.2
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H
    
/* Include main global file */
#include "Global_Custom_Define.h"

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include OS layer */
#ifdef USE_OS
	#include "OS_Layer.h"
#endif

/* Include project lib */
#include "PlatFormType.h"
#include "Utilities.h"
#include "Dataframe.h"

/* Include Hardware profile */
#include "Hardware_Function_Profile.h"
#include "PointerFunctionDefine.h"
#include "Project_Function.h"

/* Include kernel for MCU */
#include "IO_Function.h"
#include "Time_Manage_Function.h"
#include "USART1_AppCall_Function.h"
//#include "USART2_AppCall_Function.h"
#include "Watchdog_Function.h"

#endif /* __COMMON_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
