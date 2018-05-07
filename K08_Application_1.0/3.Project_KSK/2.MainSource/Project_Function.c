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
#include "ComFunction.h"

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
    .HW_Version = "HW_V1.0.0.0",        /* Current value */
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
        #ifdef USE_LED_1
		  USART1_AppCall_SendString("[SYSTEM DEBUG]: LED done! \r\n");
          extern IO_Struct pLED1;
          IO_LED1_Init(&pLED1);
		  pLED1.write(OFF);
		  pLED1.write(ON);
        #endif          
	
	/* Turn off LED */

	
	/* Config Button */
        #ifdef USE_BUTTON_IO_1
          extern IO_Struct pBUT_1;
          IO_BUTTON_1_Init(&pBUT_1);
        #endif
        
        #ifdef USE_BUTTON_IO_2
          extern IO_Struct pBUT_2;
          IO_BUTTON_2_Init(&pBUT_2);
        #endif
        
        #ifdef USE_BUTTON_IO_3
          extern IO_Struct pBUT_3;
          IO_BUTTON_3_Init(&pBUT_3);
        #endif
	
	/* Config Relay */
	
        #ifdef USE_RELAY_1
          extern IO_Struct pRELAY_1;
          IO_RELAY_1_Init(&pRELAY_1);
          pRELAY_1.write(OFF);
        #endif
        
	#ifdef USE_RELAY_2
          extern IO_Struct pRELAY_2;
          IO_RELAY_2_Init(&pRELAY_2);
          pRELAY_2.write(OFF);
        #endif
	
        
  	#ifdef USE_RELAY_3
          extern IO_Struct pRELAY_3;
          IO_RELAY_3_Init(&pRELAY_3);
          pRELAY_3.write(OFF);
        #endif      
	
	
	/* Init PWM function */
	USART1_AppCall_SendString("[SYSTEM DEBUG]: PWM Init success! \r\n");
        
	/* Init ADC function */

	/* Init EXTI encoder function */
	  EXTILine1_Config();
  	  EXTILine2_Config();
      EXTILine3_Config();
	  EXTILine4_Config();

	/* Init Stepmotor function */
	vInit_STEP_MOTOR_Function();
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










/*-----------------------------------------------------------*/
//-------------------FLASH USER FUNCTION---------------------//
/*-----------------------------------------------------------*/
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








/*-----------------------------------------------------------*/
//-------------------KSK CONFIG FUNCTION---------------------//
/*-----------------------------------------------------------*/
//---------ENCODER FUNCTION----------//
/*
  Author :  Le Bien
  Date   :  03/04/2018
  Edited :  09/04/2018 
  1. Read Encoder Pulse by EXTERNAL INTERRUPT
  2. Config : Line 1 - pinA1 | Line 2 - pinA2     by EXTILine1_Config | EXTILine2_Config
  3. EXTI function : EXTI1_IRQHandler | EXTI2_IRQHandler : define encoder pulse
  4. Read encoder value by vGetEncoderValue
*/

/* Encoder function for Axis X*/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
EXTI_InitTypeDef  EXTI_InitStructure;
 
uint16_t countA_X = 0, countB_X = 0 ; 
uint16   X_Axis_Encoder = 0;
int  cnt_rotary_X = 0;

enumbool bFlag_CntRotary_X=eFALSE;

void vGetEncoderValue_X(void)
{

  if (bFlag_CntRotary_X == eFALSE)    	X_Axis_Encoder = countA_X  + countB_X + 2400*cnt_rotary_X;         
  if (bFlag_CntRotary_X == eTRUE)		X_Axis_Encoder = countA_X  - 1 + 2400*cnt_rotary_X; 
       	      	              
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
	countA_X = (1200+countA_X+1)%1200;
	bFlag_CntRotary_X=eFALSE;
	if(	countA_X==0)
	{
		cnt_rotary_X++;
		bFlag_CntRotary_X=eTRUE;
	}
  }	  
  else
  {
  	countA_X = (1200+countA_X-1)%1200;
  	if(	countA_X==1199)				cnt_rotary_X--;
  }
 }
}

void EXTI2_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line2) != RESET)
 {
  EXTI_ClearITPendingBit(EXTI_Line2);
 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)== GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2))
   {
	countB_X = (1200+countB_X-1)%1200;
	}
  else
   countB_X = (1200+countB_X+1)%1200;
 }
}
/* Encoder function for Axis Y*/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
EXTI_InitTypeDef  EXTI_InitStructure;
 
uint16_t countA_Y = 0, countB_Y = 0 ; 
uint32   Y_Axis_Encoder = 0;
int  cnt_rotary_Y = 0;

enumbool bFlag_CntRotary_Y = eFALSE;
void vGetEncoderValue_Y(void)
{
  if (bFlag_CntRotary_Y==eFALSE)    	Y_Axis_Encoder = countA_Y  + countB_Y + 2400*cnt_rotary_Y;         
  if (bFlag_CntRotary_Y==eTRUE)		    Y_Axis_Encoder = countA_Y  - 1 + 2400*cnt_rotary_Y;
}
void EXTILine3_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
    
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTILine4_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
    
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI3_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line3) != RESET)
 {
  EXTI_ClearITPendingBit(EXTI_Line3);
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)== GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) )
  {  
	countA_Y = (1200+countA_Y+1)%1200;
	bFlag_CntRotary_Y=eFALSE;
	if(	countA_Y==0)
	{
		cnt_rotary_Y++;
		bFlag_CntRotary_Y=eTRUE;
	}
  }	  
  else
  {
  	countA_Y = (1200+countA_Y-1)%1200;
  	if(	countA_Y==1199)				cnt_rotary_Y--;
  }
 }
}

void EXTI4_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line4) != RESET)
 {
  EXTI_ClearITPendingBit(EXTI_Line4);
 if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)== GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
   {
	countB_Y = (1200+countB_Y-1)%1200;

	}
  else
   countB_Y = (1200+countB_Y+1)%1200;
 }
}

//---------CONTROL STEPMOTOR FUNCTION----------//
/*
  Author :  Le Bien
  Date   :  09/04/2018
  Edited :  09/04/2018 
  1. Control Stepmotor by Generate Pulse
  2. Config IO: pinA3 - CCW | pinA4 - DIR | pin A5 - ENABLE     by vInit_STEP_MOTOR_Function
  3. Control Stepmotor by Control_step_motor
*/
/*CONTROL STEP MOTOR OF AXIS X ON SLAVE1
1. Config Pin A3, Pin A4, Pin A5 corresponding with CCW,DIR,ENABLE.
2. Calculation Pulse control in void Calculate_Pulse(uint8 iIndex_avitme, uint8 iIndex_amicro).
3. Control Direction in void Control_Direction (void).
4. Control Pulse in void vMotorStepControl(void).
5. Check status of step motor in void vMotorStepControl_Status(uint8_t bDirection).
6  Calculation excisting position in void Determined_Position(uint8 iIndex_amicro).
7. Compear Calculation excisting position with Position Control for speed PULSE.
*/
GPIO_InitTypeDef  GPIO_InitStructure;
timer tP_StepA;
timer tP_ScanHole;
timer tP_ResetCutter;
/********************************************************/
/*AXIS X*/
uint8 cnt_stepmotor_X = 0; 
uint32 Cnt_Pulse_X    = 0;
/*AXIS Y*/
uint8 cnt_stepmotor_Y = 0; 
uint32 Cnt_Pulse_Y	  = 0;
/*AXIS Z*/
uint8 cnt_stepmotor_Z = 0; 
uint32 Cnt_Pulse_Z    = 0;

/********************************************************/
/*value for Axis X*/
uint32 X_Axis_PositionControl = 0;
uint32 X_Axis_PulseControl    = 0;
uint32 X_Axis_PositionGet     = 0;
uint8 X_Axis_Speed;
/*value for Axis Y*/
uint32 Y_Axis_PositionControl = 0;
uint32 Y_Axis_PulseControl    = 0;
uint32 Y_Axis_PositionGet     = 0;
uint8  Y_Axis_Speed;
/*value for Axis z*/
uint32 Z_Axis_PositionControl = 0;
uint32 Z_Axis_PulseControl    = 0;
uint32 Z_Axis_PositionGet     = 0;
uint8  Z_Axis_Speed;
/********************************************************/
/*value of 3 Axis control*/
#define  Value_vitmeX  		5
#define  Value_vitmeY  		5
#define  Value_vitmeZ  		5
/*********************************************************/
uint8   aValue_vitme[3]={Value_vitmeX , Value_vitmeY , Value_vitmeZ};

/* value microstepping*/
#define  value_micro1      1
#define  value_micro2      2
#define  value_micro8      8
#define  value_micro16     16
#define  value_micro32     32
#define  value_micro64     64
/********************************************************/
uint8 aValue_micro[6] = {value_micro1,value_micro2,value_micro8,value_micro16,value_micro32,value_micro64};
/* DATA RECIVE */
uint8_t X_Axis_bDirection;
uint8_t Y_Axis_bDirection;
uint8_t Z_Axis_bDirection;
/* STATUS DIRECTION STEPMOTOR */
uint8_t X_Axis_sDirection;
uint8_t Y_Axis_SDirection;
uint8_t Z_Axis_SDirection;
/********************************************************/

/*********************************************************/
/*DEFIN PIN FOR STEP MOTOR OF AXIS X*/
#define pin_X_CCW		 	GPIO_Pin_3
#define pin_X_DIR			GPIO_Pin_4
#define pin_X_ENABLE		GPIO_Pin_5
/*DEFIN PIN FOR STEP MOTOR OF AXIS Y*/
#define pin_Y_CCW		 	GPIO_Pin_15
#define pin_Y_DIR			GPIO_Pin_12
#define pin_Y_ENABLE		GPIO_Pin_11
/*DEFIN PIN FOR STEP MOTOR OF AXIS Z*/
#define pin_Z_CCW		 	GPIO_Pin_14
#define pin_Z_DIR			GPIO_Pin_13
#define pin_Z_ENABLE		GPIO_Pin_12
/**********************************************************/
enumbool bFlag_Compare = eTRUE;
enumbool	bFlag_GoHOME_X = eFALSE;
enumbool	bFlag_GoHOME_Y = eFALSE;
enumbool	bFlag_GoHOME_Z = eFALSE;	




void Calculate_Pulse(uint8 iIndex_avitme, uint8 iIndex_amicro) 
{
    X_Axis_PulseControl = ( X_Axis_PositionControl*200*aValue_micro[iIndex_amicro]/aValue_vitme[iIndex_avitme]);
	Y_Axis_PulseControl = ( Y_Axis_PositionControl*200*aValue_micro[iIndex_amicro]/aValue_vitme[iIndex_avitme]);
	Z_Axis_PulseControl = ( Z_Axis_PositionControl*200*aValue_micro[iIndex_amicro]/aValue_vitme[iIndex_avitme]);
}
/*Control Direction for step motor of axis X*/
void Control_Direction_X(void)
{  
 //if(BUFFER_FLAG_CONTROL.bFlag_GoHOME_X == eTRUE)
   if(bFlag_GoHOME_X == eTRUE)
	{
 	if(Cnt_Pulse_X < X_Axis_PulseControl)
       {
        	X_Axis_sDirection = MOTOR_STEP_FORWARD;
			//GPIO_SetBits(GPIOA, pin_X_DIR);
      
	   }
    if(Cnt_Pulse_X > X_Axis_PulseControl)
       {
			X_Axis_sDirection = MOTOR_STEP_REVERSE;
			//GPIO_ResetBits(GPIOA, pin_X_DIR);
      
       }

	if(Cnt_Pulse_X == X_Axis_PulseControl)
	  {
			X_Axis_sDirection = MOTOR_STEP_STOP;
	  }
	}
}
/*Control Direction for step motor of axis X*/
void Control_Direction_Y(void)
{
  //if(BUFFER_FLAG_CONTROL.bFlag_GoHOME_Y == eTRUE)
   if(bFlag_GoHOME_Y == eTRUE)
	{
    if( Cnt_Pulse_Y < Y_Axis_PulseControl)
       {
        	Y_Axis_SDirection = MOTOR_STEP_FORWARD;
			//GPIO_SetBits(GPIOA, pin_Y_DIR);
	   }
    if(Cnt_Pulse_Y > Y_Axis_PulseControl)
       {
			Y_Axis_SDirection = MOTOR_STEP_REVERSE;
			//GPIO_ResetBits(GPIOA, pin_Y_DIR);
       }

	if(Cnt_Pulse_Y == Y_Axis_PulseControl)
	  {
			Y_Axis_SDirection = MOTOR_STEP_STOP;
	  }
	}
}
/*Control Direction for step motor of axis X*/
void Control_Direction_Z(void)
{
//  if(BUFFER_FLAG_CONTROL.bFlag_GoHOME_Z == eTRUE)
	if(bFlag_GoHOME_Z == eTRUE)
	{
    if( Cnt_Pulse_Z < Z_Axis_PulseControl)
       {
        	Z_Axis_SDirection = MOTOR_STEP_FORWARD;
			//GPIO_SetBits(GPIOB, pin_Z_DIR);
	   }
    if(Cnt_Pulse_Z > Z_Axis_PulseControl)
       {
			Z_Axis_SDirection = MOTOR_STEP_REVERSE;
			//GPIO_ResetBits(GPIOB, pin_Z_DIR);
       }

	if(Cnt_Pulse_Z == Z_Axis_PulseControl)
	  {
			Z_Axis_SDirection = MOTOR_STEP_STOP;
	  }
	}
    
}
/*************************************************************/


#ifdef	USE_DEBUG_TIME_CATCH
	timer tP_catchtime;
	uint32	catch_time[100];
	uint8 iIndexcatch_time;
#endif

void vMotorStepControl(void)
{
	#ifdef	USE_DEBUG_TIME_CATCH
		timer_restart(&tP_catchtime);
	#endif
	
	if(timer_expired(&tP_StepA))
	{
		timer_restart(&tP_StepA);

		switch(X_Axis_bDirection)
		{ 
            case MOTOR_STEP_GOFORWARD:
				vMotorStepControl_Status(StepMotorX,X_Axis_bDirection);
			break;
   			case MOTOR_STEP_GOREVERSE:
				vMotorStepControl_Status(StepMotorX,X_Axis_bDirection);
			break;
			case MOTOR_STEP_ABLE:
                Calculate_Pulse(0,5);      
				Control_Direction_X();
				vMotorStepControl_Status(StepMotorX,X_Axis_sDirection);
            break;
			case MOTOR_STEP_BRAKE:
				X_Axis_PulseControl = X_Axis_Encoder;
			break;
			case MOTOR_STEP_DISABLE:
				GPIO_SetBits(GPIOA, pin_X_ENABLE);
			break;
			case MOTOR_STEP_STOP:
			break;
			default:
			break;
		}
 		switch(Y_Axis_bDirection)
		{
            case MOTOR_STEP_GOFORWARD:
				vMotorStepControl_Status(StepMotorY,Y_Axis_bDirection);
			break;
   			case MOTOR_STEP_GOREVERSE:
				vMotorStepControl_Status(StepMotorY,Y_Axis_bDirection);
			break;
			case MOTOR_STEP_ABLE:
				Calculate_Pulse(1,5); 
				Control_Direction_Y();
				vMotorStepControl_Status(StepMotorY,Y_Axis_SDirection);
            break;
			case MOTOR_STEP_BRAKE:
				Y_Axis_PulseControl = Y_Axis_Encoder;
			break;
			case MOTOR_STEP_DISABLE:
				GPIO_SetBits(GPIOA, pin_Y_ENABLE);
			break;
			case MOTOR_STEP_STOP:
		    break;
			default:
			break;
		}
		switch(Z_Axis_bDirection)
		{
			case MOTOR_STEP_GOFORWARD:
				vMotorStepControl_Status(StepMotorZ,Z_Axis_bDirection);
			break;
   			case MOTOR_STEP_GOREVERSE:
				vMotorStepControl_Status(StepMotorZ,Z_Axis_bDirection);
			break;
			case MOTOR_STEP_ABLE:
				Calculate_Pulse(2,5); 
				Control_Direction_Z();
				vMotorStepControl_Status(StepMotorZ,Z_Axis_SDirection);
            break;
			case MOTOR_STEP_BRAKE:
				Z_Axis_PulseControl = Z_Axis_SDirection;
			break;
			case MOTOR_STEP_DISABLE:
				GPIO_SetBits(GPIOB, pin_Z_ENABLE);
			break;
			case MOTOR_STEP_STOP:
			break;
			default:
			break;
		}
	}
	#ifdef	USE_DEBUG_TIME_CATCH
	  	catch_time[iIndexcatch_time]=timer_remaining(&tP_catchtime);
		iIndexcatch_time=(iIndexcatch_time+1)%100;
	#endif
}
/*Pointer function*/
void (*generate_pulse)(void);
uint8 *cnt_stepmotor;
uint32 *Cnt_Pulse;

void vMotorStepControl_Status(uint8 bStepMotor,uint8_t bDirection)
{
	switch(bStepMotor)
	{
		case StepMotorX:
			generate_pulse = &Generate_Pulse_X;
			cnt_stepmotor  = &cnt_stepmotor_X;
            Cnt_Pulse      = &Cnt_Pulse_X;

		break;
  		case StepMotorY:
			generate_pulse = &Generate_Pulse_Y;
			cnt_stepmotor  = &cnt_stepmotor_Y;
            Cnt_Pulse      = &Cnt_Pulse_Y;
           
		break;
		case StepMotorZ:
			generate_pulse = &Generate_Pulse_Z;
			cnt_stepmotor  = &cnt_stepmotor_Z;
            Cnt_Pulse      = &Cnt_Pulse_Z;
		
		break;
     	default:
		break;
       
	}
	switch(bDirection)
	{
		case MOTOR_STEP_GOFORWARD :
			GPIO_SetBits(GPIOB, pin_Z_DIR);
		    GPIO_SetBits(GPIOA, pin_X_DIR);
            GPIO_SetBits(GPIOA, pin_Y_DIR);
		    *cnt_stepmotor = (*cnt_stepmotor+1)%2;
			generate_pulse();
		break;	
        case MOTOR_STEP_GOREVERSE:
			GPIO_ResetBits(GPIOB, pin_Z_DIR);
			GPIO_ResetBits(GPIOA, pin_X_DIR);
			GPIO_ResetBits(GPIOA, pin_Y_DIR);
			*cnt_stepmotor = (*cnt_stepmotor+1)%2;   
			generate_pulse(); 
		break;
		case MOTOR_STEP_FORWARD:
			GPIO_SetBits(GPIOB, pin_Z_DIR);
		    GPIO_SetBits(GPIOA, pin_X_DIR);
            GPIO_SetBits(GPIOA, pin_Y_DIR);
			*cnt_stepmotor = (*cnt_stepmotor+1)%2;
			generate_pulse();
			if(*cnt_stepmotor == 0) 		*Cnt_Pulse += 1;
		break;
		case MOTOR_STEP_REVERSE:
			GPIO_ResetBits(GPIOB, pin_Z_DIR);
			GPIO_ResetBits(GPIOA, pin_X_DIR);
			GPIO_ResetBits(GPIOA, pin_Y_DIR);
			*cnt_stepmotor = (*cnt_stepmotor+1)%2;
			generate_pulse();
			if(*cnt_stepmotor == 1) 		*Cnt_Pulse -= 1;
		break;
     
		case MOTOR_STEP_STOP:
		break;
		default:
		break;
	}

}

/*CONTROL PULSE FOR AXIS X*/
void Generate_Pulse_X(void)
{   
	if(cnt_stepmotor_X == 0)
	{				
		GPIO_SetBits(GPIOA ,pin_X_CCW );
	}
	if(cnt_stepmotor_X == 1)	
	{			
		GPIO_ResetBits(GPIOA ,pin_X_CCW );
	}
}
/*CONTROL PULSE FOR AXIS Y*/
void Generate_Pulse_Y(void)
{   
	if(cnt_stepmotor_Y == 0)
	{				
		GPIO_SetBits(GPIOA ,pin_Y_CCW );
	}
	if(cnt_stepmotor_Y == 1)	
	{			
		GPIO_ResetBits(GPIOA ,pin_Y_CCW );
	}
}
/*CONTROL PULSE FOR AXIS Z*/
void Generate_Pulse_Z(void)
{   
	if(cnt_stepmotor_Z == 0)
	{				
		GPIO_SetBits(GPIOB ,pin_Z_CCW );
	}
	if(cnt_stepmotor_Z == 1)	
	{			
		GPIO_ResetBits(GPIOB ,pin_Z_CCW );
	}
}

void Determined_Position(uint8 iIndex_amicro)
{ 
 	X_Axis_PositionGet = ((5*X_Axis_Encoder)/(200*aValue_micro[iIndex_amicro]));
	Y_Axis_PositionGet = ((5*Y_Axis_Encoder)/(200*aValue_micro[iIndex_amicro]));

}
//Xu ly toc do khi co bFlag_Setspeed ==1
//bFlag_setspeed duoc xu ly o tren cac tang cao hon
void Compare_Position(void)
{ 
    Determined_Position(3);
	if( X_Axis_PositionControl >> X_Axis_PositionGet + 100)
	{
		timer_set(&tP_StepA,30,CLOCK_TYPE_US);	
    }
    else 
     	timer_set(&tP_StepA,70,CLOCK_TYPE_US);
}

void vInit_STEP_MOTOR_Function (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  /* config pin for control step motor Axis X and step motor Axis Y*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Write(GPIOA,0x0000);
/* config pin for control step motor Axis Z*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Write(GPIOB,0x0001);
 /*config pin for Endstop*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

 }
/*----------RESET HOME------------------- */
uint32 bFlag_Status_Axis =0 ;
void Z_HOME(void)
{   
	if(bFlag_Status_Axis==0)
	{
		Z_Axis_bDirection = MOTOR_STEP_GOFORWARD;
		timer_set(&tP_StepA, 20 ,CLOCK_TYPE_US);
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	}
	if (State_EndStop_Z_1[0] == eTRUE && bFlag_Status_Axis==0)
    {
		Z_Axis_bDirection = MOTOR_STEP_GOREVERSE;
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		timer_set(&tP_StepA, 70 ,CLOCK_TYPE_US);
		bFlag_Status_Axis = 1;
	}
	
    if (State_EndStop_Z_1[0]== eFALSE && bFlag_Status_Axis==1)	
	{
		Z_Axis_bDirection = MOTOR_STEP_GOFORWARD;
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		timer_set(&tP_StepA, 150 ,CLOCK_TYPE_US);
		bFlag_Status_Axis = 2;
	}

    if(State_EndStop_Z_1[0] == eTRUE && bFlag_Status_Axis==2)
    {
		 Z_Axis_bDirection = MOTOR_STEP_STOP;
		 bFlag_Status_Axis = 3;
    }
    
}
void X_HOME(void)
{   
  if(bFlag_Status_Axis == 3 )
	{
	X_Axis_bDirection = MOTOR_STEP_GOFORWARD;
	timer_set(&tP_StepA, 20 ,CLOCK_TYPE_US);
	vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	
	}
  if(State_EndStop_X_1[0] == eTRUE && bFlag_Status_Axis == 3)
	{
    X_Axis_bDirection = MOTOR_STEP_GOREVERSE; 
	vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
    timer_set(&tP_StepA, 70 ,CLOCK_TYPE_US);
	bFlag_Status_Axis =4;
	}
  if(State_EndStop_X_1[0]== eFALSE && bFlag_Status_Axis == 4)
	{
	X_Axis_bDirection = MOTOR_STEP_GOFORWARD;
	vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	timer_set(&tP_StepA, 150 ,CLOCK_TYPE_US);
	bFlag_Status_Axis = 5;
    }
  if(State_EndStop_X_1[0]== eTRUE && bFlag_Status_Axis == 5)
	{
	X_Axis_bDirection = MOTOR_STEP_STOP;
    bFlag_Status_Axis = 6;
	
	}
}
void Y_HOME(void)
{
  if(bFlag_Status_Axis == 6)
	{
	timer_set(&tP_StepA, 20 ,CLOCK_TYPE_US);
    Y_Axis_bDirection = MOTOR_STEP_GOFORWARD;
	}
  if(State_EndStop_Y_1[0]== eTRUE && bFlag_Status_Axis == 6)
	{
	Y_Axis_bDirection = MOTOR_STEP_GOREVERSE;
    timer_set(&tP_StepA, 70 ,CLOCK_TYPE_US);
	bFlag_Status_Axis = 7;
	}
  if(State_EndStop_Y_1[0]== eFALSE && bFlag_Status_Axis == 7)
	{
    Y_Axis_bDirection = MOTOR_STEP_GOFORWARD;
    timer_set(&tP_StepA, 150 ,CLOCK_TYPE_US);
	bFlag_Status_Axis = 8;
    }
  if(State_EndStop_Y_1[0]== eTRUE && bFlag_Status_Axis == 8)
	{
	Y_Axis_bDirection = MOTOR_STEP_STOP;
	bFlag_Status_Axis = 9;
	}
}


/*************************************RESET CUTTER******************************/
uint32 bFlag_Status_Sensor = 0;
#define X_Cutter  		100
#define Y_Cutter  		100
#define Z_Cutter  		50
#define Z_SafePosition 	100  

uint32 bFlag_ReleaseCutter = 0;
uint32 bFlag_GetCutter = 0;

void vInitReleaseCutter(void)
{
switch (bFlag_ReleaseCutter)
	{
	  case 0:	
	  //  Z_SafePosition = BUFFER_CONTROL_Z_AXIS.PositionControl;
		Z_Axis_PositionControl = Z_SafePosition;
		Z_Axis_bDirection= MOTOR_STEP_ABLE;
		vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_FORWARD);
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		bFlag_ReleaseCutter = 1;
	  break;

	  case 1:
		if(Cnt_Pulse_Z == Z_Axis_PulseControl)
		{
	  //  X_Cutter = BUFFER_CONTROL_X_AXIS.PositionControl;
		Z_Axis_PulseControl = 0;
		X_Axis_PositionControl = X_Cutter + 50;
		X_Axis_bDirection = MOTOR_STEP_ABLE;
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_FORWARD);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
		bFlag_ReleaseCutter = 2;
		}
	  break;
	  case 2:
		 if(Cnt_Pulse_X == X_Axis_PulseControl)
		 {
	  //   Y_Cutter = BUFFER_CONTROL_Y_AXIS.PositionControl;
		 X_Axis_PulseControl = 0;
		 Y_Axis_PositionControl = Y_Cutter;
		 Y_Axis_bDirection= MOTOR_STEP_ABLE;
		 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_FORWARD);
		 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		 vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
		 bFlag_ReleaseCutter = 3;
		 }
	  break;
	  case 3:
		 if(Cnt_Pulse_Y == Y_Axis_PulseControl)
		 {
	  //   Z_Cutter = BUFFER_CONTROL_Z_AXIS.PositionControl;
		 Y_Axis_PulseControl = 0;
		 Z_Axis_PositionControl = Z_Cutter;
		 Z_Axis_bDirection= MOTOR_STEP_ABLE;
		 vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_REVERSE);
		 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		 bFlag_ReleaseCutter = 4;
		 }
	  break;
	  case 4:
		 if(Cnt_Pulse_Z == Z_Axis_PulseControl)
		 {
	//   X_Cutter = BUFFER_CONTROL_X_AXIS.PositionControl;
		 Z_Axis_PulseControl = 0;
		 X_Axis_PositionControl = X_Cutter;
		 X_Axis_bDirection= MOTOR_STEP_ABLE;
		 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_REVERSE);
		 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		 vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
		 bFlag_ReleaseCutter = 5;
		 }
	  break;
	  case 5:
		 if(Cnt_Pulse_X == X_Axis_PulseControl)
		 {
	 //  Z_SafePosition = BUFFER_CONTROL_Z_AXIS.PositionControl;
		 X_Axis_PulseControl = 0;
		 Z_Axis_PositionControl = Z_SafePosition;
		 Z_Axis_bDirection= MOTOR_STEP_ABLE;
         Y_Axis_bDirection= MOTOR_STEP_STOP;
		 vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_FORWARD);
		 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		 bFlag_ReleaseCutter = 6; 
		 }
	  break;
	  case 6:
		 if(Cnt_Pulse_Z == Z_Axis_PulseControl)
		 { 
		 X_Axis_bDirection= MOTOR_STEP_STOP;
		 Y_Axis_bDirection= MOTOR_STEP_STOP;
		 Z_Axis_bDirection= MOTOR_STEP_STOP;
		 bFlag_ReleaseCutter = 7; 
		 }
	  break;
	  case 7:
		 bFlag_ReleaseCutter = 8; 
	  break;
	default:
	break;
   }
}
void vInitGetCutter(void)
{
  switch (bFlag_GetCutter)
{
	case 0:	
	//  Z_SafePosition = BUFFER_CONTROL_Z_AXIS.PositionControl;
	Z_Axis_PositionControl = Z_SafePosition;
	Z_Axis_bDirection= MOTOR_STEP_ABLE;
	vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_FORWARD);
	vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
	vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	bFlag_GetCutter = 1;
	break;
	case 1:
	if(Z_Axis_PulseControl == Cnt_Pulse_Z)
	{
	//   X_Cutter = BUFFER_CONTROL_X_AXIS.PositionControl;
	X_Axis_PositionControl = X_Cutter;
	X_Axis_bDirection = MOTOR_STEP_ABLE;
	vMotorStepControl_Status(StepMotorX, MOTOR_STEP_FORWARD);
	vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
	bFlag_GetCutter = 2;
	}
	break;
	case 2:
	if(X_Axis_PulseControl == Cnt_Pulse_X)
	{
	//   Y_Cutter = BUFFER_CONTROL_Y_AXIS.PositionControl;
	 Y_Axis_PositionControl = Y_Cutter;
	 Y_Axis_bDirection= MOTOR_STEP_ABLE;
	 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_FORWARD);
	 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
	 vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
	 bFlag_GetCutter = 3;
	}
	break;
	case 3:
	if(Y_Axis_PulseControl == Cnt_Pulse_Y)
	{
	//  Z_SafePosition = BUFFER_CONTROL_Z_AXIS.PositionControl;
	 Z_Axis_PositionControl = Z_Cutter;
	 Z_Axis_bDirection= MOTOR_STEP_ABLE;
	 vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_REVERSE);
	 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
	 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	  bFlag_GetCutter = 4;
	}
 	break;
	case 4:
	if(Z_Axis_PulseControl == Cnt_Pulse_Z)
	{
	//   X_Cutter = BUFFER_CONTROL_X_AXIS.PositionControl;
	 X_Axis_PositionControl = X_Cutter + 50;
	 X_Axis_bDirection= MOTOR_STEP_ABLE;
	 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_REVERSE);
	 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	 vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
	 bFlag_GetCutter = 5;

	}
	break;
	case 5:
	if(X_Axis_PulseControl == Cnt_Pulse_X )
	{
	//  Z_SafePosition = BUFFER_CONTROL_Z_AXIS.PositionControl;
	 Z_Axis_PositionControl = Z_SafePosition;
	 Z_Axis_bDirection= MOTOR_STEP_ABLE;
	 vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_FORWARD);
	 vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
	 vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
	 bFlag_GetCutter = 6;
	}  
	break;
	case 6:
	if(Z_Axis_PulseControl == Cnt_Pulse_Z)
	{
	  X_Axis_bDirection= MOTOR_STEP_STOP;
	  Y_Axis_bDirection= MOTOR_STEP_STOP;
	  Z_Axis_bDirection= MOTOR_STEP_STOP;
      bFlag_GetCutter = 7;
	}
	break;
	case 7:
		bFlag_ReleaseCutter = 8; 
	break;
  default:
  break;
 }
}
/*--------------------------------RUN TO POINT---------------------------------*/
uint32 bFlag_RunToPoint = 0;

#define X_Point  20
#define Y_Point  20
#
void Run_To_Point(void)
{ 
	switch(bFlag_RunToPoint)
	{
	   case 0:
	//  Z_SafePosition = BUFFER_CONTROL_Z_AXIS.PositionControl;
		Z_Axis_PositionControl = Z_SafePosition;
		Z_Axis_bDirection= MOTOR_STEP_ABLE;
		vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_FORWARD);
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		bFlag_RunToPoint = 1;
		break;
		case 1:
	   	if(Z_Axis_PulseControl == Cnt_Pulse_Z)
		{
		//X_Point =  BUFFER_CONTROL_Z_AXIS.PositionControl;
		X_Axis_PositionControl = X_Point;
		X_Axis_bDirection = MOTOR_STEP_ABLE;
		vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
		vMotorStepControl_Status(StepMotorY, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
		bFlag_RunToPoint = 2;
		}
        break;
		case 2:
		if(X_Axis_PulseControl == Cnt_Pulse_X)
		{
		//Y_Point =  BUFFER_CONTROL_Y_AXIS.PositionControl;
		Y_Axis_PositionControl = Y_Point;
		Y_Axis_bDirection= MOTOR_STEP_ABLE;
		vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
		vMotorStepControl_Status(StepMotorX, MOTOR_STEP_STOP);
		vMotorStepControl_Status(StepMotorZ, MOTOR_STEP_STOP);
		bFlag_RunToPoint = 3;
		}
		break;
		case 3:
		if(Y_Axis_PulseControl == Cnt_Pulse_Y)
		{
		X_Axis_bDirection = MOTOR_STEP_STOP;
		Y_Axis_bDirection = MOTOR_STEP_STOP;
		Z_Axis_bDirection = MOTOR_STEP_STOP;
		bFlag_RunToPoint = 4;
		}
		break;
	default:
	break;	
  } 
}

/*----------------------------------SCANHOLE-----------------------------------*/
#define X1_ScanHole  50
#define X2_ScanHole  400
#define Y1_ScanHole  50
#define Y2_ScanHole  400
uint32 bFlag_ScanHole_X = 0;
uint32 bFlag_ScanHole_Y = 0;
void ScanHole (void)
{
 	if(timer_expired(&tP_ScanHole))
	{
		timer_restart(&tP_ScanHole);
        switch (bFlag_ScanHole_X)
		{
        case 0:
           		Z_Axis_PositionControl = Z_SafePosition;
				Z_Axis_bDirection = MOTOR_STEP_ABLE;
                vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_FORWARD);
				vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
      			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
				bFlag_ScanHole_X =1;
       	break;	
		case 1:
 				if(Z_Axis_PulseControl == Cnt_Pulse_Z)
				{
				// X1_ScanHole = BUFFER_CONTROL_X_AXIS.PositionControl;
				Z_Axis_PulseControl = 0;
				X_Axis_PositionControl = X1_ScanHole;
				X_Axis_bDirection = MOTOR_STEP_ABLE;
                Z_Axis_bDirection = MOTOR_STEP_STOP;
				vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
				vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
      			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
				bFlag_ScanHole_X =2;
				}
		break;
		case 2:
				if(X_Axis_PulseControl == Cnt_Pulse_X)
				{
				// Y1_ScanHole = BUFFER_CONTROL_Y_AXIS.PositionControl;
				X_Axis_PulseControl =0;
				Y_Axis_PositionControl = Y1_ScanHole;
				Y_Axis_bDirection = MOTOR_STEP_ABLE;
				vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
				vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
      			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
				bFlag_ScanHole_X =3;
				}
		break;
        case 3:
				if(Y_Axis_PulseControl == Cnt_Pulse_Y)
				{
				X_Axis_bDirection = MOTOR_STEP_STOP;
				Y_Axis_bDirection = MOTOR_STEP_STOP;
				Z_Axis_bDirection = MOTOR_STEP_STOP;
				bFlag_ScanHole_X = 4;
				}
		break;
        case 4:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					Y_Axis_PositionControl = Y_Axis_PositionControl + 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 5;
					}
			break;
            default :
			break;
			}
        
		break;
        case 5:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 6;
			}
		break;
		case 6:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
                    X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl - 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;	
					}
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 7;
					}
			break;
            default :
			break;
			}
		break;
       	case 7:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 8;
			}
		break;
		case 8:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl + 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 9;
					}
			break;
            default :
			break;
			}
        
		break;
        case 9:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 10;
			}
		break;
		case 10:	
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl - 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 11;
					}
			break;
            default :
			break;
			}
		break;
       	case 11:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 12;
			}
		break;
		case 12:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl + 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 13;
					}
			break;
            default :
			break;
			}
        
		break;
        case 13:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 14;
			}
		break;
		case 14:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl - 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 15;
					}
			break;
            default :
			break;
			}
		break;
       	case 15:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 16;
			}
		break;
		case 16:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl + 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 17;
					}
			break;
            default :
			break;
			}
        
		break;
        case 17:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 18;
			}
		break;
		case 18:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl - 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 19;
					}
			break;
            default :
			break;
			}
		break;
       	case 19:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 20;
			}
		break;
		case 20:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl + 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;
					}	
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl + 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_FORWARD);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 22;
					}
			break;
            default :
			break;
			}
        
		break;
        case 21:
			if(Y_Axis_PulseControl == Cnt_Pulse_Y)
			{
            X_Axis_PositionControl = X_Axis_PositionControl + 30;
			X_Axis_bDirection = MOTOR_STEP_ABLE;
			vMotorStepControl_Status(StepMotorX,MOTOR_STEP_FORWARD);
			vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
			vMotorStepControl_Status(StepMotorZ,MOTOR_STEP_STOP);
			bFlag_ScanHole_X = 22;
			}
		break;
		case 22:
			switch(bFlag_ScanHole_Y)
			{
			case 0:
					X_Axis_bDirection = MOTOR_STEP_STOP;
					Y_Axis_PositionControl = Y_Axis_PositionControl - 80;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 1;
			break;
 			case 1:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
                    Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 2;	
					}
			break;
			case 2:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 3;
					}
			break;
			case 3:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 4;
					}
			break;	
			case 4:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 5;
					}
			break;
			case 5:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 6;
					}
			break;
			case 6:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 7;
					}
			break;
			case 7:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 8;
					}
			break;
			case 8:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 9;
					}
			break;
			case 9:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 10;
					}	
			break;
			case 10:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 11;
					}
			break;
			case 11:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 12;
					}
			break;
			case 12:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 13;
					}
			break;
			case 13:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 14;
					}
			break;
			case 14:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 15;
					}
			break;
			case 15:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 16;
					}
			break;
			case 16:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 17;
					}
			break;
			case 17:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 18;
					}
			break;
			case 18:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 30;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 19;
					}
			break;
			case 19:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 20;
					}
			break;
			case 20:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_PositionControl = Y_Axis_PositionControl - 50;
					Y_Axis_bDirection = MOTOR_STEP_ABLE;
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_REVERSE);
					vMotorStepControl_Status(StepMotorX,MOTOR_STEP_STOP);
					vMotorStepControl_Status(StepMotorY,MOTOR_STEP_STOP);
 					bFlag_ScanHole_Y = 21;
					}
			break;
			case 21:
					if(Y_Axis_PulseControl == Cnt_Pulse_Y)
					{
					Y_Axis_bDirection = MOTOR_STEP_STOP;
					bFlag_ScanHole_Y = 0;
					bFlag_ScanHole_X = 23;
					}
			break;
            default :
			break;
			}
		break;	
		case 23:
			X_Axis_bDirection = MOTOR_STEP_STOP;
			Y_Axis_bDirection = MOTOR_STEP_STOP;
			Z_Axis_bDirection = MOTOR_STEP_STOP;
		break;
		default:
		break;
		} 
    }
}



/*--------------------------Error_Process----------------------------------------*/
enumbool bFlag_Error_Process = eFALSE;
void vInit_Error_Process(void)
{
 if( State_EndStop_X_2[0] == eTRUE)
 { 	
  BUFFER_MOTOR_CONTROL_PROCESS.Error_Process = E_OverJourney;
  bFlag_Error_Process = eTRUE; 	
 }
 if( State_EndStop_Y_2[0] == eTRUE) 
{	
 BUFFER_MOTOR_CONTROL_PROCESS.Error_Process = E_OverJourney;
 bFlag_Error_Process = eTRUE;
}
 if( State_EndStop_Z_2[0] == eTRUE) 	
{
 BUFFER_MOTOR_CONTROL_PROCESS.Error_Process = E_OverJourney;
 bFlag_Error_Process = eTRUE;
}
// if(X_Axis_Encoder == X_Axis_Encoder) 	
//{
// BUFFER_MOTOR_CONTROL_PROCESS.Error_Process = E_OverLoad;
// bFlag_Error_Process = eTRUE;
//}
// if(Y_Axis_Encoder == Y_Axis_Encoder) 	
//{
// BUFFER_MOTOR_CONTROL_PROCESS.Error_Process = E_OverLoad;
// bFlag_Error_Process = eTRUE;
//}
}
#endif /* _Project_Function__C */
