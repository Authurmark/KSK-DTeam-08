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

/*----------------------------------------------------------------------------*/
//--------------DEFINE FOR PROTOTYPE & PWM FUNCTION PROTOTYPE----------------//
/*--------------------------------------------------------------------------*/
#define MOTOR_1_DUTY 	vChangeDutyCycleOC1
#define MOTOR_2_DUTY 	vChangeDutyCycleOC2

void vInitPWMFunction(void);
void vChangeDutyCycleOC1(uint8_t bDutyPercent);
void vChangeDutyCycleOC2(uint8_t bDutyPercent);

typedef enum {
    SPINDLE_RORATY = 0x01,
    SPINDLE_RESET  = 0x02,
}state_process;

typedef enum {
    SPINDLE_FORWARD = 0x01,
    SPINDLE_REVERSE = 0x02,
    SPINDLE_BREAK   = 0x03,
    SPINDLE_DISABLE = 0x04,
}state_DC_Spindle;

void vMotorControl( uint8 bDutyMotor,state_DC_Spindle bDirection);
void vInit_HomeEncoder(void);
void Spindle_Home(void);

extern uint32 bFlag_Status_Spindle;
extern uint16 Current_Value;

/*----------------------------------------------------------------------------*/
//------------------------Current_Measure_Value------------------------------//
/*--------------------------------------------------------------------------*/
void Current_Measure_Value(void);

/*----------------------------------------------------------------------------*/
//--------------DEFINE FOR PROTOTYPE & ADC FUNCTION PROTOTYPE----------------//
/*--------------------------------------------------------------------------*/

void vInit_DMA_ADC_Function(void);


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
/*--------------------------Error_Process----------------------------------------*/
void vInit_Error_Process(void);
extern enumbool bFlag_Error_Process;






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
#define LED_USER_ON			pLED1.write(ON);
#define LED_USER_OFF		pLED1.write(OFF);
#define LED_USER_TOGGLE		pLED1.write(1-pLED1.writeSta());
#define KEY_IN_STATE		strIO_Button_Value.bButtonState[eButton1]
#define LEG_BIKE_IN_STATE	strIO_Button_Value.bButtonState[eButton2]
#define OUT_IC_FIRE_STR		strRELAY_1
#define OUT_KEY_STR			strRELAY_2
#define OUT_XINHAN_STR		strRELAY_3
#define OUT_BELL			strBELL
#define OUT_LED_SIGNAL		strLED_1
#define OUT_TRUNK			strTRUNK
#define RS_485_MODE_RECIEVE 	pRS485_DIR.write(eFALSE)
#define RS_485_MODE_TRANSMIT 	pRS485_DIR.write(eTRUE)
#define TURNOFFBOARD 			pCutOffCircuit.write(eFALSE)
#define TURNONBOARD 			pCutOffCircuit.write(eTRUE)
#endif /* _Project_Function__H */
