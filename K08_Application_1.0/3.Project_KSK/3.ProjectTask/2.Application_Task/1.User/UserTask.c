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
        eST_User_Task_INIT						= 1,
        eST_User_Task_IDLE 						= 2,
        eST_User_Task_LOGGING					= 3,
        eST_User_Task_ERROR						= 4,
        eST_User_Task_CHECKING_EVENT			= 5,
        eST_User_Task_PC_CONNECT				= 6,
        eST_User_Task_PWM                       = 7,
        eST_User_Task_DMA_ADC                   = 8,
        eST_User_Task_Encoder                   = 9,
		eST_User_Task_StopButton                = 10,
	
		
        eST_User_Task_UN 						= 0xff,
        
        eST_User_Task_ResetHome                 =0x11,
        eST_User_Task_ResetCutter               =0x12,
        eST_User_Task_ScanHole                  =0x13,
        eST_User_Task_ThreadChecker             =0x14,
        
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


#define USE_DEBUG_MASTER




/* Control Process */
enumbool eFlag_Process_Finish;

#define sReleaseCutter          1
#define sGetCutter              2
#define sChangeCutterFinish     3

enumbool bChangeCutter = eFALSE;
uint8 sChangeCutter;

#define Status_ChangeCutter             1
#define Status_RuntoHole                2
#define Status_CheckThread              3
#define Status_FinishThreadProcess      4

enumbool bFlag_1st_Go   =eFALSE;
enumbool bFlag_TimeSet  =eFALSE;
uint8 Status_CheckThreadProcess;




/* Control Spindle Check Thread Hole */
#define sSpindle_GetReady                1
#define sSpindle_Z_GoDown                2
#define sSpindle_Spindle_GoDown          3
#define sSpindle_Checking                4
#define sSpindle_Hole_GoOut              5
#define sSpindle_Spindle_GetReady        6
#define sSpindle_Spindle_GoUp            7
#define sSpindle_Z_GoUp                  8
#define sSpindle_Finish                  9

#define Time_SpindleUpInHole             700            //ms
timer tIO_SpindleUpInHole;
enumbool bFlag_FinishCheckHole  = eFALSE;
uint8    Status_SpindleCheckProcess =0;     


       





/*-----------------------------------------------------------*/
/*----------------------USER TASK FUNCTION-------------------*/
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



void vUserTaskMainProcess(void)
{
	switch(eState_User_Task)
	{
		case eST_User_Task_INIT:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
                                
				BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine = PMachine_Reset;
				BUFFER_MACHINE_CONTROL.eFlag_Process_Update = eFALSE;
				eFlag_Process_Finish = eFALSE;
				BUFFER_STATEBUTTON.bflag_Stop = 0;
				
				//Get Ready for System
				vInitCutter();
				//Get Ready for Axis Control
				
				//Get Ready for Spindle Control
				vSetAirVale(SpindleResetHome);
				
				BUFFER_CONTROL_DC_SPINDLE.bProcess = SPINDLE_STOP;
				BUFFER_CONTROL_DC_SPINDLE.Flag_Update = eTRUE;
 
			}
			else
			{
				BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine = PMachine_Ready;
				eState_User_Task = eST_User_Task_IDLE;
				bFlag_1st_Case = eTRUE;
								
			}
		break;
                
                
                
                
                
                
                
                
                
                //whenever process finish, system comeback to Idle
		case eST_User_Task_IDLE:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE; 
				Control_LED_SYSTEM(SYS_READY);
			}
			else
			{
				//when process finish and PC recieve feedback status process, system comback to Idle
				//PC send new command, and BUFFER_MACHINE_CONTROL.eFlag_Process_Update = eTRUE;
				//In this time, we will handle the system

//				BUFFER_MACHINE_CONTROL.eFlag_Process_Update=eTRUE;
				if(BUFFER_MACHINE_CONTROL.eFlag_Process_Update==eTRUE)
				{
//					BUFFER_MACHINE_CONTROL.bProcess_Control_Machine = PMachine_ResetHome;
					switch(BUFFER_MACHINE_CONTROL.bProcess_Control_Machine)
					{
						case PMachine_ResetHome:
						  eState_User_Task=eST_User_Task_ResetHome;
						  eFlag_Process_Finish=eFALSE;
						  bFlag_1st_Case = eTRUE;
						  //Led Status 
//						  GPIO_SetBits(GPIOA,GPIO_Pin_11);
//						  GPIO_SetBits(GPIOA,GPIO_Pin_12);
//						  GPIO_SetBits(GPIOA,GPIO_Pin_15);
						break;
						  
						case PMachine_ResetCutter:
						  eState_User_Task = eST_User_Task_ResetCutter;
						  eFlag_Process_Finish = eFALSE;
						  bFlag_1st_Case = eTRUE;
						break;
						  
						case PMachine_ScanHole:
						  eState_User_Task = eST_User_Task_ScanHole;
						  eFlag_Process_Finish = eFALSE;
						  bFlag_1st_Case = eTRUE;
						break;
						  
						case PMachine_ThreadChecker:
						  eState_User_Task = eST_User_Task_ThreadChecker;
						  eFlag_Process_Finish=eFALSE;
						  bFlag_1st_Case    = eTRUE;
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
				  Control_LED_SYSTEM(SYS_WORKING);
				  bFlag_1st_Case = eFALSE;
				  BUFFER_MACHINE_CONTROL.eFlag_Process_Update=eFALSE;
				  
				  //Control X,Y,Z
				  BUFFER_AXIS_PROCESS.bProcess=RESETHOME;
				  BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
				  
				  //Control Spindle Motor
				  BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RESET;
				  BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
				  
				  #ifdef USE_DEBUG_MASTER
					BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
				  #endif /*USE_DEBUG_MASTER*/
		  }
		  else
		  {		
	
				  // STOP BUTTON
				  if(State_StopButton == eTRUE)
				  {
						eState_User_Task = eST_User_Task_StopButton;
				  }
				  else
				  {
						if(State_PauseButton == eTRUE)
						{
							Control_LED_SYSTEM(SYS_PAUSE);
						}
						else
						{
							//status of led
							Control_LED_SYSTEM(SYS_WORKING);
						  //Feedback Status Process Machine to PC
							BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_ResetHome;

							//Condition Process Finished
							if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
							  if(BUFFER_ENCODER.Flag_Home==eTRUE)
							  {
									  #ifdef USE_DEBUG_MASTER
										  BUFFER_ENCODER.Flag_Home=eFALSE;     
									  #endif /*USE_DEBUG_MASTER*/
										  
									  eFlag_Process_Finish=eTRUE;
							  }
							
							
							
							
							//Back to eST_User_Task_IDLE, when process finish
							if (eFlag_Process_Finish==eTRUE)
							{
							  BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_FinishProcess;    //Feedback Status Process Machine to PC
							  eState_User_Task = eST_User_Task_IDLE;
							}
							//Back to eST_User_Task_IDLE, when reset
							if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
							{
							  eState_User_Task = eST_User_Task_INIT;
							}
					     }
			    	}
		  		}
		break;
                
                
                
                
                
                
                
                
                
		case eST_User_Task_ResetCutter:
		  if(bFlag_1st_Case==eTRUE)
		  {
				  Control_LED_SYSTEM(SYS_WORKING);
				  bFlag_1st_Case = eFALSE;
				  BUFFER_MACHINE_CONTROL.eFlag_Process_Update=eFALSE;
										 
				  //Control Spindle Motor
				  BUFFER_CONTROL_DC_SPINDLE.bProcess = SPINDLE_RESET;             //after reset home for spindle, break DC Spindle
				  BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
				  #ifdef USE_DEBUG_MASTER
					BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
				  #endif /*USE_DEBUG_MASTER*/

		  }
		  else
		  {
				  
				  // STOP BUTTON
				  if(State_StopButton == eTRUE)
				  {
						eState_User_Task = eST_User_Task_StopButton;
				  }
				  else
				  {
						if(State_PauseButton == eTRUE)
						{
							Control_LED_SYSTEM(SYS_PAUSE);
						}
						else
						{
							//status of led
							Control_LED_SYSTEM(SYS_WORKING);
							//Control X,Y,Z
							if(BUFFER_ENCODER.Flag_Home==eTRUE)
							{
							  #ifdef USE_DEBUG_MASTER
								  BUFFER_ENCODER.Flag_Home=eFALSE;     
							  #endif /*USE_DEBUG_MASTER*/
							  vReleaseCutter();
							  BUFFER_AXIS_PROCESS.bProcess=RELEASECUTTER;
							  BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
							}
							
							//Feedback Status Process Machine to PC
							BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_ResetCutter;
												
			 
							//Condition Process Finished
							if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
							  eFlag_Process_Finish=eTRUE;
						  
							
							
							
							
							//Back to eST_User_Task_IDLE, when process finish
							if (eFlag_Process_Finish==eTRUE)
							{
							  BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_FinishProcess;    //Feedback Status Process Machine to PC
							  eState_User_Task=eST_User_Task_IDLE;
							}
							//Back to eST_User_Task_IDLE, when reset
							if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
							{
							  
							  eState_User_Task=eST_User_Task_INIT;
							}
					    }
			 		}
		  		}
		break;
                
                
                
                
                
                
                
                
                
                
		case eST_User_Task_ScanHole:
		  if(bFlag_1st_Case==eTRUE)
		  {
				 Control_LED_SYSTEM(SYS_WORKING);
				  bFlag_1st_Case = eFALSE;
				  BUFFER_MACHINE_CONTROL.eFlag_Process_Update=eFALSE;
										 
				  //Control X,Y,Z
				  BUFFER_AXIS_PROCESS.bProcess=RUNTOSCAN;
				  BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
				  
				  #ifdef USE_DEBUG_MASTER
					BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
				  #endif /*USE_DEBUG_MASTER*/

		  }
		  else
		  {
			  // STOP BUTTON
			  if (State_StopButton == eTRUE)
			  {
					eState_User_Task = eST_User_Task_StopButton;
			  }
			  else
				  {
					if(State_PauseButton == eTRUE)
					{
						Control_LED_SYSTEM(SYS_PAUSE);	
					}
					else
					{
						//status of led
						Control_LED_SYSTEM(SYS_WORKING);
						//Scan Thread Hole
						if(bDetectThreadHole())
						  BUFFER_HOLE_DATA.Detect_Thread_Hole[BUFFER_HOLE_DATA.iIdex_hole_data]=eTRUE;
				   

						//Feedback Status Process Machine to PC
						BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_ScanHole;
											
		 
						//Condition Process Finished
						if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
						  eFlag_Process_Finish=eTRUE;
					  
						
						
						
						
						//Back to eST_User_Task_IDLE, when process finish
						if (eFlag_Process_Finish==eTRUE)
						{
						  BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine = PMachine_FinishProcess;    //Feedback Status Process Machine to PC
						  eState_User_Task=eST_User_Task_IDLE;
						}
						//Back to eST_User_Task_IDLE, when reset
						if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine == PMachine_Reset)
						{
						  
						  eState_User_Task=eST_User_Task_INIT;
						}
						}
				     }
		  		}
		break;
                
      
             
                
                
		case eST_User_Task_ThreadChecker:
		  if(bFlag_1st_Case==eTRUE)
		  {
				  Control_LED_SYSTEM(SYS_WORKING);
				  bFlag_1st_Case = eFALSE;
				  BUFFER_MACHINE_CONTROL.eFlag_Process_Update=eFALSE;
										 
				  //Control X,Y,Z
				  Status_CheckThreadProcess = Status_ChangeCutter;
				  bFlag_1st_Go=eTRUE;
				  
				  timer_set(&tIO_SpindleUpInHole, Time_SpindleUpInHole ,CLOCK_TYPE_MS); /*700ms */
				  
				  #ifdef USE_DEBUG_MASTER
					BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
				  #endif /*USE_DEBUG_MASTER*/
		  }
		  else
		  {
			  // STOP BUTTON
			  if(State_StopButton == eTRUE)
			  {
					eState_User_Task = eST_User_Task_StopButton;
			  }
			  else
				{
					if(State_PauseButton == eTRUE)
					{
						Control_LED_SYSTEM(SYS_PAUSE);
					}
					else
					{
						//status of led
						Control_LED_SYSTEM(SYS_WORKING);
						//Read Hole Data follow iIdex_hole_check
						switch (Status_CheckThreadProcess)
						{
							case Status_ChangeCutter :
							   //What things to do
								  //1st : Change cutter
								  if(bFlag_1st_Go==eTRUE && BUFFER_HOLE_DATA.Dia_Hole[BUFFER_MACHINE_CONTROL.iIdex_hole_check] != BUFFER_MACHINE_CONTROL.sHold_Cutter)
									{
									  bChangeCutter = eTRUE;
									  sChangeCutter = sReleaseCutter;
									  bFlag_1st_Go  = eFALSE; 
									}
									if(bChangeCutter==eTRUE)
									{
									  vStepChangeCutterProcess();
									}
								//Conditions Change State :
									if(bChangeCutter==eFALSE)
									  Status_CheckThreadProcess = Status_RuntoHole;
							  break;
							  
							case Status_RuntoHole :
							  //What things to do
								  //2nd : Control X,Y runtopoint
								  BUFFER_X_AXIS_CONTROL.Axis_PositionControl=BUFFER_HOLE_DATA.Position_X[BUFFER_MACHINE_CONTROL.iIdex_hole_check];
								  BUFFER_X_AXIS_CONTROL.bFlag_Update=eTRUE;
								  BUFFER_Y_AXIS_CONTROL.Axis_PositionControl=BUFFER_HOLE_DATA.Position_Y[BUFFER_MACHINE_CONTROL.iIdex_hole_check];
								  BUFFER_Y_AXIS_CONTROL.bFlag_Update=eTRUE;
								  
								  BUFFER_AXIS_PROCESS.bProcess=RUNTOPOINT;
								  BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
								  
								  BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_STOP;
								  BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
								  
							   //Conditions Change State :
								  if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
								  {
									Status_CheckThreadProcess=Status_CheckThread;
									Status_SpindleCheckProcess=sSpindle_GetReady;
									#ifdef USE_DEBUG_MASTER
									  BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
									#endif /*USE_DEBUG_MASTER*/
				
								  }
							break;
							  
							case Status_CheckThread :
								  //3rd : + control air valve - IO output
								  //3rd : + review Current Measure to define quality of hole
								  //3rd : + run follow steps
								  vStepSpindleCheckHoleProcess();
								  //Conditions Change State :
								  if(Status_SpindleCheckProcess == sSpindle_Finish)
									Status_CheckThreadProcess = Status_FinishThreadProcess;
							 break;
							  
							case Status_FinishThreadProcess :
								  eFlag_Process_Finish=eTRUE;
							break;
							  
							default :
							break;
							  
						}
				

							//Feedback Status Process Machine to PC
							BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_ThreadChecker;
												
			 
							//Condition Process Finished

						  
							
							//Back to eST_User_Task_IDLE, when process finish
							if (eFlag_Process_Finish==eTRUE)
							{
							  BUFFER_MACHINE_CONTROL.bProcess_Feedback_Machine=PMachine_FinishProcess;    //Feedback Status Process Machine to PC
							  eState_User_Task=eST_User_Task_IDLE;
							}
							//Back to eST_User_Task_IDLE, when reset
							if (BUFFER_MACHINE_CONTROL.bProcess_Control_Machine==PMachine_Reset)
							{
							  eState_User_Task=eST_User_Task_INIT;
							}

					  	}
			 		}
		  		}
		break;
		case eST_User_Task_StopButton:
			BUFFER_STATEBUTTON.bflag_Stop = 1;
			eState_User_Task  = eST_User_Task_IDLE;
			bFlag_1st_Case = eTRUE;
			GPIO_ResetBits(Led,WorkingLed); 
   			memset(UART1_BUFFER_TX,0,i_MAX_UART);
			memset(UART2_BUFFER_TX,0,i_MAX_UART);
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
                //<<<<<<<<<-------------------SYSTEM WORKS---------------------->>>>>>>>>//
                

		default:
			eState_User_Task = eST_User_Task_INIT;
			bFlag_1st_Case = eTRUE;
		break;
	}
}

/*--------------Init Parameter For FlowChart----------------*/

void vInitCutter()
{
  BUFFER_MACHINE_CONTROL.PositionCutter_X[1]=320;
  BUFFER_MACHINE_CONTROL.PositionCutter_X[2]=320;
  BUFFER_MACHINE_CONTROL.PositionCutter_X[3]=320;
  BUFFER_MACHINE_CONTROL.PositionCutter_X[4]=320;
  BUFFER_MACHINE_CONTROL.PositionCutter_X[5]=320;
  
  BUFFER_MACHINE_CONTROL.PositionCutter_Y[1]=40;
  BUFFER_MACHINE_CONTROL.PositionCutter_Y[2]=70;
  BUFFER_MACHINE_CONTROL.PositionCutter_Y[3]=100;
  BUFFER_MACHINE_CONTROL.PositionCutter_Y[4]=130;
  BUFFER_MACHINE_CONTROL.PositionCutter_Y[5]=170;
  
  BUFFER_MACHINE_CONTROL.sHold_Cutter=0x02;
}


/*--------------Change Cutter----------------*/
//Get X,Y Position Cutter from Cutter is Holding
void vReleaseCutter()
{
  uint8 iIndex_Cutter = BUFFER_MACHINE_CONTROL.sHold_Cutter;
  
  BUFFER_X_AXIS_CONTROL.Axis_PositionControl=BUFFER_MACHINE_CONTROL.PositionCutter_X[iIndex_Cutter];
  BUFFER_X_AXIS_CONTROL.bFlag_Update=eTRUE;
  BUFFER_Y_AXIS_CONTROL.Axis_PositionControl=BUFFER_MACHINE_CONTROL.PositionCutter_Y[iIndex_Cutter];
  BUFFER_Y_AXIS_CONTROL.bFlag_Update=eTRUE;
}

//Get X,Y Position Cutter follow Dia Hole want to check 
void vGetCutter()
{
  uint8 iIndex_Cutter = BUFFER_HOLE_DATA.Dia_Hole[BUFFER_MACHINE_CONTROL.iIdex_hole_check];
  
  BUFFER_X_AXIS_CONTROL.Axis_PositionControl=BUFFER_MACHINE_CONTROL.PositionCutter_X[iIndex_Cutter];
  BUFFER_X_AXIS_CONTROL.bFlag_Update=eTRUE;
  BUFFER_Y_AXIS_CONTROL.Axis_PositionControl=BUFFER_MACHINE_CONTROL.PositionCutter_Y[iIndex_Cutter];
  BUFFER_Y_AXIS_CONTROL.bFlag_Update=eTRUE;
}


/*--------------Control Air Valve----------------*/

void vSetAirVale(state_control_air_value bProcess)
{
  switch(bProcess)
  {
  case SpindleGoDown:
		GPIO_SetBits(sCoil_1, sCoil_1_GoDown);
        GPIO_ResetBits(sCoil_2,sCoil_2_GoDown);
 		GPIO_ResetBits(sCoil_3,sCoil_3_GoDown);
    break;
    
  case SpindleGoUp:
        GPIO_ResetBits(sCoil_1,sCoil_1_GoUp);
		GPIO_SetBits(sCoil_2,sCoil_2_GoUp);
 		GPIO_ResetBits(sCoil_3,sCoil_3_GoUp);
    break;
    
  case SpindleResetHome:
 		GPIO_SetBits(sCoil_3,sCoil_3_ResetHome);
		GPIO_ResetBits(sCoil_2,sCoil_2_ResetHome);
        GPIO_ResetBits(sCoil_1,sCoil_1_ResetHome);
		
 		
    break;

  default:
    break;
  }
}

/*--------------Thread Checker Flowchart----------------*/
void vStepChangeCutterProcess()
{
    switch(sChangeCutter)
    {
    case sReleaseCutter :
      //What things to do
      BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RESET;
      BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
      if(BUFFER_ENCODER.Flag_Home == eTRUE)
      {
            vReleaseCutter();
            BUFFER_AXIS_PROCESS.bProcess=RELEASECUTTER;
            BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
            //Conditions Change State :
            if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
            {
              sChangeCutter=sGetCutter;
              #ifdef USE_DEBUG_MASTER
                BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;
                BUFFER_ENCODER.Flag_Home=eFALSE;  
              #endif /*USE_DEBUG_MASTER*/
            }
      }
    break;
    
    case sGetCutter :
      //What things to do
      BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RESET;
      BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
      if(BUFFER_ENCODER.Flag_Home==eTRUE)
      {                
            vGetCutter();
            BUFFER_AXIS_PROCESS.bProcess=GETCUTTER;
            BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
            //Conditions Change State :
            if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
            {
              sChangeCutter=sChangeCutterFinish;
              #ifdef USE_DEBUG_MASTER
                BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;
                BUFFER_ENCODER.Flag_Home=eFALSE; 
              #endif /*USE_DEBUG_MASTER*/
            }
      }
    break;
      
    case sChangeCutterFinish :
      bChangeCutter=eFALSE;
    break;
      
    default:
      bChangeCutter=eFALSE;
    break;
    }
}

void vStepSpindleCheckHoleProcess()
{
  switch(Status_SpindleCheckProcess)
  {
    case sSpindle_GetReady:
          //What things to do
          vSetAirVale(SpindleResetHome);
          BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_STOP;
          BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
          
          //Conditions Change State :
          Status_SpindleCheckProcess=sSpindle_Z_GoDown;
      break;
      
    case sSpindle_Z_GoDown :
          //What things to do
          BUFFER_Z_AXIS_CONTROL.Axis_PositionControl=Z_CheckThread;             //Z go to Z=50
          BUFFER_Z_AXIS_CONTROL.bFlag_Update=eTRUE;
          BUFFER_AXIS_PROCESS.bProcess=RUNTOPOINT;
          BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
          
          vSetAirVale(SpindleResetHome);
          //Conditions Change State :
          if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
          {
            Status_SpindleCheckProcess=sSpindle_Spindle_GoDown;
            #ifdef USE_DEBUG_MASTER
              BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
            #endif /*USE_DEBUG_MASTER*/
          }
      
      break;
      
    case sSpindle_Spindle_GoDown:
          //What things to do
          Buffer_LinearScale.spindle_position = SPINDLE_LINEARHOME;                           
          
          BUFFER_CONTROL_DC_SPINDLE.bDC_Driection=SPINDLE_FORWARD;              
          BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RORATY;
          BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
          
          vSetAirVale(SpindleGoDown);                                          
      
          //Conditions Change State :
          Status_SpindleCheckProcess=sSpindle_Checking;
          bFlag_1st_Go=eTRUE;
      
      break;
      
    case sSpindle_Checking:
          //What things to do
          //In Slave2, it read Current Value, check with Current Max
          //If Current Value reach Current Max, Immediately, Slave2 change bDC_Driection control to Reverse Motor 
//          if(timer_expired(&tIO_SpindleUpInHole))
//            uint8 test = 1;
          
          if(Buffer_LinearScale.spindle_position<=SPINDLE_POSITION_CHECK)
            bFlag_FinishCheckHole=eTRUE;
      
          if(bFlag_1st_Go==eTRUE && bFlag_FinishCheckHole==eTRUE)
          {
            bFlag_1st_Go  = eFALSE;
            BUFFER_CONTROL_DC_SPINDLE.bDC_Driection=SPINDLE_REVERSE;
            
            BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RORATY;
            BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
            
            vSetAirVale(SpindleGoUp);
            
            timer_restart(&tIO_SpindleUpInHole);
            bFlag_TimeSet=eTRUE;    
          }
          
          if(BUFFER_CURRENT_MEASURE.Flag_QualityPoor==eTRUE)
          {
            BUFFER_HOLE_DATA.Hole_Check_Value[BUFFER_HOLE_DATA.iIdex_hole_data]=POOR;
            
            BUFFER_CONTROL_DC_SPINDLE.bDC_Driection=SPINDLE_REVERSE;
            
            BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RORATY;
            BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
            
            vSetAirVale(SpindleGoUp);
            
            timer_restart(&tIO_SpindleUpInHole);
            bFlag_TimeSet=eTRUE;
            
          }
          //Conditions Change State :
          if((bFlag_TimeSet==eTRUE) && (timer_expired(&tIO_SpindleUpInHole)))
          {
            Status_SpindleCheckProcess=sSpindle_Spindle_GoUp;
          }
      
      break;
      
      
    case sSpindle_Spindle_GoUp:
          //What things to do
          BUFFER_CONTROL_DC_SPINDLE.bDC_Driection=SPINDLE_REVERSE;
          
          BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_RORATY;
          BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
          
          vSetAirVale(SpindleResetHome); 
      
          //Conditions Change State :
          if(Buffer_LinearScale.spindle_position>SPINDLE_LINEARHOME-5)
            Status_SpindleCheckProcess=sSpindle_Z_GoUp;
      
      break;
      
    case sSpindle_Z_GoUp:
          //What things to do
          BUFFER_Z_AXIS_CONTROL.Axis_PositionControl=Z_SafePosition;             //Z go to Z=100
          BUFFER_Z_AXIS_CONTROL.bFlag_Update=eTRUE;
          BUFFER_AXIS_PROCESS.bProcess=RUNTOPOINT;
          BUFFER_AXIS_PROCESS.Flag_Update=eTRUE;
      
          vSetAirVale(SpindleResetHome); 
          BUFFER_CONTROL_DC_SPINDLE.bProcess=SPINDLE_STOP;
          BUFFER_CONTROL_DC_SPINDLE.Flag_Update=eTRUE;
          
          //Conditions Change State :
          if(BUFFER_AXIS_PROCESS.bFeedBackAxis==eAxis_Finsish)
          {
            Status_SpindleCheckProcess=sSpindle_Finish;
            #ifdef USE_DEBUG_MASTER
              BUFFER_AXIS_PROCESS.bFeedBackAxis=eAxis_InProcess;     
            #endif /*USE_DEBUG_MASTER*/
          }
      
      break;
      
    case sSpindle_Finish:
          //What things to do
          
      
          //Conditions Change State :
      
      
      break;
      
    default:
      break;
  }
}


























