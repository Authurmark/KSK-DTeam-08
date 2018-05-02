/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : UserTask.c
* Author             : OS Team
* Date First Issued  : 16/8/2014
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Project includes. */
#include "Common.h"
/*-----------------------------------------------------------*/
typedef enum
{ 
    eUserTask_State_Idle			= 1,
    eUserTask_State_CheckButton 		= 2,
    eUserTask_State_CheckPIR 			= 3,
    eUserTask_State_CheckRelayState 	        = 4,
    eMainTask_State_MAIN,
    eMainNullState                              =0xff,
}eUserState;
/* Funtion Prototype */
void vUserTask( void *pvParameters );


/* Init Parameter For System */
void vInitCutter(void);






/* Control Air Valves */
void vReleaseCutter(void);
void vGetCutter(void);


#define sCoil_1_GoDown       eTRUE         
#define sCoil_1_GoUp         eFALSE
#define sCoil_1_ResetHome    eFALSE

#define sCoil_2_GoDown       eFALSE         
#define sCoil_2_GoUp         eTRUE
#define sCoil_2_ResetHome    eTRUE

#define sCoil_3_GoDown       eFALSE         
#define sCoil_3_GoUp         eFALSE
#define sCoil_3_ResetHome    eTRUE

typedef enum {
    SpindleGoDown      = 0x01,
    SpindleGoUp        = 0x02,
    SpindleResetHome   = 0x03,
}state_control_air_value;

typedef struct{
  state_control_air_value       bProcess;
  enumbool                      sCoil_1;
  enumbool                      sCoil_2;
  enumbool                      sCoil_3;
}strcontrolairvalue;

strcontrolairvalue sControlAirValue;



void vSetAirVale(state_control_air_value bProcess);

void vStepChangeCutterProcess(void);
void vStepSpindleCheckHoleProcess(void);
