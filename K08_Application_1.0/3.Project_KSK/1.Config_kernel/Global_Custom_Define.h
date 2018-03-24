/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Global_Custom_Define.h
* Author             : Hai Dang
* Date First Issued  : 20-Sep-2014
* Description        : Define for global use for project

********************************************************************************
* History: DD/MM/YY
* 20/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Global_Custom_Define__H
#define _Global_Custom_Define__H

/* Define for use or not use OS to control system */
/* Uncomment to Activate, comment to deactivate */
#define USE_OS
	#define USE_FREERTOS

/* Define MCU for USE */
/* Uncomment to Activate, comment to deactivate */
#define USE_ST_MCU

/* Define Lib use */
/* Uncomment to Activate, comment to deactivate */
#define USE_CMCIS_DRIVER
//#define USE_ONLY_REGISTER

/* Define type MCU for USE */
/* Uncomment to Activate, comment to deactivate */
#ifdef USE_ST_MCU
	//#define USE_ST_M3_LD
	#define USE_ST_M3_MD
	//#define USE_ST_M3_HD
#endif

/* Define for USE_ST_M3_HD */
#ifdef USE_ST_M3_HD
	#define STM32F10X_HD
	/* Define when use USE_CMCIS_DRIVER */
	#ifdef USE_CMCIS_DRIVER
		#define USE_STDPERIPH_DRIVER
		#define VECT_TAB_FLASH
		#define IAR_ARM_CM3
	#endif

	/* Include register header file */
	#include "stm32f10x.h"

#endif
#ifdef USE_ST_M3_MD
	#define STM32F10X_MD
	/* Define when use USE_CMCIS_DRIVER */
	#ifdef USE_CMCIS_DRIVER
		#define USE_STDPERIPH_DRIVER
		#define VECT_TAB_FLASH
		#define IAR_ARM_CM3
	#endif

	/* Include register header file */
	#include "stm32f10x.h"

#endif
#endif/*_Global_Custom_Define__H*/

