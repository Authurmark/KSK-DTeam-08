/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Authenticate.h
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

#include "Common.h"
/* Define type of authen */
typedef enum
{ 
    eTypeAuthen1			= 1,
    eTypeAuthen2 			= 2,
    eTypeAuthen3			= 3,
	eTypeAuthen4			= 4,
	eTypeAuthen5			= 5,
	eTypeAuthen6			= 6,
	eTypeAuthen7			= 7,
	eTypeAuthen8			= 8,
	eTypeAuthenUN			= 0xff,
}eTypeAuthenticate;

typedef enum
{ 
	eStateEncript			= 1,
	eStateDecript			= 2,
}eStateAuthen;
/* Function Prototype */
void vEncript_Authenticate_Message(uint8* pSend_Message, eTypeAuthenticate eTypeAuthen, uint8 bSizeMSG);
void vDecript_Authenticate_Message(uint8* pReceive_Message, uint8 bSizeMSG);
eTypeAuthenticate vCreate_Random_Type_Authen(eTypeAuthenticate bOldAuthenValue);
void vChange_Value_Blank_Byte(uint8* pPayload, uint8 bSizeMSG);
/* Some function encript and decript */
void vChange_Authen_1(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen);
void vChange_Authen_2(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen);
void vChange_Authen_3(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen);
void vChange_Authen_4(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen);
void vChange_Authen_5(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen);
void vChange_Authen_6(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen);