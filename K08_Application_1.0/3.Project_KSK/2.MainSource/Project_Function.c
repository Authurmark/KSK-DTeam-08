/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Project_Function.c
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
#ifndef	_Project_Function__C
#define _Project_Function__C
/* Include global header file */
#include "Project_Function.h"
#include "Time_Manage_Function.h"
#include "USART1_AppCall_Function.h"
#include "USART2_AppCall_Function.h"
#include "USART3_AppCall_Function.h"
#include "IO_Function.h"
#include "IO_Kernel_Function.h"

/* Global Variable, system Information */
Struct_System_Information 		StrSystemInfo;
Struct_User_Information			StrUserInfo;
Struct_Manufactuer_Information	StrManufacturerInfo;
/* Information */
Struct_Flash_Config_Parameter	StrConfigPara =
{
	.ProductID = "FFFF",		/* Default value */
	.ProductName = "KSK_Machine",	/* Current value */
	.BL_Version = "BL_V2.5.0.3",	/* Current value */
    .FW_Version = "FW_V2.5.2.5",	/* Current value */
    .HW_Version = "HW_V1.0.0.0",/* Current value */
};
/* Config RAM Buff */
char StrConfig[FLASH_PAGE_SIZE];
Struct_Flash_Config_Parameter* pStrConfig;

#ifdef USE_WWDG
	/* Extern for watchdog */
	extern uint16_t bWatchdogCounter;
#endif /* USE_WWDG */

/* Flags for Wake up system */
volatile enumbool bFlag_RF_Int = eFALSE, bFlag_Key_Bike_Int = eFALSE, bFlag_Leg_Bike_Int = eFALSE, bFlag_ACCE_Int = eFALSE;
volatile enumbool bFlag_ACCE_Int_Sensor_Task = eFALSE;

/* Init clock, some function for MCU module */
void vMCU_Init_Hardware( void )
{
	static uint32_t lSystemClock;
	/* Init system clock */
	SystemInit();
	SystemCoreClockUpdate();
	lSystemClock = SystemCoreClock;
	/* Set the Vector Table base address at 0x08000000,  0x08005000 if have bootloader */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, FW_FLASH_ADDR );
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );	
	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	/* Setup hight resolution timer */
	vSetupTimerBaseFunction();
	/* Setup USART1 for debug */
	/* Init usart1 */
	USART1_AppCall_Init(&pUSART1);
	USART1_AppCall_SendString("[SYSTEM DEBUG]: FW User Mode!\r\n");
	USART1_AppCall_SendString("[SYSTEM DEBUG]: USART1 Init Success!\r\n");
        
        /* Init usart2 */
	USART2_AppCall_Init(&pUSART2);
	USART2_AppCall_SendString("[SYSTEM DEBUG]: FW User Mode!\r\n");
	USART2_AppCall_SendString("[SYSTEM DEBUG]: USART2 Init Success!\r\n");
	/* Config Clock AFIO, use for antenate function  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/* Init hardware module and system variale for project */
void vProject_Init()
{
	/* Project config */
	/* Disable Jtag to use PA15, PB3, PB4 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	USART1_AppCall_SendString("[SYSTEM DEBUG]: GPIO JTAG Disable done!\r\n");
	GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);
	USART1_AppCall_SendString("[SYSTEM DEBUG]: PD0 PD1 Remap done!\r\n");
    
	/* Config LED, for LED Tag Signal */
	extern IO_Struct pLED1;
	IO_LED1_Init(&pLED1);
	USART1_AppCall_SendString("[SYSTEM DEBUG]: LED done! \r\n");
	/* Turn off LED */
	pLED1.write(OFF);
	pLED1.write(ON);
	
	/* Config Button */
	extern IO_Struct pBUT_1, pBUT_2, pBUT_3;
	IO_BUTTON_1_Init(&pBUT_1);
	IO_BUTTON_2_Init(&pBUT_2);
        IO_BUTTON_3_Init(&pBUT_3);
	
	/* Config Relay */
	extern IO_Struct pRELAY_1, pRELAY_2, pRELAY_3;
	IO_RELAY_1_Init(&pRELAY_1);
	IO_RELAY_2_Init(&pRELAY_2);
	IO_RELAY_3_Init(&pRELAY_3);
	pRELAY_1.write(OFF);
	pRELAY_2.write(OFF);
	pRELAY_3.write(OFF);
	
	/* Init PWM function */
	vInitPWMFunction();
	USART1_AppCall_SendString("[SYSTEM DEBUG]: PWM Init success! \r\n");
	/* Init ADC function */
	vInit_DMA_ADC_Function();
	/* Init TIM3 encoder function */
	//vInit_TIM_ENCODER_Function();
	/* Update Flash Data */
	//vFLASH_UpdateData();
	/* Load config from flash and update */
	FLASH_Unlock();
	vFLASH_User_Read(0,USER_INFO_FLASH_ADDR,(uint32_t*)StrConfig,FLASH_PAGE_SIZE);
	FLASH_Lock();
	/* Check flash config already or not */
	if((StrConfig[0]!=0xFF)&&(StrConfig[1]!=0xFF)&&(StrConfig[2]!=0xFF)&&(StrConfig[3]!=0xFF))
	{
        /* Load firmware version and rewrite */
        pStrConfig = (Struct_Flash_Config_Parameter*)(StrConfig);
        /* Send Buf FW OLD version */
        USART1_AppCall_SendBuf("[SYSTEM DEBUG]: OLD FW Version: ",sizeof("[SYSTEM DEBUG]: OLD FW Version:"));
        USART1_AppCall_SendBuf(pStrConfig->FW_Version, SIZE_FW_VERSION+1);
        USART1_AppCall_SendString("\r\n");
        /* Send Buf FW Current version */
        USART1_AppCall_SendBuf("[SYSTEM DEBUG]: NEW FW Version: ",sizeof("[SYSTEM DEBUG]: OLD FW Version:"));
        USART1_AppCall_SendBuf(StrConfigPara.FW_Version, SIZE_FW_VERSION+1);
        USART1_AppCall_SendString("\r\n");
        /* Check Firmare version */
        if(memcmp(&StrConfigPara.FW_Version,pStrConfig->FW_Version, SIZE_FW_VERSION+1)!=eFALSE)
        {
            USART1_AppCall_SendString("[SYSTEM DEBUG]: Write New FW Version! \r\n");
            /* Only update firmware version */
            memcpy(pStrConfig->FW_Version, StrConfigPara.FW_Version, SIZE_FW_VERSION+1);
            memcpy(&StrConfigPara,StrConfig,sizeof(StrConfigPara));
            vFLASH_UpdateData();
        }
        USART1_AppCall_SendString("[SYSTEM DEBUG]: CHECK VERSION DONE! \r\n");
	}
	
    /* IWDG_Init: for reset system */
	/* Watchdog duoc reset boi timer dam bao cho viec van hanh */
	#ifdef USE_WATCHDOG
		#ifdef USE_IWDG
			IWDG_Init();
			IWDG_Start_Countdown();
		#endif /* USE_IWDG */
		#ifdef USE_WWDG
			WWDG_Init();
			WWDG_Start_Countdown();
			bWatchdogCounter = 0;
		#endif /* USE_WWDG */
	#endif /* USE_WATCHDOG */
            
	/* Delay before begin */
	vTimerBase_DelayMS(1500);
}
/* FLASH USER FUNCTION */
/* Read Flash USER Function */
void vFLASH_User_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{
    uint16_t i;
    switch (lun)
    {
        /* Internal FLASH */
        case 0:
            /* Read from USER_FLASH_ADDR */
            for( i=0; i < Transfer_Length; i+=4 )
            {
              Readbuff[i>>2] = ((vu32*)(Memory_Offset))[i>>2]; 
            }
        break;
        /* External FLASH */
        case 1:
        break;
        default:
        break;
    }
}
/* Write Flash USER Function */
void vFLASH_User_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{
    uint16_t i;
    switch (lun)
    {
    case 0:
        for( i = 0; i < Transfer_Length; i += FLASH_PAGE_SIZE )
        {
            if( FLASH_WaitForLastOperation(FLASH_WAIT_TIMEOUT) != FLASH_TIMEOUT )
            {
                FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
            } 	
            FLASH_ErasePage( Memory_Offset + i); 
        }			
        for( i = 0; i < Transfer_Length; i+=4 )
        { 
            if( FLASH_WaitForLastOperation(FLASH_WAIT_TIMEOUT) != FLASH_TIMEOUT )
            {
                FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR); 
            }      
            FLASH_ProgramWord( Memory_Offset + i , Writebuff[i>>2]); 
        }       
    break;
    case 1:
        break;
    default:
        break;
    }
}
void vFLASH_UpdateData(void)
{
    FLASH_Unlock();
    /* Copy data */
    memcpy(StrConfig,&StrConfigPara,FLASH_PAGE_SIZE);	
    vFLASH_User_Write(0,USER_INFO_FLASH_ADDR,(uint32_t*)StrConfig,FLASH_PAGE_SIZE);
    FLASH_Lock();
}

/* KSK machine */
void vInitPWMFunction(void)
{
	/* Config timer for PWM function */
	/* PWM will control DC motor */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	TIM_OCInitTypeDef          TIM_OCInitStructure;    

	TIM_TimeBaseStructure.TIM_Prescaler = 0;  
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   // 65535
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	/* TIM1 enable counter */
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	/* Init IO for PWM function */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure PB0 PB1 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void vChangeDutyCycleOC1(uint8_t bDutyPercent)
{
	TIM1->CCR1 = (bDutyPercent*65535)/100;
}
void vChangeDutyCycleOC2(uint8_t bDutyPercent)
{
	TIM1->CCR2 = (bDutyPercent*65535)/100;
}

#define MOTOR_FORWARD 	1
#define MOTOR_REVERSE 	2
#define MOTOR_STOP 		3
#define MOTOR_BRAKE 	4
void vMotorControl(uint8_t bDutyMotor, uint8_t bDirection)
{
	switch(bDirection)
	{
		case MOTOR_FORWARD:
			MOTOR_1_DUTY(bDutyMotor);
			MOTOR_2_DUTY(0);
		break;
		case MOTOR_REVERSE:
			MOTOR_1_DUTY(0);
			MOTOR_2_DUTY(bDutyMotor);
		break;
		case MOTOR_STOP:
            MOTOR_1_DUTY(0);
			MOTOR_2_DUTY(0);
		break;
		case MOTOR_BRAKE:
			MOTOR_2_DUTY(bDutyMotor);
			MOTOR_1_DUTY(bDutyMotor);
		break;
		default:
		break;
	}
}

/* Global variable */
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
__IO uint16_t ADCConvertedValue;
/* DMA ADC function */
void vInit_DMA_ADC_Function(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* Enable ADC1 and GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
  /* Configure PA.04 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/* Encoder function */
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
EXTI_InitTypeDef  EXTI_InitStructure;
 
uint16_t countA = 0, countB = 0 ; 
uint16   X_Axis_Encoder = 0;
int  cnt_rotary = 0;


void EXTILine1_Config(void);
void EXTILine2_Config(void);

void vGetEncoderValue(void)
{
  EXTILine1_Config();
  EXTILine2_Config();
  X_Axis_Encoder = countA  + countB;                       
}

void EXTILine1_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
    
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTILine2_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
    
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line1) != RESET)
 {
  EXTI_ClearITPendingBit(EXTI_Line1);
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)== GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) )
  {  
	countA = (1200+countA+1)%1200;
	if(	countA==0)					cnt_rotary++;
  }	  
  else
  {
  	countA = (1200+countA-1)%1200;
  	if(	countA==1199)				cnt_rotary--;
  }
 }
}

void EXTI2_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line2) != RESET)
 {
  EXTI_ClearITPendingBit(EXTI_Line2);
 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)== GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) )
   countB = (1200+countB-1)%1200;
  else
   countB = (1200+countB+1)%1200;
 }
}

/*CONTROL STEPMOTOR*/
GPIO_InitTypeDef  GPIO_InitStructure;
timer tP_StepA;
uint8 cnt_stepmotor; 
uint32 X_Axis_PositionControl = 5;
uint32 Cnt_Pulse = 0;
uint32 X_Axis_PulseControl = 0;
uint32 X_Axis_PositionGet = 0;
uint8 X_Axis_Speed;

/*value of 3 Axis control*/
#define  Value_vitmeX  		5
#define  Value_vitmeY  		10
#define  Value_vitmeZ  		10

#define ScrewShaft_X		0
#define ScrewShaft_Y		1
#define ScrewShaft_Z		2

uint8   aValue_vitme[3]={Value_vitmeX , Value_vitmeY , Value_vitmeZ};

/* value microstepping*/
#define  value_micro1      1
#define  value_micro2      2
#define  value_micro8      8
#define  value_micro16     16
#define  value_micro32     32
#define  value_micro64     64

uint8 aValue_micro[6] = {value_micro1,value_micro2,value_micro8,value_micro16,value_micro32,value_micro64};

/* CONTROL DIRECTION MOTOR*/
#define MOTOR_STEP_FORWARD 	1
#define MOTOR_STEP_REVERSE 	2
#define MOTOR_STEP_DISABLE 	3
#define MOTOR_STEP_BRAKE 	4


void Calculate_Pulse(uint8 iIndex_avitme, uint8 iIndex_amicro) 
{
    X_Axis_PulseControl = ( X_Axis_PositionControl*200*aValue_micro[iIndex_amicro]/aValue_vitme[iIndex_avitme]);
}

void Control_Pulse (void)
{
    if( Cnt_Pulse < X_Axis_PulseControl)
       {
        Cnt_Pulse += 1;
       }
    if(Cnt_Pulse > X_Axis_PulseControl)
       {
        Cnt_Pulse -= 1;
       }
    Cnt_Pulse = X_Axis_Encoder;

   }

void Generate_Pulse(void)
{
    vInit_STEP_MOTOR_Function();
    Control_Pulse ();
    Calculate_Pulse(ScrewShaft_X,2);
    vMotorStepControl(1);
    Compare_Position();
    if(timer_expired(&tP_StepA))
	{
		timer_restart(&tP_StepA);
		cnt_stepmotor=(cnt_stepmotor+1)%2;
		if(cnt_stepmotor==0)
		GPIO_SetBits(GPIOA ,GPIO_Pin_3 );
		else 
		GPIO_ResetBits(GPIOA ,GPIO_Pin_3 );
	}
    
}

void vMotorStepControl( uint8_t X_Axis_bDirection)
{
 switch(X_Axis_bDirection)
	{
		case MOTOR_STEP_FORWARD:
		     GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		break;
		case MOTOR_STEP_REVERSE:
			GPIO_SetBits(GPIOA, GPIO_Pin_4);
		break;
		case MOTOR_STEP_BRAKE:
            GPIO_SetBits(GPIOA, GPIO_Pin_5);
            GPIO_SetBits(GPIOA, GPIO_Pin_4);
		break;
		case MOTOR_STEP_DISABLE:
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
		break;
		default:
		break;
	}
}

void Determined_Position(uint8 iIndex_amicro)
{ 
 	X_Axis_PositionGet = ((5*X_Axis_Encoder)/(200*aValue_micro[iIndex_amicro]));
}
void Compare_Position(void)
{ 
    Determined_Position(2);
	if( X_Axis_PositionControl >> X_Axis_PositionGet)
	{
		timer_set(&tP_StepA,30,CLOCK_TYPE_US);	
    }
    else 
     	timer_set(&tP_StepA,70,CLOCK_TYPE_US);
}
 
void vInit_STEP_MOTOR_Function (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Write(GPIOA,0x0000);
}

#endif /* _Project_Function__C */
