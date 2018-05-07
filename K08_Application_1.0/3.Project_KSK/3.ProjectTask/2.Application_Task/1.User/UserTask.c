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
#include "Timer_Function.h"		

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
extern  uint32_t rotary_cntr;

/* State of User Task */
typedef enum
{
        eST_User_Task_INIT				= 1,
        eST_User_Task_IDLE 				= 2,
        eST_User_Task_LOGGING			= 3,
        eST_User_Task_ERROR				= 4,
        eST_User_Task_CHECKING_EVENT    = 5,
        eST_User_Task_PC_CONNECT		= 6,
        eST_User_Task_PWM               = 7,
        eST_User_Task_DMA_ADC           = 8,
        eST_User_Task_Encoder           = 9,
		eST_User_Task_ResetHome			= 10,
		eST_User_Task_ReleaseCutter 	= 11,
		eST_User_Task_GetCutter			= 12,
		eST_User_Task_RunForScan		= 13,
		eST_User_Task_RunToPoint		= 14,
		eST_User_Task_Stop   			= 15,
		
		
		
        eST_User_Task_UN 			= 0xff,
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

uint32_t	ixIndex_ADC_Buffer;
uint32_t	ADC_Buffer[10];
static uint32_t sum_ADC = 0;
static uint32_t value_ADC_tb = 0;
enumbool  bFlag_Update = eFALSE; 
enumbool bFlag_Finish =  eFALSE;
extern uint32 bFlag_Status_Axis;

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
                BUFFER_MOTOR_CONTROL_PROCESS.bFlag_Process_Update = eTRUE;
              if (BUFFER_MOTOR_CONTROL_PROCESS.bFlag_Process_Update = eTRUE)
			   {
				BUFFER_MOTOR_CONTROL_PROCESS.bProcess_Axis = RunForScan;
            	switch(BUFFER_MOTOR_CONTROL_PROCESS.bProcess_Axis)
				{
					case ResetHome:
					  eState_User_Task = eST_User_Task_ResetHome;
					  bFlag_1st_Case = eTRUE;
					break;
					case ReleaseCutter:
					  eState_User_Task = eST_User_Task_ReleaseCutter;
					  bFlag_1st_Case = eTRUE;
					break;

					case GetCutter:
					  eState_User_Task = eST_User_Task_GetCutter;
					  bFlag_1st_Case = eTRUE;
					break;
  
					case RunForScan:
					  eState_User_Task = eST_User_Task_RunForScan; 
					  bFlag_1st_Case = eTRUE; 
					break;

					case RunToPoint :
                      eState_User_Task = eST_User_Task_RunToPoint;
					  bFlag_1st_Case = eTRUE;
                    break;

					case Stop:
                      eState_User_Task = eST_User_Task_Stop;
					break;

					default :
					break;
				}
			   }
           }
		 break;
                
                
                
                
                
		case eST_User_Task_ResetHome:
			if(bFlag_1st_Case==eTRUE)
			{
			 	  bFlag_1st_Case = eFALSE;
                  BUFFER_MOTOR_CONTROL_PROCESS.bFlag_Process_Update = eFALSE;
				  bFlag_Status_Axis = 0;
				  bFlag_Error_Process = eFALSE;
			}
			else
			{
				
				/*X to Home*/
				   	Z_HOME();
					
			    /*Y to Home*/
			       X_HOME();
				
           		/*Z to Home*/
			       Y_HOME();
                 			  
				/*Condition Process Finished*/
				  if(bFlag_Status_Axis == 9)	bFlag_Finish == eTRUE;

      
				/*Back to eST_User_Task_IDLE, when finished process*/
			      if (bFlag_Finish== eTRUE)    eState_User_Task=eST_User_Task_IDLE;
				  if(bFlag_Error_Process == eTRUE) eState_User_Task=eST_User_Task_IDLE;
              
				//Back to eST_User_Task_IDLE, when reset
//				if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
//				  eState_User_Task=eST_User_Task_INIT;
            }
		break;

		case eST_User_Task_ReleaseCutter:
			if(bFlag_1st_Case==eTRUE)
			{
			 	 bFlag_1st_Case = eFALSE;
 				 BUFFER_MOTOR_CONTROL_PROCESS.bFlag_Process_Update = eFALSE;
				 bFlag_ReleaseCutter 	= 0;	
                 bFlag_GoHOME_X 		= eTRUE;
				 bFlag_GoHOME_Y 		= eTRUE;
				 bFlag_GoHOME_Z 		= eTRUE;
                 bFlag_Error_Process 	= eFALSE;
			}
			else
			{
			      vInitReleaseCutter();
 
				//Back to eST_User_Task_IDLE, when finished process
				 if(bFlag_ReleaseCutter == 8)              	eState_User_Task = eST_User_Task_IDLE;
				 if(bFlag_Error_Process == eTRUE)  			eState_User_Task=eST_User_Task_IDLE;		
				//Back to eST_User_Task_IDLE, when reset
//				if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
//				 eState_User_Task=eST_User_Task_INIT;
            }
		break;
        case eST_User_Task_GetCutter:
			if(bFlag_1st_Case==eTRUE)
			{
			 	 bFlag_1st_Case = eFALSE;
 				 BUFFER_MOTOR_CONTROL_PROCESS.bFlag_Process_Update = eFALSE;
				 bFlag_GetCutter 	  	= 0;	
                 bFlag_GoHOME_X 		= eTRUE;
				 bFlag_GoHOME_Y 		= eTRUE;
				 bFlag_GoHOME_Z 		= eTRUE;
				 bFlag_Error_Process 	= eFALSE;
			}
			else
			{
				vInitGetCutter();
				//Back to eST_User_Task_IDLE, when finished process
				 if(bFlag_GetCutter == 8)              	eState_User_Task = eST_User_Task_IDLE;	
				 if(bFlag_Error_Process == eTRUE) 		eState_User_Task=eST_User_Task_IDLE;	
				//Back to eST_User_Task_IDLE, when reset
//				if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
//				 eState_User_Task=eST_User_Task_INIT;
			}
   		break;
		case eST_User_Task_RunForScan:
                   if(bFlag_1st_Case==eTRUE)
			{
			 	  bFlag_1st_Case = eFALSE;
 				  bFlag_ScanHole_Y 		= 0;
                  bFlag_ScanHole_X 		= 0;
				  bFlag_GoHOME_X   		= eTRUE;
				  bFlag_GoHOME_Y 		= eTRUE;
				  bFlag_GoHOME_Z 		= eTRUE;
				  bFlag_Error_Process 	= eFALSE;
			}
			else
			{
				ScanHole(); 
		   		//Back to eST_User_Task_IDLE, when finished process
				if (bFlag_ScanHole_X == 11)				eState_User_Task=eST_User_Task_IDLE;
				if(bFlag_Error_Process == eTRUE) 		eState_User_Task=eST_User_Task_IDLE;
				//Back to eST_User_Task_IDLE, when reset
//				if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
//				  eState_User_Task=eST_User_Task_INIT;
			}
		break;
		break;
		case eST_User_Task_RunToPoint:
		  if(bFlag_1st_Case==eTRUE)
			{
			 	  bFlag_1st_Case  		= eFALSE;
 				  bFlag_RunToPoint 		= 0;
				  bFlag_GoHOME_X 		= eTRUE;
				  bFlag_GoHOME_Y 		= eTRUE;
				  bFlag_GoHOME_Z 	 	= eTRUE;
				  bFlag_Error_Process 	= eFALSE;
			}
			else
			{
			
                  Run_To_Point();
				//Back to eST_User_Task_IDLE, when finished process
				if (bFlag_RunToPoint == 4)				 eState_User_Task=eST_User_Task_IDLE;
				if(bFlag_Error_Process == eTRUE) 		 eState_User_Task=eST_User_Task_IDLE;
				//Back to eST_User_Task_IDLE, when reset
//				if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
//				  eState_User_Task=eST_User_Task_INIT;
		  	}

        case eST_User_Task_Stop:
         
		break;
                
                //<<<<<<<<<-------------------SYSTEM WORKS---------------------->>>>>>>>>//
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
		default:
			eState_User_Task = eST_User_Task_IDLE;
			bFlag_1st_Case = eTRUE;
		break; 
}
}


