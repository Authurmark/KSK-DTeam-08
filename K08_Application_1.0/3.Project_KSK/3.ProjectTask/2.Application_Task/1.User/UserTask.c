/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UserTask.c
* Author             : OS Team
* Date First Issued  : 16/8/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 20/09/2014: v0.2
* 22/12/2017: v1.0 	- 1st build for a Phi Navigator
/********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Project includes. */
#include "UserTask.h"
#include "Project_Function.h"
#include "Time_Manage_Function.h"
#include "IO_Kernel_Function.h"
#include "ComFunction.h"

/* Define */
#define USER_TASK_FREQUENCY 10
/* Global Variable */
extern Struct_User_Information StrUserInfo;
extern char StrConfig[FLASH_PAGE_SIZE];
extern Struct_Flash_Config_Parameter	StrConfigPara;
extern enumbool xFlag_User_Task_Still_Running;
extern Struct_System_Information 		StrSystemInfo;
extern xTaskHandle xRF_Task_Handle, xSensor_Task_Handle, xIO_Task_Handle, xSensor_IO_Task_Handle;
extern enumbool xFlag_User_Task_Still_Running, xFlag_User_Task_Init_Done, xFlag_User_Task_Process_Check;

/* State of User Task */
typedef enum
{
	eST_User_Task_INIT						= 1,
	eST_User_Task_IDLE 						= 2,
	eST_User_Task_LOGGING					= 3,
	eST_User_Task_ERROR						= 4,
	eST_User_Task_CHECKING_EVENT			= 5,
	eST_User_Task_PC_CONNECT		        = 6,
    eST_User_Task_PWM                       = 7,
    eST_User_Task_DMA_ADC                   = 8,
		
	eST_User_Task_UN 						= 0xff,
}eST_User_Task;


/* Local Variable */
void vUserTaskMainProcess(void);
enumbool bFlag_1st_Case = eFALSE;
eST_User_Task eState_User_Task;

//#define USER_TASK_DEBUG
#ifdef USER_TASK_DEBUG 
	#define DEBUG_USERTASK printf
#else
	#define DEBUG_USERTASK
#endif

//#define USER_TASK_RFDEBUG
#ifdef USER_TASK_RFDEBUG 
	#define DEBUG_RFUSERTASK printf
#else
	#define DEBUG_RFUSERTASK
#endif

/* Code for show */
#define DEMO_CODE

/*-----------------------------------------------------------*/
void vUserTask( void *pvParameters )
{
	/* Delay before begin task */
	OS_vTaskDelay(50);
        /* Set flag */
	bFlag_1st_Case = eTRUE;
	/* Set prequency */
	portTickType xLastWakeTime;
	const portTickType xUser_Task_Frequency = USER_TASK_FREQUENCY;/* 10 tick slice */
	xLastWakeTime = xTaskGetTickCount();
	
	/* Init ok */
	xFlag_User_Task_Init_Done= eTRUE;
        
	
      /* Task process */
      for(;;)
      {	
          /* Delay Exactly Frequency */
          OS_vTaskDelayUntil(&xLastWakeTime,xUser_Task_Frequency);

          /* Set xFlag_User_Task_Still_Running */
          xFlag_User_Task_Still_Running = eTRUE;
          /* Process User Task */
          vUserTaskMainProcess();
      }
}
/*********************************************************************/

uint32_t		ixIndex_ADC_Buffer;
uint32_t	ADC_Buffer[10];
static uint32_t sum_ADC = 0;
static uint32_t value_ADC_tb = 0;

void vUserTaskMainProcess(void)
{
	switch(eState_User_Task)
	{
		case eST_User_Task_INIT:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
                eState_User_Task = eST_User_Task_IDLE;
                bFlag_1st_Case = eTRUE;
			}
		break;
		case eST_User_Task_IDLE:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
                        }
		break;
		case eST_User_Task_LOGGING:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				
			}
		break;
		case eST_User_Task_ERROR:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				
			}
		break;
		case eST_User_Task_CHECKING_EVENT:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				eState_User_Task = eST_User_Task_IDLE;
				bFlag_1st_Case = eTRUE;
			}
		break;
		case eST_User_Task_PC_CONNECT:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				
			}
		break;
        case eST_User_Task_DMA_ADC:
             if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
            else
			{
				/* Check Comport process */
                /* Check config UART LED parameter */
                if(bFlagGetCommandLEDConfigUART1==eTRUE)
                {
                    /* Clear flag */
                    bFlagGetCommandLEDConfigUART1 = eFALSE;
                    /* Set parameter to IO task */
                    vIO_ConfigOutput(&OUT_LED_SIGNAL,bLEDConfigCommand.uFrequency,bLEDConfigCommand.uFrequency*bLEDConfigCommand.uCountToggle,bLEDConfigCommand.uCountToggle,bLEDConfigCommand.bStartState,bLEDConfigCommand.bEndState,bLEDConfigCommand.bFlagStart);
//                    vIO_ConfigOutput(&OUT_LED_SIGNAL,50,100,2,RELAY_ON,RELAY_OFF,eFALSE);                 
                }
				
				
				/* Test ADC to PWM function */
				static uint32_t iIndex;
				sum_ADC = 0;
				ixIndex_ADC_Buffer = ixIndex_ADC_Buffer+1;

				if(ixIndex_ADC_Buffer>=10)		ixIndex_ADC_Buffer=0;
				ADC_Buffer[ixIndex_ADC_Buffer] = ADCConvertedValue;
				
				sum_ADC = 0;	
                for (iIndex=0;iIndex<10;iIndex++)
                {
                  sum_ADC = sum_ADC+ ADC_Buffer[iIndex];
                }
         
                value_ADC_tb = sum_ADC/10;
			    MOTOR_1_DUTY(value_ADC_tb/41);
                   
                                      
	            }
		break;
		default:
			eState_User_Task = eST_User_Task_DMA_ADC;
			bFlag_1st_Case = eTRUE;
		break;
	}
}
