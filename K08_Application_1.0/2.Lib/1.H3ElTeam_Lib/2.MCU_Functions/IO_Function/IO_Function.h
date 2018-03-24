/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : IO_Function.h
* Author             : Hai Dang
* Date First Issued  : 20-Sep-2014
* Description        : global function struct to manage and pointer function.

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
#ifndef	_IO_Function__H
#define _IO_Function__H
/* Include Header file */
#include "Common.h"
#include "IO_Function_Profile.h"
/* Init LED function */
#ifdef USE_LED
	#ifdef USE_LED_1
	/* Init function */
	void IO_LED1_Init(IO_Struct *pIO);
	/* Read function */
	enumbool IO_LED1_Read(void);
	/* write function */
	void IO_LED1_Write(enumbool BitVal);
	/* Read write status function */
	enumbool IO_LED1_WriteStatus(void);
	#endif
	#ifdef USE_LED_2
	/* Init function */
	void IO_LED2_Init(IO_Struct *pIO);
	/* Read function */
	enumbool IO_LED2_Read(void);
	/* write function */
	void IO_LED2_Write(enumbool BitVal);
	/* Read write status function */
	enumbool IO_LED2_WriteStatus(void);
	#endif
	#ifdef USE_LED_3
	/* Init function */
	void IO_LED3_Init(IO_Struct *pIO);
	/* Read function */
	enumbool IO_LED3_Read(void);
	/* write function */
	void IO_LED3_Write(enumbool BitVal);
	/* Read write status function */
	enumbool IO_LED3_WriteStatus(void);
	#endif
	#ifdef USE_LED_4
	/* Init function */
	void IO_LED4_Init(IO_Struct *pIO);
	/* Read function */
	enumbool IO_LED4_Read(void);
	/* write function */
	void IO_LED4_Write(enumbool BitVal);
	/* Read write status function */
	enumbool IO_LED4_WriteStatus(void);
	#endif
#endif
#ifdef USE_RELAY
	#ifdef USE_RELAY_1
	void IO_RELAY_1_Init(IO_Struct *pIO);
	void IO_RELAY_1_Write(enumbool BitVal);
	enumbool IO_RELAY_1_WriteStatus(void);
	#endif
	#ifdef USE_RELAY_1_FB
	void IO_RELAY_1_FB_Init(IO_Struct *pIO);
	enumbool IO_RELAY_1_FB_Read(void);
	#endif
	#ifdef USE_RELAY_2
	void IO_RELAY_2_Init(IO_Struct *pIO);
	void IO_RELAY_2_Write(enumbool BitVal);
	enumbool IO_RELAY_2_WriteStatus(void);
	#endif
	#ifdef USE_RELAY_3
	void IO_RELAY_3_Init(IO_Struct *pIO);
	void IO_RELAY_3_Write(enumbool BitVal);
	enumbool IO_RELAY_3_WriteStatus(void);
	#endif
#endif
#ifdef USE_BUTTON_IO
	#ifdef USE_BUTTON_IO_1
	void IO_BUTTON_1_Init(IO_Struct *pIO);
	enumbool IO_BUTTON_1_Read(void);
	#endif
	#ifdef USE_BUTTON_IO_2
	void IO_BUTTON_2_Init(IO_Struct *pIO);
	enumbool IO_BUTTON_2_Read(void);
	#endif
	#ifdef USE_BUTTON_IO_3
	void IO_BUTTON_3_Init(IO_Struct *pIO);
	enumbool IO_BUTTON_3_Read(void);
	#endif
	#ifdef USE_BUTTON_IO_4
	void IO_BUTTON_4_Init(IO_Struct *pIO);
	enumbool IO_BUTTON_4_Read(void);
	#endif
#endif
#endif