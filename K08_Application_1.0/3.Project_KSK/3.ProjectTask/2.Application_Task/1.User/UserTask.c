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
        eST_User_Task_INIT			= 1,
        eST_User_Task_IDLE 			= 2,
        eST_User_Task_LOGGING			= 3,
        eST_User_Task_ERROR			= 4,    
        eST_User_Task_CHECKING_EVENT            = 5,
        eST_User_Task_PC_CONNECT		= 6,
        eST_User_Task_PWM                       = 7,
        eST_User_Task_DMA_ADC                   = 8,
        eST_User_Task_Encoder                   = 9,
        eST_User_Task_ResetHome			= 10,
        eST_User_Task_ReleaseCutter 	        = 11,
        eST_User_Task_GetCutter			= 12,
        eST_User_Task_RunForScan		= 13,
        eST_User_Task_RunToPoint		= 14,
        eST_User_Task_Stop   			= 15,
        eST_User_Task_Pause			= 16,
		
		
		
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
enumbool  bFlag_Update = eFALSE; 
enumbool bFlag_Finish =  eFALSE;
extern uint32 bFlag_Status_Axis;

void vUserTaskMainProcess(void)
{
	switch(eState_User_Task)
	{
		case eST_User_Task_INIT:
			if(bFlag_1st_Case == eTRUE)
			{
				bFlag_1st_Case = eFALSE;
                                State_ReleaseCutter                     = 0;
                                State_GetCutter                         = 0;
                                bFlag_Scanhold_Finish                   = eFALSE;
                                State_RunToPoint                        = 0;
                                bFlag_Finish                            = eFALSE;
			}
			else
			{
                                eState_User_Task = eST_User_Task_IDLE;
			}
		break;
		case eST_User_Task_IDLE:
                            if (BUFFER_AXIS_PROCESS.bFlag_Process_Update == eTRUE)
                            {   
                                switch(BUFFER_AXIS_PROCESS.bProcess_Axis)
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

					case RunToPoint:
                                              eState_User_Task = eST_User_Task_RunToPoint;
                                              bFlag_1st_Case = eTRUE;
                                        break;

					default:
					break;
				}
                           }
                        
		 break;
                
                
                
                
                
		case eST_User_Task_ResetHome:
			if(bFlag_1st_Case == eTRUE)
			{
			 	  	bFlag_1st_Case = eFALSE;
				  	bFlag_Error_Process = eFALSE;
                                        bStepMotor = MOTOR_STEP_STOP;
                                        BUFFER_AXIS_PROCESS.bFlag_Process_Update                = eFALSE;
                                   
			}
			else
			{
				if (BUFFER_STATEBUTTON.bflag_Stop == 1 )
				{
					eState_User_Task = eST_User_Task_Stop;
					ControlMotor_Button();  
				}
				else
				{
					switch(BUFFER_STATEBUTTON.bflag_Pause)
					{
					case 0:
						  /*Z to Home*/
						  Z_HOME();

						  /*X to Home*/
                                                  
						  X_HOME();

						  /*Y to Home*/
                                                  
						  Y_HOME();

						  /*Condition Process Finished*/
						  if(bFlag_Status_Axis == 12)	
                                                    bFlag_Finish = eTRUE;


						  /*Back to eST_User_Task_IDLE, when finished process*/
						  if (bFlag_Finish == eTRUE) 
						  {   
                                                     BUFFER_AXIS_PROCESS.bFeedBackAxis = eAxis_Finsish_Resethome;
                                                     BUFFER_AXIS_PROCESS.bFlag_Process_Info_FeedBack = eTRUE;
                                                     eState_User_Task = eST_User_Task_IDLE;
						  }
						  if(bFlag_Error_Process == eTRUE) eState_User_Task = eST_User_Task_IDLE;
					break;
					case 1:
					
					break;
					default:
					break;
					}
				}
            }
		break;

		case eST_User_Task_ReleaseCutter:
			if(bFlag_1st_Case==eTRUE)
			{
			 	 bFlag_1st_Case = eFALSE;
                                 bFlag_Error_Process 	                                = eFALSE;
                                 Status_StepMotor                                       = MOTOR_STEP_STOP;
                                 BUFFER_AXIS_PROCESS.bFlag_Process_Update               = eFALSE;
//                                 BUFFER_CONTROL_X_AXIS.bFlag_Process_Update_data        = eTRUE;
//                                 BUFFER_CONTROL_Y_AXIS.bFlag_Process_Update_data        = eTRUE;
//                                 BUFFER_CONTROL_Z_AXIS.bFlag_Process_Update_data        = eTRUE;
                                 i                                                      = 0;
			}
			else
			{
				if(BUFFER_STATEBUTTON.bflag_Stop == 1)
				{
					eState_User_Task = eST_User_Task_Stop;
					ControlMotor_Button();
				}
				else
				{
                                  switch(BUFFER_STATEBUTTON.bflag_Pause)
					{
					case 0:
//                                                  vGetEncoderValue_X();
//                                                  vGetEncoderValue_Y();
						  ReleaseCutter_Machine();
                                                   vInit_Error_Process();
						  //Back to eST_User_Task_IDLE, when finished process
						  if(State_ReleaseCutter == 7)     
                                                  {
                                                     eState_User_Task = eST_User_Task_IDLE;
                                                     BUFFER_AXIS_PROCESS.bFeedBackAxis = eAxis_Finsish_Releasecutter;
                                                     BUFFER_AXIS_PROCESS.bFlag_Process_Info_FeedBack = eTRUE;
                                                  }
						  if(bFlag_Error_Process == eTRUE)  	        eState_User_Task=eST_User_Task_IDLE;		
					break;
					case 1:
					break;
					default:
					break;
					}
                                }
			}
		break;
                case eST_User_Task_GetCutter:
			if(bFlag_1st_Case==eTRUE)
			{
			 	 bFlag_1st_Case = eFALSE;
				 bFlag_Error_Process 	                                = eFALSE;
                                 Status_StepMotor                                       = MOTOR_STEP_STOP;
                                 BUFFER_AXIS_PROCESS.bFlag_Process_Update               = eFALSE;
//                                 BUFFER_CONTROL_X_AXIS.bFlag_Process_Update_data        = eTRUE;
//                                 BUFFER_CONTROL_Y_AXIS.bFlag_Process_Update_data        = eTRUE;
//                                 BUFFER_CONTROL_Z_AXIS.bFlag_Process_Update_data        = eTRUE;
                                 i=0;
			}
			else
			{
				if (BUFFER_STATEBUTTON.bflag_Stop == 1)
				{
					eState_User_Task = eST_User_Task_Stop;
					ControlMotor_Button();
				}
				else
				{
					switch(BUFFER_STATEBUTTON.bflag_Pause)
					{
					case 0:
						  GetCutter_Machine();
//                                                  vGetEncoderValue_X();
//                                                  vGetEncoderValue_Y();
                                                  vInit_Error_Process();
						  //Back to eST_User_Task_IDLE, when finished process
						   if(State_GetCutter == 7)        
                                                   {
                                                     eState_User_Task = eST_User_Task_IDLE;	
                                                     BUFFER_AXIS_PROCESS.bFeedBackAxis = eAxis_Finsish_Getcutter;
                                                     BUFFER_AXIS_PROCESS.bFlag_Process_Info_FeedBack = eTRUE;
                                                   }
						   if(bFlag_Error_Process == eTRUE) 		eState_User_Task=eST_User_Task_IDLE;	
					break;
					case 1:
						  
					break;
					default:
					break;
					}
                                }
			}
   		break;
		case eST_User_Task_RunForScan:
                      if(bFlag_1st_Case==eTRUE)
			{
			 	  bFlag_1st_Case                                        = eFALSE;
 				  State_ScanHole_Y 		                        = 0;
                                  State_ScanHole_X 		                        = 0;
				  bFlag_Error_Process 	                                = eFALSE;
                                  Status_StepMotor                                      = MOTOR_STEP_STOP;
                                  BUFFER_AXIS_PROCESS.bFlag_Process_Update              = eFALSE;
                                  bFlag_Finish                                          = eFALSE;
//                                  BUFFER_CONTROL_X_AXIS.bFlag_Process_Update_data       = eTRUE;
//                                  BUFFER_CONTROL_Y_AXIS.bFlag_Process_Update_data       = eTRUE;
//                                  BUFFER_CONTROL_Z_AXIS.bFlag_Process_Update_data       = eTRUE;
                                  i=0;
			}
                      else
			{
				if(BUFFER_STATEBUTTON.bflag_Stop == 1)
				{
					eState_User_Task = eST_User_Task_Stop;
					ControlMotor_Button();
				}
				else
				{
					switch (BUFFER_STATEBUTTON.bflag_Pause)
					{
					case 0:
						  ScanHole();
//                                                  vGetEncoderValue_X();
//                                                  vGetEncoderValue_Y();
                                                  vInit_Error_Process();
						  //Back to eST_User_Task_IDLE, when finished process
						  if (bFlag_Scanhold_Finish == eTRUE)	        
                                                  {
                                                    eState_User_Task = eST_User_Task_IDLE;
                                                    BUFFER_AXIS_PROCESS.bFeedBackAxis = eAxis_Finsish_Scanhole;
                                                    BUFFER_AXIS_PROCESS.bFlag_Process_Info_FeedBack = eTRUE;

                                                  }
						  if(bFlag_Error_Process == eTRUE) 		eState_User_Task = eST_User_Task_IDLE;
					break;
					case 1:

					break;
					default:
					break;
					}
			     }
			}
		break;
		case eST_User_Task_RunToPoint:
                        if(bFlag_1st_Case==eTRUE)
			{
			 	  bFlag_1st_Case  		                        = eFALSE;
				  bFlag_Error_Process 	                                = eFALSE;
                                  Status_StepMotor                                      = MOTOR_STEP_STOP;
                                  BUFFER_AXIS_PROCESS.bFlag_Process_Update              = eFALSE;
                                  bFlag_Finish                                          = eFALSE;
//                                  BUFFER_CONTROL_X_AXIS.bFlag_Process_Update_data       = eTRUE;
//                                  BUFFER_CONTROL_Y_AXIS.bFlag_Process_Update_data       = eTRUE;
//                                  BUFFER_CONTROL_Z_AXIS.bFlag_Process_Update_data       = eTRUE;
                                  i=0;
			}
			else
			{
				if(BUFFER_STATEBUTTON.bflag_Stop == 1 )
				{
					eState_User_Task = eST_User_Task_Stop;
					ControlMotor_Button();
				}
				else
				{
					switch(BUFFER_STATEBUTTON.bflag_Pause)
					{
					case 0:
						  Run_To_Point();
//                                                  vGetEncoderValue_X();
//                                                  vGetEncoderValue_Y();
                                                  vInit_Error_Process();
						  //Back to eST_User_Task_IDLE, when finished process
						  if (State_RunToPoint == 5)
                                                  {
                                                    eState_User_Task = eST_User_Task_IDLE;
                                                    BUFFER_AXIS_PROCESS.bFeedBackAxis = eAxis_Finsish_Runtopoint;
                                                    BUFFER_AXIS_PROCESS.bFlag_Process_Info_FeedBack = eTRUE;
                                                  }
						  if (bFlag_Error_Process == eTRUE) 		 eState_User_Task = eST_User_Task_IDLE;

					break;
					case 1:

					break;
					default:
					break;
					}
		  	     }
			 }
                break;
                case eST_User_Task_Stop:
			memset(UART1_BUFFER_RX,0,i_MAX_UART);  
			eState_User_Task                                                = eST_User_Task_IDLE;
			BUFFER_STATEBUTTON.bflag_Stop                                   = 0;
                        bFlag_1st_Case                                                  = eTRUE;
                        BUFFER_CONTROL_X_AXIS.bFlag_Process_Update_data                 = eFALSE;
                        BUFFER_CONTROL_Y_AXIS.bFlag_Process_Update_data                 = eFALSE;
                        BUFFER_CONTROL_Z_AXIS.bFlag_Process_Update_data                 = eFALSE;
		break;

		case eST_User_Task_Pause:

		break;
		default:
			eState_User_Task = eST_User_Task_INIT;
			bFlag_1st_Case = eTRUE;
		break; 
}
}


