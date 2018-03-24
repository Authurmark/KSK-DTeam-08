/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : OS_Layer.h
* Author             : Hai Dang
* Date First Issued  : 20-Sep-2014
* Description        : include OS function define.

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
/* Define function for OS choosen */
#ifdef USE_OS
	/* Include RTOS lib base on the OS to run */
	#ifdef USE_FREERTOS
		/* Include function file */
		#include "FreeRTOS.h"
		#include "task.h"
		#include "queue.h"
		/* Define function and variable for FreeRTOS OS */
		#define OS_xTaskCreate 		xTaskCreate
		#define OS_xTaskSuspend		vTaskSuspend
		#define OS_xTaskResume		vTaskResume
		#define OS_xTaskDelete		vTaskDelete
		#define OS_vTaskScheduler 	vTaskStartScheduler
		#define OS_xQueueCreate		xQueueCreate
		#define OS_vTaskDelay		vTaskDelay
		#define OS_vTaskDelayUntil	vTaskDelayUntil
	#endif
#endif