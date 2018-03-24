/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Authenticate.c
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
#include "Authenticate.h"
#include "Utilities.h"
/* Make Authenticate function */
void vEncript_Authenticate_Message(uint8* pSend_Message, eTypeAuthenticate eTypeAuthen, uint8 bSizeMSG)
{
	/* Base on Type authen to make type package */
	switch(eTypeAuthen)
	{
		case eTypeAuthen1:
			vChange_Authen_1(pSend_Message, bSizeMSG - 1, eStateEncript);
		break;
		case eTypeAuthen2:
			vChange_Authen_2(pSend_Message, bSizeMSG - 3, eStateEncript);
		break;
		case eTypeAuthen3:
			vChange_Authen_3(pSend_Message, bSizeMSG - 3, eStateEncript);
		break;
		case eTypeAuthen4:
			vChange_Authen_4(pSend_Message, bSizeMSG - 2, eStateEncript);
		break;
		case eTypeAuthen5:
			vChange_Authen_5(pSend_Message, bSizeMSG - 2, eStateEncript);
		break;
		case eTypeAuthen6:
			vChange_Authen_6(pSend_Message, bSizeMSG - 2, eStateEncript);
		break;
		case eTypeAuthen7:
			vChange_Authen_3(pSend_Message, bSizeMSG - 3, eStateEncript);
			vChange_Authen_1(pSend_Message, bSizeMSG - 3, eStateEncript);
		break;
		case eTypeAuthen8:
			vChange_Authen_4(pSend_Message, bSizeMSG - 3, eStateEncript);
			vChange_Authen_2(pSend_Message, bSizeMSG - 3, eStateEncript);
		break;
		default:
		break;
	}
}
/* Make Authenticate function */
void vDecript_Authenticate_Message(uint8* pReceive_Message, uint8 bSizeMSG)
{
	/* Get type authen */
	eTypeAuthenticate eTypeAuthen;
	eTypeAuthen = pReceive_Message[MAX_PW-2];
	/* Base on Type authen to parse type package */
	switch(eTypeAuthen)
	{
		case eTypeAuthen1:
			vChange_Authen_1(pReceive_Message, bSizeMSG - 1, eStateDecript);
		break;
		case eTypeAuthen2:
			vChange_Authen_2(pReceive_Message, bSizeMSG - 3, eStateDecript );
		break;
		case eTypeAuthen3:
			vChange_Authen_3(pReceive_Message, bSizeMSG - 3, eStateDecript );
		break;
		case eTypeAuthen4:
			vChange_Authen_4(pReceive_Message, bSizeMSG - 2, eStateDecript );
		break;
		case eTypeAuthen5:
			vChange_Authen_5(pReceive_Message, bSizeMSG - 2, eStateDecript);
		break;
		case eTypeAuthen6:
			vChange_Authen_6(pReceive_Message, bSizeMSG - 2, eStateDecript);
		break;
		case eTypeAuthen7:
			vChange_Authen_1(pReceive_Message, bSizeMSG - 3, eStateDecript);
			vChange_Authen_3(pReceive_Message, bSizeMSG - 3, eStateDecript);
		break;
		case eTypeAuthen8:
			vChange_Authen_2(pReceive_Message, bSizeMSG - 3, eStateDecript);
			vChange_Authen_4(pReceive_Message, bSizeMSG - 3, eStateDecript);
		break;
		default:
		break;
	}
}
/* Create random Authen Type */
void vChange_Value_Blank_Byte(uint8* pPayload, uint8 bSizeMSG)
{
	uint8_t i;
	i=0;
	while(i<=bSizeMSG)
	{
		if(pPayload[i]==0)
		{
			pPayload[i] = (uint8_t)gf_CreatRandomNumTimer();
		}
		i++;
	}
}
/* Create random Authen Type */
eTypeAuthenticate vCreate_Random_Type_Authen(eTypeAuthenticate bOldAuthenValue)
{
	#define MIN_RANDOMTYPE 1//1
	#define MAX_RANDOMTIME 8 //8
	uint32 uRandomType;
	uint32 uRandomNumber;
//	/* Reset variable */
//	uRandomType = bOldAuthenValue;
//	/* Get right value */
//	while(uRandomType==bOldAuthenValue)
//	{
//		uRandomNumber = gf_CreatRandomNumTimer();
//		uRandomType = uRandomNumber&0x00000007;
//		uRandomType+= 1;/* value 1->8 */
//	}
	uRandomType = bOldAuthenValue+1;
	if(uRandomType >= MAX_RANDOMTIME) uRandomType = MIN_RANDOMTYPE;
	return (eTypeAuthenticate)(uRandomType);
}
/* Change odd & even byte */
void vChange_Authen_1(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen)
{
	uint8_t bTempByte1;
	uint8_t i;
	/* Reset counter */
	i=0;
	while(i<=bSizeMSG)
	{
		bTempByte1 = pPayload[i];
		pPayload[i] = pPayload[i+1];
		pPayload[i+1] 	= bTempByte1;
		i+=2;
	}
}
/* Change 0->2, 1->0, 2->1 */
void vChange_Authen_2(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen)
{
	uint8_t bTempByte1, bTempByte2;
	uint8_t i;
	/* Reset counter */
	i=0;
	if(eActionAuthen == eStateEncript)
	{
		/* Change 0->2, 1->0, 2->1 */
		while(i<=bSizeMSG)
		{
			bTempByte1 = pPayload[i];
			bTempByte2 = pPayload[i+1];
			/* Change 0->2 */
			pPayload[i] = pPayload[i+2];
			/* Change 1->0 */
			pPayload[i+1] = bTempByte1;
			/* Change 2->1 */
			pPayload[i+2] = bTempByte2;
			i+=3;
		}
	}
	else if(eActionAuthen == eStateDecript)
	{
		/* Change 2->0, 0->1, 1->2 */
		while(i<=bSizeMSG)
		{
			bTempByte1 = pPayload[i+2];
			bTempByte2 = pPayload[i+1];
			/* Change 2->0 */
			pPayload[i+2] = pPayload[i];
			/* Change 0->1 */
			pPayload[i] = bTempByte2;
			/* Change 1->2 */
			pPayload[i+1] = bTempByte1;
			i+=3;
		}
	}
}
/* Change 0->1, 1->2, 2->0 */
void vChange_Authen_3(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen)
{
	uint8_t bTempByte1, bTempByte2;
	uint8_t i;
	/* Reset counter */
	i=0;
	if(eActionAuthen == eStateEncript)
	{
		/* Change 0->1, 1->2, 2->0 */
		while(i<=bSizeMSG)
		{
			bTempByte1 = pPayload[i+2];
			bTempByte2 = pPayload[i];
			/* Change 0->1 */
			pPayload[i] = pPayload[i+1];
			/* Change 1->2 */
			pPayload[i+1] = bTempByte1;
			/* Change 2->0 */
			pPayload[i+2] = bTempByte2;
			i+=3;
		}
	}
	else if(eActionAuthen == eStateDecript)
	{
		/* Change 1->0, 2->1, 0->2 */
		while(i<=bSizeMSG)
		{
			bTempByte1 = pPayload[i+1];
			bTempByte2 = pPayload[i+2];
			/* Change 1->0 */
			pPayload[i+1] = pPayload[i];
			/* Change 2->1 */
			pPayload[i+2] = bTempByte1;
			/* Change 0->2 */
			pPayload[i] = bTempByte2;
			i+=3;
		}
	}
}
/* Change head & tail */
void vChange_Authen_4(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen)
{
	uint8_t bTempByte1;
	uint8_t i;
	/* Reset counter */
	i=0;
	while(i<(bSizeMSG/2))
	{
		bTempByte1 = pPayload[i];
		pPayload[i] = pPayload[bSizeMSG-i];
		pPayload[bSizeMSG-i] = bTempByte1;
		i++;
	}
}
/* Change head & tail from 1/2 frame*/
void vChange_Authen_5(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen)
{
	uint8_t bTempByte1;
	uint8_t i;
	uint8_t bChangebyte;
	/* Reset counter */
	i=0;
	bChangebyte = bSizeMSG/2;
	while(i<bChangebyte)
	{
		bTempByte1 = pPayload[i];
		pPayload[i] = pPayload[bChangebyte+i];
		pPayload[bChangebyte+i] = bTempByte1;
		i++;
	}
}
/* Change head & tail from 1/2 frame*/
void vChange_Authen_6(uint8* pPayload, uint8 bSizeMSG, eStateAuthen eActionAuthen)
{
	uint8_t bTempByte1;
	uint8_t i;
	uint8_t bChangebyte;
	/* Reset counter */
	i=0;
	bChangebyte = bSizeMSG/2;
	while(i<bChangebyte)
	{
		bTempByte1 = pPayload[bChangebyte-i];
		pPayload[bChangebyte-i] = pPayload[bChangebyte+i];
		pPayload[bChangebyte+i] = bTempByte1;
		i++;
	}
}