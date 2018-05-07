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
        
        /* Init usart3 */
	USART3_AppCall_Init(&pUSART3);
	USART3_AppCall_SendString("[SYSTEM DEBUG]: FW User Mode!\r\n");
	USART3_AppCall_SendString("[SYSTEM DEBUG]: USART3 Init Success!\r\n");
        
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
          extern IO_Struct pLED1;
          IO_LED1_Init(&pLED1);
          USART1_AppCall_SendString("[SYSTEM DEBUG]: LED done! \r\n");
          /* Turn off LED */
          pLED1.write(OFF);
          pLED1.write(ON);
        #endif          

	
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
	
        /* Init Detect Thread Hole function*/
        vInit_DetectHole();
        
        /* Init Linear Scale Function*/
        vInitLinearScale();
//        /* Init FeedBack Detect OverTime */
//        vInitFeedBackDetectOverTime();
        /*Init Detect LED function*/
      	vInit_LED();
		vInit_LED_DeBug();
  		/*Init Detect BUTTON function*/
		vInit_BUTTON();
		/*Control Air Valve*/
		vInit_SetAirVale();
		/*interrupt for pausebutton*/
		EXTILine15_Config();
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

//---------DETECT THREAD HOLE FUNCTION----------//
#define Pin_LaserSensor_1       GPIO_Pin_8
#define Pin_LaserSensor_2       GPIO_Pin_9
void vInit_DetectHole()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = Pin_LaserSensor_1 | Pin_LaserSensor_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

uint8 Cnt_TimeHold_LS1;
uint8 Cnt_TimeHold_LS2;
   
enumbool bLaserSensor_1()
{
  //Scan status pin
  if((GPIO_ReadInputDataBit(GPIOB, Pin_LaserSensor_1))==eTRUE)                 Cnt_TimeHold_LS1=Cnt_TimeHold_LS1+1;                    //1 stick = 10ms
  if((GPIO_ReadInputDataBit(GPIOB, Pin_LaserSensor_1))==eFALSE)                Cnt_TimeHold_LS1=0;
 
  //Define status pin
  if(Cnt_TimeHold_LS1>=2)               return eTRUE;   
  if(Cnt_TimeHold_LS1<2)                return eFALSE;
}

enumbool bLaserSensor_2()
{
  //Scan status pin
  if((GPIO_ReadInputDataBit(GPIOB, Pin_LaserSensor_2))==eTRUE)                 Cnt_TimeHold_LS2=Cnt_TimeHold_LS2+1;                    //1 stick = 10ms
  if((GPIO_ReadInputDataBit(GPIOB, Pin_LaserSensor_2))==eFALSE)                Cnt_TimeHold_LS2=0;
 
  //Define status pin
  if(Cnt_TimeHold_LS2>=2)               return eTRUE;   
  if(Cnt_TimeHold_LS2<2)                return eFALSE;
}

enumbool bDetectThreadHole()
{
  if(bLaserSensor_1())
//    if(bLaserSensor_2())
      return eTRUE;
  
  else
    return eFALSE;
}



//---------ENCODER FUNCTION----------//
/* In Master KIT : Read Linear Scale */




/*
  Author :  Le Bien
  Date   :  03/04/2018
  Edited :  09/04/2018 
  1. Read Encoder Pulse by EXTERNAL INTERRUPT
  2. Config : Line 1 - pinA1 | Line 2 - pinA2     by EXTILine1_Config | EXTILine2_Config
  3. EXTI function : EXTI1_IRQHandler | EXTI2_IRQHandler : define encoder pulse
  4. Read encoder value by vGetEncoderValue
*/

strBuffer_linearscale Buffer_LinearScale;

void vInitLinearScale()
{
  EXTILine7_Config();
  vInit_LinearScale_BPulse();
}
void vGetLinearScaleValue(void)
{
    //calculate pulse --> Spindle position value
    //godown => value--  
	Buffer_LinearScale.spindle_position = ( Buffer_LinearScale.pulse_cnt_LinearScale / 100);    
}
void EXTILine7_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
    
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
 

void vInit_LinearScale_BPulse()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void EXTI9_5_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line7) != RESET)
 {
  EXTI_ClearITPendingBit(EXTI_Line7);
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)== GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
	{
    Buffer_LinearScale.pulse_cnt_LinearScale ++;
	}
  else
	{
    Buffer_LinearScale.pulse_cnt_LinearScale --;
	}
 }
}



/*----------------------------Control Air Valve-------------------------------*/

void vInit_SetAirVale(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_ResetBits(GPIOB,GPIO_Pin_3);
  GPIO_ResetBits(GPIOB,GPIO_Pin_4);
  GPIO_ResetBits(GPIOB,GPIO_Pin_5); 
}

/*---------------------------CONFIG GPIO FOR LED 24V------------------------------*/
void vInit_LED(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}
/*---------------------------CONFIG GPIO FOR LED DEBUG------------------------------*/
void vInit_LED_DeBug(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOA,GPIO_Pin_11);
  GPIO_ResetBits(GPIOA,GPIO_Pin_12);
  GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}
/*---------------------------CONFIG GPIO FOR STOPBUTTON------------------------------*/
void vInit_BUTTON(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}
/*---------------------------CONFIG GPIO FOR PAUSEBUTTON------------------------------*/
void EXTILine15_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
    
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
 if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
	 EXTI_ClearITPendingBit(EXTI_Line15);
	 GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}
}


#endif /* _Project_Function__C */
