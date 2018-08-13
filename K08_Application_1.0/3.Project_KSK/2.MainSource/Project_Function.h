/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Project_Function.h
* Author             : OS Team
* Date First Issued  : 27/09/2014
* Description        : This file all project function .

********************************************************************************
* History: DD/MM/YY
* 27/09/2014: V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Project_Function__H
#define _Project_Function__H
/* Include global header file */
#include "Common.h"
#include "Timer_Function.h"

/*----------------------------------------------------------------------------------------*/
//--------------DEFINE FOR PROTOTYPE & MAIN SOURCE USE FUNCTION PROTOTYPE----------------//
/*--------------------------------------------------------------------------------------*/
void vMCU_Init_Hardware( void );
void vProject_Init( void );

/*----------------------------------------------------------------------------------------*/
//-------------------DEFINE FOR PROTOTYPE & FLASH FUNCTION PROTOTYPE---------------------//
/*--------------------------------------------------------------------------------------*/

/* Flash Function prototype */
#define     FW_FLASH_ADDR	        0x08005000	/* 20KB bootloader */
#define     USER_INFO_FLASH_ADDR	0x08004800	/* 18KB bootloader */
//#define   FLASH_PAGE_SIZE             0x800		//2048	/* 0x800 2K per page, 1K value 0x400*/
#define     FLASH_PAGE_SIZE             0x400		//2048	/* 0x800 2K per page, 1K value 0x400*//* STM32F103C8 */
#define     FLASH_WAIT_TIMEOUT          100000

void vFLASH_User_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length);
void vFLASH_User_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length);
void vFLASH_UpdateData(void);

/*-----------------------------------------------------------------------------*/
//--------------DEFINE FOR PROTOTYPE & ENCODER FUNCTION PROTOTYPE-------------//
/*---------------------------------------------------------------------------*/

/*ENCODER OF AXIS X*/
void vGetEncoderValue_X(void);
void EXTI1_IRQHandler(void);
void EXTILine1_Config(void);




/*ENCODER OF AXIS Y*/
void EXTI3_IRQHandler(void);
void EXTILine3_Config(void);
void vGetEncoderValue_Y(void);

/*-----------------------------------------------------------------------------*/
//------------DEFINE FOR PROTOTYPE & STEPMOTOR FUNCTION PROTOTYPE-------------//
/*---------------------------------------------------------------------------*/

void vInit_STEP_MOTOR_Function (void);
void Generate_Pulse_X(void);
void Generate_Pulse_Y(void);
void Generate_Pulse_Z(void);
void Calculate_Pulse(uint8 iIndex_avitme, uint8 iIndex_amicro) ;
void Control_Pulse (void);
void MotorStep_Control(void);
void Determined_Position(uint8 iIndex_amicro);
void Compare_Position(void);

extern uint32 bStepMotor;
extern uint32 Status_StepMotor;
/*DEFIN PIN FOR STEP MOTOR OF AXIS X*/
#define pin_X_CCW		 	GPIO_Pin_3
#define pin_X_DIR			GPIO_Pin_4
#define pin_X_ENABLE		        GPIO_Pin_5
/*DEFIN PIN FOR STEP MOTOR OF AXIS Y*/
#define pin_Y_CCW		 	GPIO_Pin_15
#define pin_Y_DIR			GPIO_Pin_12
#define pin_Y_ENABLE		        GPIO_Pin_11
/*DEFIN PIN FOR STEP MOTOR OF AXIS Z*/
#define pin_Z_CCW		 	GPIO_Pin_14
#define pin_Z_DIR			GPIO_Pin_13
#define pin_Z_ENABLE		        GPIO_Pin_12
/**********************************************************/

extern int i;
/*------------------------------------------------------------------------------*/
//-------------------------------RESET HOME-------------------------------------//
/*-----------------------------------------------------------------------------*/
void X_HOME(void);
void Y_HOME(void);
void Z_HOME(void);
extern enumbool bFlag_GoHOME_X ;
extern enumbool bFlag_GoHOME_Y ;
extern enumbool bFlag_GoHOME_Z ;

extern uint32 State_ReleaseCutter;
extern uint32 State_GetCutter ;

extern enumbool bState_Enstop1; 
/*-----------------------------------------------------------------------------*/
//---------------------------------RESET CUTTER---------------------------------//
/*---------------------------------------------------------------------------*/
void GetCutter_Machine(void);
void ReleaseCutter_Machine(void);
/*-----------------------------------------------------------------------------*/
//---------------------------------RUN TO POINT--------------------------------//
/*---------------------------------------------------------------------------*/
void Run_To_Point(void);
extern uint32 State_RunToPoint ;
/*-----------------------------------------------------------------------------*/
//---------------------------------SCANHOLE--------------------------------//
/*---------------------------------------------------------------------------*/
void ScanHole (void);
extern uint32 State_ScanHole_Y;
extern uint32 State_ScanHole_X;
extern enumbool bFlag_Scanhold_Finish;
void LeftToRight_Scanhole(void);
void RightToLeft_Scanhole(void);
void X_Scanhole(void);

/*-----------------------------------------------------------------------------*/
//--------------DEFINE FOR PROTOTYPE & IO FUNCTION PROTOTYPE------------------//
/*---------------------------------------------------------------------------*/

/* Extern variabe */
extern structIO_Button strIO_Button_Value, strOld_IO_Button_Value;
extern IO_Struct pLED1, pCutOffCircuit;
extern structIO_Manage_Output strRELAY_1, strRELAY_2, strRELAY_3;
extern structIO_Manage_Output strLED_1, strBELL, strTRUNK;
extern IO_Struct pRS485_DIR;

/* extern adc value */
extern __IO uint16_t ADCConvertedValue;







/*status step motor*/
#define MOTOR_STEP_FORWARD 		1
#define MOTOR_STEP_REVERSE 		2
#define MOTOR_STEP_STOP 		3

#define MOTOR_STEP_GOFORWARD	4
#define MOTOR_STEP_GOREVERSE	5
/* CONTROL DIRECTION MOTOR*/
#define MOTOR_STEP_ABLE		6
#define MOTOR_STEP_DISABLE 	7
#define MOTOR_STEP_BRAKE 	8
/*************Detect EndStop****************/
typedef enum sStepMotor 
{
	StepMotorX        = 0x00,
	StepMotorY        = 0x01,
	StepMotorZ         = 0x02,
}sStepMotor;


extern uint32 Cnt_TimeHold_EndStop_X_1 ;
extern enumbool State_EndStop_X_1 ;
extern uint32 Cnt_TimeHold_EndStop_X_2 ;
extern enumbool State_EndStop_X_2 ;

extern uint32 Cnt_TimeHold_EndStop_Y_1 ;
extern enumbool State_EndStop_Y_1 ;
extern uint32 Cnt_TimeHold_EndStop_Y_2 ;
extern enumbool State_EndStop_Y_2 ;

extern uint32 Cnt_TimeHold_EndStop_Z_1;
extern enumbool State_EndStop_Z_1;
extern uint32 Cnt_TimeHold_EndStop_Z_2 ;
extern enumbool State_EndStop_Z_2;

/******************Detect proximity sensor***********************/
extern uint8 Cnt_TimeHold_Proximity_Sensor[6];
extern enumbool State_Proximity_Sensor[6];
extern uint32 bFlag_Status_Sensor ;
/***********************Error_Process***************************/
void vInit_Error_Process(void);
void vInitDetectOverLoad(void);
extern enumbool bFlag_Error_Process ;
/*CONTROL MOTOR WHEN HAVE STATUS OF BUTTON*/
void ControlMotor_Button(void);






/****************************************/
#define LED_USER_1_ON		pLED1.write(ON);
#define LED_USER_1_OFF		pLED1.write(OFF);
#define LED_USER_1_TOGGLE	pLED1.write(1-pLED1.writeSta());
#define LED_USER_2_ON		pLED2.write(ON);
#define LED_USER_2_OFF		pLED2.write(OFF);
#define LED_USER_2_TOGGLE	pLED2.write(1-pLED2.writeSta());
#define BUTTON_1_STATE		strIO_Button_Value.bButtonState[eButton1]
#define BUTTON_2_STATE		strIO_Button_Value.bButtonState[eButton2]
#define LEG_BIKE_IN_STATE	strIO_Button_Value.bButtonState[eButton2]

#define OUT_LED_1			strLED_1
#define OUT_LED_2           strLED_2

#define EMERGENCY_BUTTON_1_STATE	BUTTON_1_STATE
#define EMERGENCY_BUTTON_IO	        pBUT_1.read()
#define EMERGENCY_BUTTON_2_STATE	BUTTON_2_STATE
#define EMERGENCY_BUTTON_IO      	pBUT_2.read()



/* Define function for all function of RFID Bike project */	
#define LED_USER_ON		pLED1.write(ON);
#define LED_USER_OFF		pLED1.write(OFF);
#define LED_USER_TOGGLE		pLED1.write(1-pLED1.writeSta());
#define KEY_IN_STATE		strIO_Button_Value.bButtonState[eButton1]
#define LEG_BIKE_IN_STATE	strIO_Button_Value.bButtonState[eButton2]
#define OUT_IC_FIRE_STR		strRELAY_1
#define OUT_KEY_STR		strRELAY_2
#define OUT_XINHAN_STR		strRELAY_3
#define OUT_BELL		strBELL
#define OUT_LED_SIGNAL		strLED_1
#define OUT_TRUNK		strTRUNK
#define RS_485_MODE_RECIEVE 	pRS485_DIR.write(eFALSE)
#define RS_485_MODE_TRANSMIT 	pRS485_DIR.write(eTRUE)
#define TURNOFFBOARD 		pCutOffCircuit.write(eFALSE)
#define TURNONBOARD 		pCutOffCircuit.write(eTRUE)
#endif /* _Project_Function__H */
