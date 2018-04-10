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
* 25/03/2018: v1.0 1st build for Project 08 - KSK Company
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
#include "USART2_AppCall_Function.h"
#include "Project_Function.h"
#include "ComFunction.h"


/*Valiable For Control StepMotor*/
extern timer tP_StepA;

/**CREATE A NEW TASK
  1. Set TASK_PRIORITY
  2. Khai bao TASK_STACK_SIZE
  3. Tao Prototype
  4. Tao Task_Handle
  5. Khoi tao Task : OS_xTaskCreate
*/

#ifdef USE_OS
	/* Kernel Task priorities. */
	#define IO_KERNEL_TASK_PRIORITY			( tskIDLE_PRIORITY + 4 )
	/* Application Task priorities. */
	#define USER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )//20-08: 3
        /* Send buffer task prorities*/
        #define MAKE_BUFFER_TX_TASK_PRORITY             ( tskIDLE_PRIORITY + 1 )

	/* Kernel Task Stack size */
	#define IO_KERNEL_TASK_STACK_SIZE		( ( unsigned short ) 64 )
	/* The check task uses the sprintf function so requires a little more stack. */
	#define USER_TASK_STACK_SIZE			( ( unsigned short ) 128 )
        /* Kernel Task Stack size */
	#define MAKE_BUFFER_TX_TASK_STACK_SIZE		( ( unsigned short ) 64 )

	/* Extern prototype function */
	extern void vIO_Kernel_Task( void *pvParameters );
	extern void vUserTask( void *pvParameters );
        extern void vMakeBufferTXTask (void *pvParameters);
    
	/* Variable for Handler */
	xTaskHandle xIO_Task_Handle;
	enumbool xFlag_IO_Task_Still_Running = eTRUE, xFlag_IO_Task_Init_Done = eFALSE, xFlag_IO_Task_Process_Check = eTRUE;
        
	xTaskHandle xSensor_Task_Handle;
        
	xTaskHandle xUser_Task_Handle;
        
        xTaskHandle xMakeBufferTXTask_Handle;
        
	enumbool xFlag_User_Task_Still_Running = eTRUE, xFlag_User_Task_Init_Done = eFALSE, xFlag_User_Task_Process_Check = eTRUE;
    
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
        /* Create MakeBufferTX Task */
	OS_xTaskCreate(vMakeBufferTXTask, "MAKE_BUFFER_TX_TASK", MAKE_BUFFER_TX_TASK_STACK_SIZE, NULL, MAKE_BUFFER_TX_TASK_PRORITY, &xMakeBufferTXTask_Handle);

        /*Valiable For Control StepMotor*/
	vInit_STEP_MOTOR_Function();
	timer_set(&tP_StepA, 30 ,CLOCK_TYPE_US);

	/* Start the scheduler. */
	OS_vTaskScheduler();
    while(1);
#else /* Not use OS */
	while(1)
	{
	
	};
#endif /* USE_OS */
}






/**********************************************************************************/
void vApplicationIdleHook(void);

/** vApplicationIdleHook
  1. Thuc thi trong thoi gian ranh (trang thai Idle) cua he thong
  2. Chuyen xu ly cac ham cong tac, cac ham co tan so xu ly nhanh hon tan so hoat dong cac Task

 ** Noi dung thuc hien
  1.Ham goi thuc thi Control Step motor
  2.Ham goi thuc thi xu ly RX Data
**/

void vApplicationIdleHook(void)
{
  Control_step_motor();

  vComDataProcess_USART1();
  vComDataProcess_USART2();
  
}


