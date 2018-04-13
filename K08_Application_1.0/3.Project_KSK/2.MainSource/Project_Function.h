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
/* Prototype function */
#define MOTOR_1_DUTY 	vChangeDutyCycleOC1
#define MOTOR_2_DUTY 	vChangeDutyCycleOC2
/* Prototype static function main source use */
void vMCU_Init_Hardware( void );
void vProject_Init( void );
/* PWM function prototype */
void vInitPWMFunction(void);
void vChangeDutyCycleOC1(uint8_t bDutyPercent);
void vChangeDutyCycleOC2(uint8_t bDutyPercent);
/* Motor control */
void vMotorControl(uint8_t bDutyMotor, uint8_t bDirection);
/* DMA ADC function */
void vInit_DMA_ADC_Function(void);

/* EXTI ENCODER function*/
/*ENCODER OF AXIS X*/
void vGetEncoderValue_X(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTILine1_Config(void);
void EXTILine2_Config(void);
/*ENCODER OF AXIS Y*/
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTILine3_Config(void);
void EXTILine4_Config(void);
void vGetEncoderValue_Y(void);
/*Control Step_Motor*/
void vInit_STEP_MOTOR_Function (void);
void Generate_Pulse_X(void);
void Generate_Pulse_Y(void);
void Generate_Pulse_Z(void);
void Calculate_Pulse(uint8 iIndex_avitme, uint8 iIndex_amicro) ;
void Control_Pulse (void);
void vMotorStepControl(void);
void Determined_Position(uint8 iIndex_amicro);
void Compare_Position(void);
void Control_Direction_X(void);
void Control_Direction_Y(void);
void Control_Direction_Z(void);
void vMotorStepControl_Status(uint8 bStepMotor,uint8_t bDirection);

/* Flash Function prototype */
#define 	FW_FLASH_ADDR			0x08005000	/* 20KB bootloader */
#define 	USER_INFO_FLASH_ADDR	0x08004800	/* 18KB bootloader */
//#define     FLASH_PAGE_SIZE         0x800		//2048	/* 0x800 2K per page, 1K value 0x400*/
#define     FLASH_PAGE_SIZE         0x400		//2048	/* 0x800 2K per page, 1K value 0x400*//* STM32F103C8 */
#define     FLASH_WAIT_TIMEOUT      100000
void vFLASH_User_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length);
void vFLASH_User_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length);
void vFLASH_UpdateData(void);
/* Extern variabe */
extern structIO_Button strIO_Button_Value, strOld_IO_Button_Value;
extern IO_Struct pLED1, pCutOffCircuit;
extern structIO_Manage_Output strRELAY_1, strRELAY_2, strRELAY_3;
extern structIO_Manage_Output strLED_1, strBELL, strTRUNK;
extern IO_Struct pRS485_DIR;
/* extern adc value */
extern __IO uint16_t ADCConvertedValue;


#define LED_USER_1_ON			pLED1.write(ON);
#define LED_USER_1_OFF		pLED1.write(OFF);
#define LED_USER_1_TOGGLE		pLED1.write(1-pLED1.writeSta());
#define LED_USER_2_ON			pLED2.write(ON);
#define LED_USER_2_OFF		pLED2.write(OFF);
#define LED_USER_2_TOGGLE		pLED2.write(1-pLED2.writeSta());
#define BUTTON_1_STATE		strIO_Button_Value.bButtonState[eButton1]
#define BUTTON_2_STATE		strIO_Button_Value.bButtonState[eButton2]
#define LEG_BIKE_IN_STATE	strIO_Button_Value.bButtonState[eButton2]

#define OUT_LED_1		        strLED_1
#define OUT_LED_2               strLED_2

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
