/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : IO_Function_Profile.h
* Author             : Hai Dang
* Date First Issued  : 23-Sep-2014
* Description        : global function struct to manage and pointer function.

********************************************************************************
* History: DD/MM/YY
* 23/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_IO_Function_Profile__H
#define _IO_Function_Profile__H
/************************************************************************************************************/
/* Define IO Function*/
#define USE_IO_FUNCTION
	/* Uncomment to use LED to display, comment to disable */
	#define USE_LED
//		#define USE_LED_1	/* Led Signal */
//		#define USE_LED_2
//		#define USE_LED_3
//		#define USE_LED_4
	/* Uncomment to use Relay, comment to disable */
	#define USE_RELAY
//		#define USE_RELAY_1
//		#define USE_RELAY_2
//		#define USE_RELAY_3

		//#define USE_RELAY_1_FB
	/* Uncomment to use LED to display, comment to disable */
	#define USE_BUTTON_IO
	#define NUMBER_IO_BUTTON_USE 3
//		#define USE_BUTTON_IO_1
//			#define USE_BUTTON_IO_1_EXT
//		#define USE_BUTTON_IO_2
//			#define USE_BUTTON_IO_2_EXT
//		#define USE_BUTTON_IO_3
//          #define USE_BUTTON_IO_3_EXT
//		#define USE_BUTTON_IO_4
/************************************************************************************************************/
/* Detail IO function */
#ifdef USE_IO_FUNCTION
/* USE LED to monitor */
	#ifdef USE_LED
		/* Define for LED 1*/
		#ifdef USE_LED_1
			#define LED_1_PORT		GPIOC
			#define LED_1_PIN		GPIO_Pin_13
			#define LED_1_RCC_PORT	RCC_APB2Periph_GPIOC
			#define LED_1_CLK_CMD	RCC_APB2PeriphClockCmd
			#define LED_1_MODE		GPIO_Mode_Out_PP
			#define LED_1_SPEED		GPIO_Speed_50MHz
		#endif /* USE_LED_1 */
		/* Define for LED 2 */
		#ifdef USE_LED_2
			#define LED_2_PORT		GPIOC
			#define LED_2_PIN		GPIO_Pin_10
			#define LED_2_RCC_PORT	RCC_APB2Periph_GPIOC
			#define LED_2_CLK_CMD	RCC_APB2PeriphClockCmd
			#define LED_2_MODE		GPIO_Mode_Out_PP
			#define LED_2_SPEED		GPIO_Speed_50MHz
		#endif /* USE_LED_2 */
		/* Define for LED 3 */
		#ifdef USE_LED_3
			#define LED_3_PORT		GPIOC
			#define LED_3_PIN		GPIO_Pin_11
			#define LED_3_RCC_PORT	RCC_APB2Periph_GPIOC
			#define LED_3_CLK_CMD	RCC_APB2PeriphClockCmd
			#define LED_3_MODE		GPIO_Mode_Out_PP
			#define LED_3_SPEED		GPIO_Speed_50MHz
		#endif /* USE_LED_3 */
		/* Define for LED 4 */
		#ifdef USE_LED_4
			#define LED_4_PORT		GPIOC
			#define LED_4_PIN		GPIO_Pin_12
			#define LED_4_RCC_PORT	RCC_APB2Periph_GPIOC
			#define LED_4_CLK_CMD	RCC_APB2PeriphClockCmd
			#define LED_4_MODE		GPIO_Mode_Out_PP
			#define LED_4_SPEED		GPIO_Speed_50MHz
		#endif /* USE_LED_4 */
	#endif /*USE_LED*/
	
	#ifdef USE_RELAY
		#ifdef USE_RELAY_1
			#define RELAY_1_PORT		GPIOB
			#define RELAY_1_PIN			GPIO_Pin_3
			#define RELAY_1_RCC_PORT	RCC_APB2Periph_GPIOB
			#define RELAY_1_CLK_CMD		RCC_APB2PeriphClockCmd
			#define RELAY_1_MODE		GPIO_Mode_Out_PP
			#define RELAY_1_SPEED		GPIO_Speed_50MHz
		#endif /* USE_RELAY_1 */
		
		#ifdef USE_RELAY_1_FB
			#define RELAY_1_FB_PORT		GPIOA
			#define RELAY_1_FB_PIN		GPIO_Pin_3
			#define RELAY_1_FB_RCC_PORT	RCC_APB2Periph_GPIOA
			#define RELAY_1_FB_CLK_CMD	RCC_APB2PeriphClockCmd
			#define RELAY_1_FB_MODE		GPIO_Mode_IN_FLOATING
			#define RELAY_1_FB_SPEED	GPIO_Speed_50MHz
		#endif /* USE_RELAY_1_FB */

		#ifdef USE_RELAY_2
			#define RELAY_2_PORT		GPIOB
			#define RELAY_2_PIN			GPIO_Pin_6
			#define RELAY_2_RCC_PORT	RCC_APB2Periph_GPIOB
			#define RELAY_2_CLK_CMD		RCC_APB2PeriphClockCmd
			#define RELAY_2_MODE		GPIO_Mode_Out_PP
			#define RELAY_2_SPEED		GPIO_Speed_50MHz
		#endif /* USE_RELAY_2 */
		
		#ifdef USE_RELAY_3
			#define RELAY_3_PORT		GPIOA
			#define RELAY_3_PIN			GPIO_Pin_4
			#define RELAY_3_RCC_PORT	RCC_APB2Periph_GPIOA
			#define RELAY_3_CLK_CMD		RCC_APB2PeriphClockCmd
			#define RELAY_3_MODE		GPIO_Mode_Out_PP
			#define RELAY_3_SPEED		GPIO_Speed_50MHz
		#endif /* USE_RELAY_3 */

	#endif
	
	#ifdef USE_BUTTON_IO
		#ifdef USE_BUTTON_IO_1
			#define BUTTON_1_PORT		GPIOA
			#define BUTTON_1_PIN		GPIO_Pin_6
			#define BUTTON_1_RCC_PORT	RCC_APB2Periph_GPIOA
			#define BUTTON_1_CLK_CMD	RCC_APB2PeriphClockCmd
			#define BUTTON_1_MODE		GPIO_Mode_IPU
			#define BUTTON_1_SPEED		GPIO_Speed_50MHz
					    #ifdef  USE_BUTTON_IO_1_EXT
						#define EXT_BUTTON_1_LINE                   EXTI_Line5
//						#define EXT_BUTTON_1_EXT                    EXTI9_5_IRQn
//                        #define BUTTON_1_EXT_HANDLER                EXTI9_5_IRQHandler
						#define EXT_BUTTON_1_MODE                 	EXTI_Mode_Interrupt
						#define EXT_BUTTON_1_TRIGGER                EXTI_Trigger_Falling
						#define BUTTON_1_PORT_SOURCE                GPIO_PortSourceGPIOB
						#define BUTTON_1_PIN_SOURCE                 GPIO_PinSource5
					#endif /* USE_BUTTON_IO_1_EXT */
		#endif /* USE_BUTTON_IO_1 */
		#ifdef USE_BUTTON_IO_2
			#define BUTTON_2_PORT		GPIOB
			#define BUTTON_2_PIN		GPIO_Pin_8
			#define BUTTON_2_RCC_PORT	RCC_APB2Periph_GPIOB
			#define BUTTON_2_CLK_CMD	RCC_APB2PeriphClockCmd
			#define BUTTON_2_MODE		GPIO_Mode_IPU
			#define BUTTON_2_SPEED		GPIO_Speed_50MHz
					#ifdef  USE_BUTTON_IO_2_EXT
						#define EXT_BUTTON_2_LINE                   EXTI_Line8
//						#define EXT_BUTTON_2_EXT                    EXTI9_5_IRQn
//                        #define EXT_BUTTON_2_EXT_HANDLER            EXTI9_5_IRQHandler
						#define EXT_BUTTON_2_MODE                 	EXTI_Mode_Interrupt
						#define EXT_BUTTON_2_TRIGGER                EXTI_Trigger_Falling
						#define BUTTON_2_PORT_SOURCE                GPIO_PortSourceGPIOB
						#define BUTTON_2_PIN_SOURCE                 GPIO_PinSource8
					#endif /* USE_BUTTON_IO_1_EXT */
		#endif /* USE_BUTTON_IO_2 */
//        #define EXT_BUTTON_1_2_EXT_HANDLER            EXTI9_5_IRQHandler

		#ifdef USE_BUTTON_IO_3
			#define BUTTON_3_PORT		GPIOA
			#define BUTTON_3_PIN		GPIO_Pin_5
			#define BUTTON_3_RCC_PORT	RCC_APB2Periph_GPIOA
			#define BUTTON_3_CLK_CMD	RCC_APB2PeriphClockCmd
			#define BUTTON_3_MODE		GPIO_Mode_IPU
			#define BUTTON_3_SPEED		GPIO_Speed_50MHz
		#endif /* USE_BUTTON_IO_3 */
		#ifdef USE_BUTTON_IO_4
			#define BUTTON_4_PORT		GPIOA
			#define BUTTON_4_PIN		GPIO_Pin_3
			#define BUTTON_4_RCC_PORT	RCC_APB2Periph_GPIOA
			#define BUTTON_4_CLK_CMD	RCC_APB2PeriphClockCmd
			#define BUTTON_4_MODE		GPIO_Mode_IPU
			#define BUTTON_4_SPEED		GPIO_Speed_50MHz
		#endif /* USE_BUTTON_IO_4 */
	#endif /* USE_BUTTON_IO */
#endif /* USE_IO_FUNCTION */
#endif
