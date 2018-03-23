/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Hardware_Function_Profile.h
* Author             : OS Team
* Date First Issued  : 13/11/2013
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 13/11/2013: V0.1
* 20/19/2014: V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Hardware_Function_Profile__H
#define _Hardware_Function_Profile__H
/* Include global header file */
#include "Common.h"

/* Include some header file useful function */
#include "ringbuf.h"

/************************************************************************************************************/
/* Define IO Function*/
/* Uncomment to use IO, comment to disable*/
#ifdef USE_IO_FUNCTION  
	/* Include header for IO function */
	#include "IO_Function_Profile.h"
#endif
/* Define Timer Function*/
#ifdef USE_TIMER_BASE_FUNCTION
	/* Include header for Timer function */
	#include "Timer_Function_Profile.h"
#endif
/* Define USART1 Function*/
#ifdef USE_USART1
	/* Include header for USART1 function */
	#include "USART1_Function_Profile.h"
#endif
/* Define USART2 Function*/
#ifdef USE_USART2
	/* Include header for USART1 function */
	#include "USART2_Function_Profile.h"
#endif
/* Define USART3 Function*/
#ifdef USE_USART3
	/* Include header for USART1 function */
	#include "USART3_Function_Profile.h"
#endif
/* Define USART4 Function*/
#ifdef USE_USART4
	/* Include header for USART1 function */
	#include "USART4_Function_Profile.h"
#endif
/* Define SPI1 Function*/
#ifdef USE_SPI1
	/* Include header for SPI1 function */
	#include "SPI1_Function_Profile.h"
#endif
/* Define SPI2 Function*/
#ifdef USE_SPI2
/* Include header for SPI2 function */
	#include "SPI2_Function_Profile.h"
#endif
/* Define RTC Function*/
#ifdef USE_RTC
	/* Include header for RTC function */
	#include "RTC_Function_Profile.h"
#endif
/************************************************************************************************************/
/************************************************************************************************************/
/* Define for SI4432_Project */
#define APhi_Navigator
#ifdef APhi_Navigator
	/* Comment to disable module, uncomment to enable */
	#define 	GPS

	/* Profile for use with project */
	#ifdef GPS
		
	#endif /* GPS */
#endif /* APhi_Navigator */
#endif	/*_Hardware_Function_Profile__H */