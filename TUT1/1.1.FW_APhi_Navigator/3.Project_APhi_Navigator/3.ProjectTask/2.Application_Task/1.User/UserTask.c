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

/* Global Variable */
extern Struct_User_Information StrUserInfo;
extern char StrConfig[FLASH_PAGE_SIZE];
extern Struct_Flash_Config_Parameter	StrConfigPara;
extern enumbool xFlag_User_Task_Still_Running;
extern Struct_System_Information 		StrSystemInfo;
extern xTaskHandle xRF_Task_Handle, xSensor_Task_Handle, xIO_Task_Handle, xSensor_IO_Task_Handle;

extern enumbool xFlag_RF_Task_Init_Done, xFlag_User_Task_Init_Done, xFlag_Sensor_Task_Init_Done, xFlag_Sensor_IO_Task_Init_Done;
extern enumbool xFlag_RF_Task_Process_Check;
extern struct tm tReal_time;
/* Define value for check time */
#define NUMBER_CHECK_TAG_ACTIVE_TIME 	1000	/* 1000*10ms = 10s */
#define NUMBER_CHECK_TAG_FAIL_ANOUCE 	500	    /* 500*10ms = 5s */
#define MAX_CHECK_TIME_TAG_LOST 		30	/* 30s */
#define TIME_3S_CHECK_TIME_TAG_LOST 	3	/* 3s */
#define TIME_7S_CHECK_TIME_TAG_LOST 	7	/* 7s */
#define TIME_10S_CHECK_TIME_TAG_LOST 	10	/* 10s */
#define USER_TASK_FREQUENCY				10	/* 10 ms */
#define NUMBER_CYCCLE_USER_1S			100	/* 100*10ms = 1s */
//#define TIME_TAG_NEAR_LOST_S			1 	/* 5s get only FAR signal or not get signal */
#define TIME_TAG_NEAR_LOST_S			5 	/* 5s get only FAR signal or not get signal */
//#define NUMBER_ACCE_ALARM			    400 /* 400*10ms=4s */
#define NUMBER_ACCE_ALARM			    4 /* 4 time */
#define TAG_BATTERY_ALARM				70	/* 70% */
#define TIME_LAST_DEEP_ACTIVE           600 /* 600s = 10m */
//#define TIME_LAST_DEEP_ACTIVE           300 /* 300s = 5m */

/* Define for timing */
#define TIME_TIMING_SAVE_FLASH				3600/* 3600s = 1h */
#define TIME_REINIT_MODULE_LOW_POWER		10/TIME_WAKE_UP_RTC_FREQUENCY

/* State of User Task */
typedef enum
{
	eST_User_Task_IDLE 						= 1,
	eST_User_Task_ALARM 					= 2,
	eST_User_Task_RUN_MODE_0 				= 3,
	eST_User_Task_RUN_MODE_1				= 4,
	eST_User_Task_RUN_MODE_2				= 5,
	eST_User_Task_RUN_MODE_Emergency		= 6,
	eST_User_Task_LOW_POWER					= 7,/* Only MCU sleep, RF & ACCE on, Key on*/
	eST_User_Task_DEEP_POWER				= 8,/* Only ACCE, Key on, RF MCU Sleep */
	eST_User_Task_SEFT_PROTECT				= 9,/* Mode SEFT PROTECT */
	eST_User_Task_TEST_CONFIG_MODE			= 10,
	eST_User_Task_PWM						= 11,
	
	eST_User_Task_AFTER_RESET				= 0xfe,/* After reset mode */
	eST_User_Task_UN 						= 0xff,
}eST_User_Task;

/* Wake Up source enum */
typedef enum
{
	eWakeUpSource_Key	= 1,
	eWakeUpSource_Leg	= 2,
	eWakeUpSource_RF	= 3,
	eWakeUpSource_ACCE	= 4,
	eWakeUpSource_RTC	= 5,
	
	eWakeUpSource_Null 	= 0xfe,
	eWakeUpSource_UN 	= 0xff,
}eWakeUpSource;
static eWakeUpSource eWakeUpEvent, eIntEvent;

/* Local variable */
static eST_User_Task eState_User_Task = eST_User_Task_IDLE;
//static eST_User_Task eState_User_Task = eST_User_Task_PWM;
enumbool bFlag_1st_Case = eTRUE;
enumbool bFlag_ChangeMode_2To1 = eFALSE;
enumbool bFlag_ChangeMode_SeftProtect = eFALSE;
enumbool bFlagRFDataProcess = eFALSE;
enumbool bFlagRunningCheckTagProcess = eTRUE;

/* Local timer */
static timer pTimerChangeToLowPower, pTimerLowPowerStep4Out;
static timer pEmergencyOut, pTimerChangeSeflProtect;
static timer pTimerEnableMode2Leg;
static timer pCheckACCEAlarm;
static uint32_t bCountChangeLowPowerToDeepPower = 0, bCountTimeoutLowpowerStep3 = 0;

/* RF timing signal */
uint16_t bCountTimeTagLostCycle=0;
enumbool bTAGNearSignal=eFALSE;
enumbool bFlagChangeMode=eFALSE;
enumbool bFlagDisableAcceCheck=eFALSE, bFlagDisableAcceCheck_FindBike_Command=eFALSE;
enumbool bFlagStopAlarm=eFALSE, bFlagStopAlarmAlready = eFALSE;
enumbool bFlagTrunk_ChangeMode0_Command=eFALSE;
enumbool bFlagChange_Config_Command=eFALSE;
enumbool bFlagConfig_Parameter_Command=eFALSE;
enumbool bFlagEnd_Config_Command=eFALSE;
enumbool bFlagChangeAlarmMode=eFALSE;
enumbool bFlagRunningCommand=eFALSE;
enumbool bFlagSeftProtect=eFALSE;
enumbool bFlagSayHelloAlready = eFALSE;
uint8_t bTime_TagNear_Lost=0;
uint8_t bTime_Tag_Lost=0;
uint16_t bCountTimeCheckStartSignal = 0;
uint16_t bCountCheckTagActive=0;
uint16_t bCountACCEAlarm=0;
uint8_t bCountChangeToSelfProtect=0;
static uint8_t bLastCommandIndex = 0;

/* Multi connect flag */
enumbool bFlag_Sensor_Check_Signal = eFALSE;
enumbool bFlag_RF_Reinit_Module = eFALSE;
enumbool bFlag_RF_TurnOff_Module = eFALSE;
enumbool bFlag_ACCE_Reinit_Module = eFALSE;
enumbool bFlag_Reset_SensorIO_Value = eFALSE;
enumbool bFlag_DeepPower_Active = eFALSE, bFlag_LowPower_Active = eFALSE, bFlagCutboardActive = eFALSE, bFlagSetConfigACCEDone = eFALSE;
enumbool bFlag_ReCreate_Alltask = eFALSE, bFlag_Resume_Alltask = eFALSE;
enumbool bFlag_Running_Active = eTRUE;

/* Local function */
void vUserTaskMainProcess(void);
void vChooseModeProcess(void);
void vRunMode1Process(void);
void vRunMode2Process(void);
enumbool vRFDataProcess(void);
void vRunningCheckTagProcess(void);
void vRunModeEmergency(void);
eWakeUpSource vStopModeAndWakeup(void);
void vUserTaskMainTestProcess(void);
enumbool vRFProcessTAGCommand(void);
void vAccelometerProcess(void);
eWakeUpSource vCheckIntSource(void);
void vRunModeConfig(void);

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

/* Emergency variable */
typedef enum
{
	eST_RunEmergencyWaitStartEvent 	= 1,
	eST_RunEmergencyWaitKey10s		= 2,
	eST_RunEmergencyProcess			= 4,
	eST_RunEmergencyEnd				= 5,
	eST_RunEmergency_UN = 0xff,
}eST_RunEmergency;
static eST_RunEmergency eRunEmergencyState = eST_RunEmergencyWaitStartEvent;
static enumbool bFlag_1st_Case_ModeEmergency = eTRUE;
/* RunMode 1 variable */
typedef enum
{
	eST_RunMode1WaitStartEvent 	= 1,
	eST_RunMode1Process			= 2,
	eST_RunMode1End				= 3,
	eST_RunMode1_UN = 0xff,
}eST_RunMode1;
static eST_RunMode1 eRunMode1State = eST_RunMode1WaitStartEvent;
static enumbool bFlag_1st_Case_Mode1=eTRUE;

/* RunMode 2 variable */
typedef enum
{
	eST_RunMode2WaitStartEvent 	= 1,
	eST_RunMode2CheckStartSignal= 2,
	eST_RunMode2Process			= 3,
	eST_RunMode2End				= 4,
	eST_RunMode2CheckKeyChangeMode1 = 5,
	eST_RunMode2_UN = 0xff,
}eST_RunMode2;
static eST_RunMode2 eRunMode2State = eST_RunMode2WaitStartEvent;
static enumbool bFlag_1st_Case_Mode2=eTRUE;

/*-----------------------------------------------------------*/
void vUserTask( void *pvParameters )
{
        /* Local variable */
        static enumbool bFlagSystemRun = eFALSE;
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
                
                /* Output LED */
//                if(BUTTON_1_STATE==eButtonSingleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,20,100,2,RELAY_ON,RELAY_OFF,eFALSE);
//                }
//                else if(BUTTON_1_STATE==eButtonDoubleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,20,100,4,RELAY_ON,RELAY_OFF,eFALSE);
//                }
//                else if(BUTTON_1_STATE==eButtonTripleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,20,120,6,RELAY_ON,RELAY_OFF,eFALSE);
//                }
                
//                if(BUTTON_1_STATE==eButtonSingleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,20,500,2,RELAY_ON,RELAY_OFF,eTRUE);
//                }
//                else if(BUTTON_1_STATE==eButtonDoubleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,20,500,4,RELAY_ON,RELAY_OFF,eTRUE);
//                }
                
//                if(BUTTON_1_STATE==eButtonSingleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,10,100,2,RELAY_ON,RELAY_OFF,eFALSE);
//                }
//                else if(BUTTON_1_STATE==eButtonDoubleClick)
//                {
//                  vIO_ConfigOutput(&OUT_LED_1,10,100,4,RELAY_ON,RELAY_OFF,eFALSE);
//                }
//                else if(BUTTON_1_STATE==eButtonLongPressT1)
//                {
//                  vIO_ConfigOutput(&OUT_LED_2,10,200,20,RELAY_ON,RELAY_OFF,eFALSE);
//                }
//                else if(BUTTON_1_STATE==eButtonLongPressT2)
//                {
//                  vIO_ConfigOutput(&OUT_LED_2,5,200,40,RELAY_ON,RELAY_OFF,eFALSE);
//                }
//                else if(BUTTON_1_STATE==eButtonHoldOn)
//                {
//                  vIO_ConfigOutput(&OUT_LED_2,2,200,100,RELAY_ON,RELAY_OFF,eFALSE);
//                }
                USART1_AppCall_SendString("[SYSTEM DEBUG]: Hello \r\n");
//                 USART2_AppCall_SendString("[SYSTEM DEBUG]: Hello \r\n");
                /* Emergency Stop */
                if(bFlagSystemRun==eFALSE)
                {
                  vIO_ConfigOutput(&OUT_LED_1,10,100,10,RELAY_ON,RELAY_OFF,eFALSE);
                  vIO_ConfigOutput(&OUT_LED_2,10,100,10,RELAY_ON,RELAY_OFF,eFALSE);
                }
                
                if(EMERGENCY_BUTTON_STATE==eButtonSingleClick)
                //if(EMERGENCY_BUTTON_IO==0)
                {
                  bFlagSystemRun = eTRUE;
                  vIO_ConfigOutput(&OUT_LED_1,10,0,0,RELAY_OFF,RELAY_OFF,eTRUE);
                  vIO_ConfigOutput(&OUT_LED_2,10,0,0,RELAY_OFF,RELAY_OFF,eTRUE);
                }
          }
}
/*********************************************************************/
void vUserTaskMainProcess(void)
{
	switch(eState_User_Task)
	{
		case eST_User_Task_AFTER_RESET:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{

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
		case eST_User_Task_SEFT_PROTECT:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				
			}
		break;
		case eST_User_Task_ALARM:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				
			}
		break;
		case eST_User_Task_RUN_MODE_0:
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
		case eST_User_Task_RUN_MODE_1:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
				
			}
		break;
		case eST_User_Task_RUN_MODE_2:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
			}
		break;
		case eST_User_Task_RUN_MODE_Emergency:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
			}
		break;
		case eST_User_Task_LOW_POWER:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
			}
		break;
		case eST_User_Task_DEEP_POWER:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
			}
		break;
		case eST_User_Task_TEST_CONFIG_MODE:
			if(bFlag_1st_Case==eTRUE)
			{
				bFlag_1st_Case = eFALSE;
			}
			else
			{
			}
		break;
		case eST_User_Task_PWM:
			
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
