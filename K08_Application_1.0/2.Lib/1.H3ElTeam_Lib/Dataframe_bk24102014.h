/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Hardware_Function_Profile.c
* Author             : OS Team
* Date First Issued  : 23/11/2013
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 23/11/2013: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef DATAFRAME_H
#define DATAFRAME_H
#include "Hardware_Function_Profile.h"
#include "PlatFormType.h"
/* It will match with the protocol */
typedef enum
{ 
    eDevice1	= 1,
    eDevice2 	= 2,
    eDevice3 	= 3,
    
    eUNDevice=0xff,
}eProtocol_Device;

typedef enum
{ 
    eFunction1	= 1,
    eFunction2 	= 2,
    eFunction3 	= 3,
    
    eUNFunction=0xff,
}eProtocol_Function;
typedef enum
{ 
    eUART_Debug		= 1,
    eUART_GSM	 	= 2,
    eUART_GPS 		= 3,
	eUART_LCD9bit	= 4,
	eUART_MCU1		= 5,
	eUART_MCU2		= 6,
	eUNUART			=0xff,	
}eUART_Function;

typedef enum
{ 
    eUART1		= 1,
    eUART2	 	= 2,
    eUART3 		= 3,
	eUART4		= 4,
	eUART5		= 5,
	eUNUARTNone	=0xff,	
}eUART_Port;

typedef enum
{ 
    eIOProtocol		= 1,
    eUARTProtocol 	= 2,
    eSPIProtocol 	= 3,
    
    eUNProtocol=0xff,
}eProtocol_Type;

typedef enum
{
	eTransmitSendACK		=	1,
	eTransmitSendNOACK		=	2,
	eTransmitReceive 		=	3,
	eTransmitACKWait		=	4,/* Wait to receive ACK signal*/
	eTransmitACKDone		=	5,/* When get data after send package,  base on time send and receive */
	eTransmitACKFail		=	6,
	eTransmitActive			=	7,/* At 1 time, with 1 protocol only 1 device can connect*/
	eTransmitSendComplete	=	8,
	eTransmitUN				=	0xff,
}eTransmitStatus;

#define APP_DATA_SIZE   (90)//(90)
#pragma pack(1)
typedef struct{
	eProtocol_Device            Protocol_Device;
	eProtocol_Function			Protocol_Function;
	eTransmitStatus				eTransStatus;
    uint16     PackageSize; /*from data field, maximum 90 bytes*/
    uint16     PacgakeCounter;
	char        *DataReceive;
	char        *DataSend;
	uint16                    	eTimesystem;
	uint16                    	eTimeSend;
	uint16                    	eTimeReceive;
    uint16                    	eTimeout;
}structProtocolDataFrame;
#pragma pack()
#pragma pack(1)
typedef struct _structManageProtocol
{
	structProtocolDataFrame *pProtocolFrame;
	uint16 uIdxReceive;
	uint16 uIdxReceiveLast;
	uint16 uACKTimeout;
}structManageProtocol;
#pragma pack()
#pragma pack(1)
typedef struct _structProtocolFrame
{
    eProtocol_Type				TypeProtocol;
	structProtocolDataFrame     strProtocolDataFrame;
}structProtocolFrame;
#pragma pack()

/* TypeDef for IO */
typedef enum
{ 
    eIOButton	= 1,
    eIOLED	 	= 2,
    eIOPWM	 	= 3,
    eIOADC	 	= 4,
	eIOLCD	 	= 5,
	eIORELAY 	= 6,
    eIOUN		= 0xff,
}eIO_Device;

/* TypeDef for Button */
typedef union
{
	int8_t val;
	struct{
	unsigned Button1    :1; 
	unsigned Button2    :1; 
	unsigned Button3    :1; 
	unsigned Button4    :1; 
	unsigned Button5    :1; 
	unsigned Button6    :1; 
	unsigned Button7    :1; 
	unsigned Button8    :1; 
	}bits;
}ButtonUnion;

typedef enum
{ 
    eButtonPress			= 1,
    eButtonRelease 			= 2,
    eButtonPressToRelease	= 3,
	eButtonReleaseToPress	= 4,
	eButtonLongPressT1		= 5,
	eButtonLongPressT2		= 6,
	eButtonNoise			= 7,
	eButtonHoldOn			= 8,
	eButtonHoldOff			= 9,	
	eButtonStateUN			= 0xff,
}eButtonState;

typedef enum
{ 
    eLow			= 0,
    eHigh 			= 1,
}eInputState;

typedef enum
{ 
    eButton1			= 0,
    eButton2 			= 1,
    eButton3			= 2,
	eButton4			= 3,
	eButton5			= 4,
	eButtonUN			= 0xff,
}eIndexButton;

typedef struct{
//    uint8_t     	bNumberButton;
    eButtonState	bValueButton[NUMBER_IO_BUTTON_USE];
}structIObutton;

typedef enum
{ 
	eButtonADCNone		= 0,
    eButtonADC1			= 1,
    eButtonADC2 		= 2,
    eButtonADC3			= 3,
	eButtonADC4			= 4,
	eButtonADC5			= 5,
	eButtonADC6			= 6,
	eButtonADC7			= 7,
	eButtonADC8			= 8,
	eButtonADC9			= 9,
	eButtonADC10		= 10,
	eButtonADC11		= 11,
	eButtonADC12		= 12,

	eButtonADCRelease	= 0xfe,
	eButtonADCUN		= 0xff,
}eIndexADCButton;

#pragma pack(1)
typedef struct _structButtonFrame
{
	structIObutton	*pValueIOButton;
	eIndexADCButton *pValueADCButton;
}structButtonFrame;
#pragma pack()

/* Struct, enum for LCD Frame */
typedef enum
{ 
    eLCDCommand			= 1,
    eLCDDataChar 		= 2,
	eLCDDataStringline	= 3,
	eLCDDataPic			= 4,
	eLCDUN				= 0xff,
}eCommandLCD;

#pragma pack(1)
typedef struct _structLCDFrame
{
	eCommandLCD	eCommand;
	uint8		LCDLine;
	uint8		Inv;
	char        *DataSend;
}structLCDFrame;
#pragma pack()

/* Struct for out IO */
#pragma pack(1)
typedef struct _structDataToglleFrame
{
	uint8  	NumberToggle1;
	uint16 	Frequency_1;	/* Step 10ms */
	uint8  	NumberToggle2;
	uint16 	Frequency_2;
	uint8  	NumberToggle3;
	uint16 	Frequency_3;
}structDataToglleFrame;
#pragma pack()

#define LED_ON 	0
#define LED_OFF 1
typedef enum
{
	eIO1	= 1,
	eIO2	= 2,
	eIO3	= 3,

	eIOAll	= 0xff,
}ePositionChange;

#pragma pack(1)
typedef struct _structDataOnOffFrame
{
	ePositionChange 	Position_change;
	enumbool 			eValue_1;
	enumbool 			eValue_2;
	enumbool 			eValue_3;
}structDataOnOffFrame;
#pragma pack()

/* Struct, enum for LED Frame */
typedef enum
{ 
    eLEDCommandOnOff				= 1,
    eLEDCommandToglleFrequency 		= 2,

	eLEDUN							= 0xff,
}eCommandLED;

#pragma pack(1)
typedef struct _structLEDFrame
{
	eCommandLED	eCommand;
	uint8       *DataLED;
}structLEDFrame;
#pragma pack()

/* Struct, enum for Relay Frame */
#define RELAY_ON 	1
#define RELAY_OFF 	0
typedef enum
{ 
    eRelayCommandOnOff					= 1,
    eRelayCommandToglleFrequency 		= 2,
	eRelayCommandToglleEndless	 		= 3,

	eRelayUN							= 0xff,
}eCommandRelay;

#pragma pack(1)
typedef struct _structRelayFrame
{
	eCommandRelay	eCommand;
	uint8        *DataRelay;
}structRelayFrame;
#pragma pack()
/* Time value struct */
typedef struct _struct_TimeValue
{
	uint16 Time_ms;
	uint16 Time_s;
	uint16 Time_m;
	uint16 Time_h;
}structTimeValue;
/* Main RFID project */
/* RF signal struct */
#pragma pack(1)
typedef struct _struct_RF_Signal_Status
{
	uint8 TagAddr;
	uint8 TagDetectedLostCount;
	uint8 UserDetectedCount;
	enumbool eTagLowBattery;
	enumbool eEnableEmergency;
	enumbool eEmergency;
	enumbool eFindNemo;
	enumbool UserDetected;
	enumbool TagDetected;
	enumbool EnableAlertFunc;
	enumbool UserArrived;
}struct_RF_Signal_Status;
#pragma pack()
#endif