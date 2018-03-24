/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : IO_Function.c
* Author             : Hai Dang
* Date First Issued  : 20-Sep-2014
* Description        : global function struct to manage and pointer function.

********************************************************************************
* History: DD/MM/YY
* 20/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_IO_Function__C
#define _IO_Function__C
#include "IO_Function.h"
/* Init LED function */
#ifdef USE_LED
	/* Manage Variable */
	IO_Struct pLED1;
	structIO_Manage_Output strLED_1;
	#ifdef USE_LED_1
	/* Init function */
	void IO_LED1_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		LED_1_CLK_CMD(LED_1_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = LED_1_MODE;
		GPIO_InitStructure.GPIO_Speed = LED_1_SPEED;
		GPIO_InitStructure.GPIO_Pin = LED_1_PIN;
    	GPIO_Init(LED_1_PORT,&GPIO_InitStructure);
		/* Init pointer function */
    	pIO->read = &IO_LED1_Read;
    	pIO->write = &IO_LED1_Write;
    	pIO->writeSta = &IO_LED1_WriteStatus;
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read function */
	enumbool IO_LED1_Read(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadInputDataBit(LED_1_PORT, LED_1_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* write function */
	void IO_LED1_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(LED_1_PORT,LED_1_PIN,(BitAction) BitVal);
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read write status function */
	enumbool IO_LED1_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(LED_1_PORT, LED_1_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	#endif /* LED 1 */
	#ifdef USE_LED_2
	/* Manage Variable */
	IO_Struct  pLED2;
	/* Init function */
	void IO_LED2_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		LED_2_CLK_CMD(LED_2_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = LED_2_MODE;
		GPIO_InitStructure.GPIO_Speed = LED_2_SPEED;
		GPIO_InitStructure.GPIO_Pin = LED_2_PIN;
		/* Init pointer function */
                GPIO_Init(LED_2_PORT,&GPIO_InitStructure);
                pIO->read = &IO_LED2_Read;
                pIO->write = &IO_LED2_Write;
                pIO->writeSta = &IO_LED2_WriteStatus;
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read function */
	enumbool IO_LED2_Read(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadInputDataBit(LED_2_PORT, LED_2_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* write function */
	void IO_LED2_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(LED_2_PORT,LED_2_PIN,(BitAction) BitVal);
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read write status function */
	enumbool IO_LED2_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(LED_2_PORT, LED_2_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	#endif /* LED 2 */
	#ifdef USE_LED_3
	/* Manage Variable */
	IO_Struct pLED3;
	/* Init function */
	void IO_LED3_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		LED_3_CLK_CMD(LED_3_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = LED_3_MODE;
		GPIO_InitStructure.GPIO_Speed = LED_3_SPEED;
		GPIO_InitStructure.GPIO_Pin = LED_3_PIN;
		/* Init pointer function */
                GPIO_Init(LED_3_PORT,&GPIO_InitStructure);
                pIO->read = &IO_LED3_Read;
                pIO->write = &IO_LED3_Write;
                pIO->writeSta = &IO_LED3_WriteStatus;
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read function */
	enumbool IO_LED3_Read(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadInputDataBit(LED_3_PORT, LED_3_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* write function */
	void IO_LED3_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(LED_3_PORT,LED_3_PIN,(BitAction) BitVal);
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read write status function */
	enumbool IO_LED3_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(LED_3_PORT, LED_3_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	#endif /* LED 3 */
	#ifdef USE_LED_4
	/* Manage Variable */
	IO_Struct pLED4;
	/* Init function */
	void IO_LED4_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		LED_4_CLK_CMD(LED_4_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = LED_4_MODE;
		GPIO_InitStructure.GPIO_Speed = LED_4_SPEED;
		GPIO_InitStructure.GPIO_Pin = LED_4_PIN;
		/* Init pointer function */
                GPIO_Init(LED_4_PORT,&GPIO_InitStructure);
                pIO->read = &IO_LED4_Read;
                pIO->write = &IO_LED4_Write;
                pIO->writeSta = &IO_LED4_WriteStatus;
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read function */
	enumbool IO_LED4_Read(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadInputDataBit(LED_4_PORT, LED_4_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* write function */
	void IO_LED4_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(LED_4_PORT,LED_4_PIN,(BitAction) BitVal);
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read write status function */
	enumbool IO_LED4_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(LED_4_PORT, LED_4_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	#endif /* LED 4 */
#endif
#ifdef USE_RELAY
	#ifdef USE_RELAY_1
	/* Manage Variable */
	IO_Struct pRELAY_1;
	structIO_Manage_Output strRELAY_1;
	/* Init function */
	void IO_RELAY_1_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		RELAY_1_CLK_CMD(RELAY_1_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = RELAY_1_MODE;
		GPIO_InitStructure.GPIO_Speed = RELAY_1_SPEED;
		GPIO_InitStructure.GPIO_Pin = RELAY_1_PIN;
		/* Init pointer function */
                GPIO_Init(RELAY_1_PORT,&GPIO_InitStructure);
                pIO->write = &IO_RELAY_1_Write;
                pIO->writeSta = &IO_RELAY_1_WriteStatus;
		#endif
	}
	/* write function */
	void IO_RELAY_1_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(RELAY_1_PORT,RELAY_1_PIN,(BitAction) BitVal);
		#endif
	}
	/* Read write status function */
	enumbool IO_RELAY_1_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(RELAY_1_PORT, RELAY_1_PIN));
		#endif
	}
	#endif /* USE_RELAY_1 */

	#ifdef USE_RELAY_2
	/* Manage Variable */
	IO_Struct pRELAY_2;
	structIO_Manage_Output strRELAY_2;
	/* Init function */
	void IO_RELAY_2_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		RELAY_2_CLK_CMD(RELAY_2_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = RELAY_2_MODE;
		GPIO_InitStructure.GPIO_Speed = RELAY_2_SPEED;
		GPIO_InitStructure.GPIO_Pin = RELAY_2_PIN;
		/* Init pointer function */
                GPIO_Init(RELAY_2_PORT,&GPIO_InitStructure);
                pIO->write = &IO_RELAY_2_Write;
                pIO->writeSta = &IO_RELAY_2_WriteStatus;
		#endif
	}
	/* write function */
	void IO_RELAY_2_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(RELAY_2_PORT,RELAY_2_PIN,(BitAction) BitVal);
		#endif
	}
	/* Read write status function */
	enumbool IO_RELAY_2_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(RELAY_2_PORT, RELAY_2_PIN));
		#endif
	}
	#endif /* USE_RELAY_2 */
	
	#ifdef USE_RELAY_3
	/* Manage Variable */
	IO_Struct pRELAY_3;
	structIO_Manage_Output strRELAY_3;
	/* Init function */
	void IO_RELAY_3_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		RELAY_3_CLK_CMD(RELAY_3_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = RELAY_3_MODE;
		GPIO_InitStructure.GPIO_Speed = RELAY_3_SPEED;
		GPIO_InitStructure.GPIO_Pin = RELAY_3_PIN;
		/* Init pointer function */
                GPIO_Init(RELAY_3_PORT,&GPIO_InitStructure);
                pIO->write = &IO_RELAY_3_Write;
                pIO->writeSta = &IO_RELAY_3_WriteStatus;
		#endif
	}
	/* write function */
	void IO_RELAY_3_Write(enumbool BitVal)
	{
		#ifdef USE_CMCIS_DRIVER
		GPIO_WriteBit(RELAY_3_PORT,RELAY_3_PIN,(BitAction) BitVal);
		#endif
	}
	/* Read write status function */
	enumbool IO_RELAY_3_WriteStatus(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadOutputDataBit(RELAY_3_PORT, RELAY_3_PIN));
		#endif
	}
	#endif /* USE_RELAY_3 */

	#ifdef USE_RELAY_1_FB
	void IO_RELAY_1_FB_Init(IO_Struct *pIO)
	{
		#ifdef USE_CMCIS_DRIVER
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		RELAY_1_FB_CLK_CMD(RELAY_1_FB_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = RELAY_1_FB_MODE;
		GPIO_InitStructure.GPIO_Speed = RELAY_1_FB_SPEED;
		GPIO_InitStructure.GPIO_Pin = RELAY_1_FB_PIN;
		/* Init pointer function */
                GPIO_Init(RELAY_1_FB_PORT,&GPIO_InitStructure);
                pIO->read = &IO_RELAY_1_FB_Read;
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	/* Read write status function */
	enumbool IO_RELAY_1_FB_Read(void)
	{
		#ifdef USE_CMCIS_DRIVER
		return(GPIO_ReadInputDataBit(RELAY_1_FB_PORT, RELAY_1_FB_PIN));
		#endif
		#ifdef USE_ONLY_REGISTER
		 
		#endif
	}
	#endif /* USE_RELAY_1_FB */
#endif /* USE_RELAY */

#ifdef USE_BUTTON_IO
	#ifdef USE_BUTTON_IO_1
	/* Manage Variable */
	IO_Struct pBUT_1;
	/* Init function */
	void IO_BUTTON_1_Init(IO_Struct *pIO)
	{
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		BUTTON_1_CLK_CMD(BUTTON_1_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = BUTTON_1_MODE;
		GPIO_InitStructure.GPIO_Speed = BUTTON_1_SPEED;
		GPIO_InitStructure.GPIO_Pin = BUTTON_1_PIN;
		/* Init pointer function */
                GPIO_Init(BUTTON_1_PORT,&GPIO_InitStructure);
                pIO->read = &IO_BUTTON_1_Read;
                
                #ifdef USE_BUTTON_IO_1_EXT
                NVIC_InitTypeDef NVIC_InitStructure; 
                        EXTI_InitTypeDef EXTI_InitStructure;
                        
                        /************************************************************/
                        GPIO_EXTILineConfig(BUTTON_1_PORT_SOURCE, BUTTON_1_PIN_SOURCE);
                        /* Config EXTI line 3 for Key Bike detect */
                        EXTI_InitStructure.EXTI_Line = EXT_BUTTON_1_LINE;
                        EXTI_InitStructure.EXTI_Mode = EXT_BUTTON_1_MODE;
                        EXTI_InitStructure.EXTI_Trigger = EXT_BUTTON_1_TRIGGER;
                        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
                        EXTI_Init(&EXTI_InitStructure); 
                        
                        /* Enable the key bike interrupt */
                        NVIC_InitStructure.NVIC_IRQChannel = EXT_BUTTON_1_EXT;
                        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
                        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
                        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                        NVIC_Init(&NVIC_InitStructure);
                #endif
	}
	/* Read write status function */
	enumbool IO_BUTTON_1_Read(void)
	{
		return(GPIO_ReadInputDataBit(BUTTON_1_PORT, BUTTON_1_PIN));
	}
	#endif /* USE_BUTTON_IO_1 */

	#ifdef USE_BUTTON_IO_2
	/* Manage Variable */
	IO_Struct pBUT_2;
	/* Init function */
	void IO_BUTTON_2_Init(IO_Struct *pIO)
	{
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		BUTTON_2_CLK_CMD(BUTTON_2_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = BUTTON_2_MODE;
		GPIO_InitStructure.GPIO_Speed = BUTTON_2_SPEED;
		GPIO_InitStructure.GPIO_Pin = BUTTON_2_PIN;
		/* Init pointer function */
                GPIO_Init(BUTTON_2_PORT,&GPIO_InitStructure);
                pIO->read = &IO_BUTTON_2_Read;
		
		#ifdef USE_BUTTON_IO_1_EXT
                NVIC_InitTypeDef NVIC_InitStructure; 
  		EXTI_InitTypeDef EXTI_InitStructure;
  		
  		/************************************************************/
	  	GPIO_EXTILineConfig(BUTTON_2_PORT_SOURCE, BUTTON_2_PIN_SOURCE);
	 	/* Config EXTI line 7 for Leg bike */
	 	EXTI_InitStructure.EXTI_Line = EXT_BUTTON_2_LINE;
	  	EXTI_InitStructure.EXTI_Mode = EXT_BUTTON_2_MODE;
	  	EXTI_InitStructure.EXTI_Trigger = EXT_BUTTON_2_TRIGGER;
	  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  	EXTI_Init(&EXTI_InitStructure); 
	  	
  		/* Enable the USB 5V DE IN interrupt */
  		NVIC_InitStructure.NVIC_IRQChannel = EXT_BUTTON_2_EXT;
  		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  		NVIC_Init(&NVIC_InitStructure);
    	#endif
	}
	/* Read write status function */
	enumbool IO_BUTTON_2_Read(void)
	{
		return(GPIO_ReadInputDataBit(BUTTON_2_PORT, BUTTON_2_PIN));
	}
	#endif /* USE_BUTTON_IO_2 */

	#ifdef USE_BUTTON_IO_3
	/* Manage Variable */
	IO_Struct pBUT_3;
	/* Init function */
	void IO_BUTTON_3_Init(IO_Struct *pIO)
	{
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		BUTTON_3_CLK_CMD(BUTTON_3_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = BUTTON_3_MODE;
		GPIO_InitStructure.GPIO_Speed = BUTTON_3_SPEED;
		GPIO_InitStructure.GPIO_Pin = BUTTON_3_PIN;
		/* Init pointer function */
                GPIO_Init(BUTTON_3_PORT,&GPIO_InitStructure);
                pIO->read = &IO_BUTTON_3_Read;
	}
	/* Read write status function */
	enumbool IO_BUTTON_3_Read(void)
	{
		return(GPIO_ReadInputDataBit(BUTTON_3_PORT, BUTTON_3_PIN));
	}
	#endif /* USE_BUTTON_IO_3 */
	
	#ifdef USE_BUTTON_IO_4
	/* Manage Variable */
	IO_Struct pBUT_4;
	/* Init function */
	void IO_BUTTON_4_Init(IO_Struct *pIO)
	{
		/* Init hardware function */
		GPIO_InitTypeDef GPIO_InitStructure;
		BUTTON_4_CLK_CMD(BUTTON_4_RCC_PORT, ENABLE);
		GPIO_InitStructure.GPIO_Mode = BUTTON_4_MODE;
		GPIO_InitStructure.GPIO_Speed = BUTTON_4_SPEED;
		GPIO_InitStructure.GPIO_Pin = BUTTON_4_PIN;
		/* Init pointer function */
                GPIO_Init(BUTTON_4_PORT,&GPIO_InitStructure);
                pIO->read = &IO_BUTTON_4_Read;
	}
	/* Read write status function */
	enumbool IO_BUTTON_4_Read(void)
	{
		return(GPIO_ReadInputDataBit(BUTTON_4_PORT, BUTTON_4_PIN));
	}
	#endif /* USE_BUTTON_IO_3 */
#endif /* USE_BUTTON_IO */
#endif