/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : main.c
* Author             : OS Team
* Date First Issued  : 20/09/2014
* Description        : 
********************************************************************************
* History: DD/MM/YY
* 20/09/2014: v0.1
* 07/07/2015: v0.2 Change Monitor task to 500ms frequency
* 10/08/2015: v0.3 Fix Name of Task
* 22/12/2017: v1.0 1st build for APhi Navigator project
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Include header file */
#include "Common.h"
#include "IO_Function.h"
#include "Time_Manage_Function.h"
#include "USART1_AppCall_Function.h"
#include "Project_Function.h"

#ifdef USE_OS
	/* Kernel Task priorities. */
	#define IO_KERNEL_TASK_PRIORITY			( tskIDLE_PRIORITY + 4 )
	/* Application Task priorities. */
	#define USER_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 )//20-08: 3

	/* Kernel Task Stack size */
	#define IO_KERNEL_TASK_STACK_SIZE		( ( unsigned short ) 64 )
	/* The check task uses the sprintf function so requires a little more stack. */
	#define USER_TASK_STACK_SIZE			( ( unsigned short ) 128 )

    /* The LED1 Task */
	#define LED1_TASK_STACK_SIZE			( ( unsigned short ) 64 )
    #define LED1_TASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
   

	/* Extern prototype function */
	extern void vIO_Kernel_Task( void *pvParameters );
	extern void vUserTask( void *pvParameters );
    
    /* Extern LED1 and LED2 Task*/
    void LED1_Task( void *pvParameters );
	void LED2_Task( void *pvParameters );
	
	/* Variable for Handler */
	xTaskHandle xIO_Task_Handle;
	enumbool xFlag_IO_Task_Still_Running = eTRUE, xFlag_IO_Task_Init_Done = eFALSE, xFlag_IO_Task_Process_Check = eTRUE;
	xTaskHandle xSensor_Task_Handle;
	xTaskHandle xUser_Task_Handle;
	enumbool xFlag_User_Task_Still_Running = eTRUE, xFlag_User_Task_Init_Done = eFALSE, xFlag_User_Task_Process_Check = eTRUE;
    
    /* Variable for Handler LED1 LED2 Task */
    xTaskHandle xLED1_Task_Handle, xLED2_Task_Handle;
	/* Application for free time MCU */
	void vApplicationIdleHook( void );
#endif

/*-----------------------------------------------------------*/
/* Main source */
void main(void)
{
	/* Init hardware module for MCU*/
	vMCU_Init_Hardware();
	/* Init module for project */
	vProject_Init();
	
	/* Depend on Choose with OS or Non OS, developer must arrange task and handle task */
#ifdef USE_OS /* Use OS */
	/* OS warning */
	#warning: [BUILD_WARNING]: configCPU_CLOCK_HZ base on the clock when Init CPU, configTICK_RATE_HZ base on the clock in the file FreeRTOSCongig.h
	
	/* Create Kernel Task */
	OS_xTaskCreate(vIO_Kernel_Task, "IO_KERNEL_TASK", IO_KERNEL_TASK_STACK_SIZE, NULL, IO_KERNEL_TASK_PRIORITY, &xIO_Task_Handle );
	/* Create Application Task */
	OS_xTaskCreate(vUserTask, "MAIN_USER_TASK", USER_TASK_STACK_SIZE, NULL, USER_TASK_PRIORITY, &xUser_Task_Handle);
	
    /* Create LED1 Task */
	//OS_xTaskCreate(LED1_Task, "LED1_Task", LED1_TASK_STACK_SIZE, NULL, LED1_TASK_PRIORITY, &xLED1_Task_Handle);
    
	/* Start the scheduler. */
	OS_vTaskScheduler();
    while(1);
#else /* Not use OS */
	while(1)
	{
	
	};
#endif /* USE_OS */
}

#ifdef USE_OS /* Use OS */
void LED1_Task( void *pvParameters )
{
    #define LED1_TASK_FREQUENCY				10	/* 10 ms */
    static timer pLED1Test;
    /* Delay before begin task */
	OS_vTaskDelay(50);
    /* Set prequency */
	portTickType xLastWakeTime;
	const portTickType xLED1_Task_Frequency = LED1_TASK_FREQUENCY;/* 10 tick slice */
	xLastWakeTime = xTaskGetTickCount();
    /* Init timer */
    timer_set(&pLED1Test,500,CLOCK_TYPE_MS);
    /* Task process */
	for(;;)
	{	
		/* Delay Exactly Frequency */
		OS_vTaskDelayUntil(&xLastWakeTime,xLED1_Task_Frequency);
        
        /* Check timer expired toggle LED, restart timer and toggle LED */
        if(timer_expired(&pLED1Test))
        {
            timer_restart(&pLED1Test);
            /* Toggle LED */
            LED_USER_TOGGLE;
            /* Print debug via UART1 */
            USART1_AppCall_SendString("[SYSTEM DEBUG]: Timer1 expired, led 1 toggle \r\n");
        }
        
    }
}
#endif /* USE_OS */
/**********************************************************************************/
void vApplicationIdleHook(void);
void vApplicationIdleHook(void)
{
}