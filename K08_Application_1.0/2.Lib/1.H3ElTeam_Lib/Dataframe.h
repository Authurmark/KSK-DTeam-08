/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : Hardware_Function_Profile.c
* Author             : Hai Dang, Anh Hoang
* Date First Issued  : 23/11/2013
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 23/11/2013: 	V0.1
* 16/03/2015:	v1.0
* 16/03/2015:	v1.1 Adding include "IO_Function_Profile.h"
* 09/07/2015:	v1.2 Adding RF address soft and hard
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

#include "PlatFormType.h"
#include "IO_Function_Profile.h"
#define DATAFRAME_NUMBER_IO_BUTTON	NUMBER_IO_BUTTON_USE

/* Enum button state */
typedef enum
{ 
    eButtonPress			= 1,
    eButtonRelease 			= 2,
	eButtonLongPressT1		= 3,
	eButtonLongPressT2		= 4,
	eButtonHoldOn			= 5,
	eButtonDoubleClick		= 6,
	eButtonSingleClick		= 7,
	eButtonTripleClick		= 8,
	eButtonHoldOff			= 9,
    eButtonHoldOffLongT1	= 10,
    eButtonHoldOffLongT2	= 11,
    eButtonHoldOffLong  	= 12,
	eButtonStateUN			= 0xff,
}eButtonState;

/* Enum IO button Logic state */
typedef enum
{ 
    eLogicLow			= 0,
    eLogicHigh 			= 1,
}eInputState;

/* Enum button index */
typedef enum
{ 
    eButton1			= 0,
    eButton2 			= 1,
    eButton3			= 2,
	eButton4			= 3,
	eButton5			= 4,
	eButtonUN			= 0xff,
}eIndexButton;

/* Struct manage button */
__packed
typedef struct{
    eButtonState	bButtonState[DATAFRAME_NUMBER_IO_BUTTON];/* Button state */
	uint16_t		bButtonTime[DATAFRAME_NUMBER_IO_BUTTON];/* Time to hole */
	enumbool 		bFlagNewButton;
}structIO_Button;

/* Define for use with Relay */
#define RELAY_ON 	1
#define RELAY_OFF 	0

/* Struct manage Output */
__packed
typedef struct{
	uint32		uFrequency;		/* Frequency Toggle 	*/
	uint32		uTimeLine;		/* Time Cycle 			*/
	uint32		uCountToggle;	/* Number Toggle 		*/
	uint32		uCycleCounter;	/* Time Cycle Counter	*/
	enumbool	bStartState;	/* Start State 			*/
	enumbool	bEndState;		/* End State 			*/
	enumbool	bCurrentProcess;/* Flag Process 		*/
	enumbool	bFlagStart;		/* Flag start 			*/
}structIO_Manage_Output;


/* Sensor Task */
#define ACCE_BIKE_FALL_VALUE 	12
#define ACCE_BALANCE_VALUE 		        2//2 org 
#define ACCE_VIBRATE_VALUE_LOW 		    3//3 org
#define ACCE_VIBRATE_VALUE_STRONG 		7//3 org

typedef enum
{
	eAcceBikeStand		= 1,
	eAcceBikeTouch		= 2,
	eAcceBikeMove		= 3,
	eAcceBikeFall		= 4,
	eAcceBikeLean		= 5,
	eAcceBikeUN			= 0xff,
}eAcceBikeStatus;

typedef enum
{
	eAcceBalance	    = 1,
	eAcceVibrate_Low    = 2,
    eAcceVibrate_Strong	= 3,
	eAcceUN			    = 0xff,
}eAcceStatus;
#pragma pack(1)
typedef struct{
	double 		an_x;
	double 		an_y;
	double 		an_z;
	eAcceStatus eAcceState;
}structAccelometerData;
#pragma pack()

/******************************************/
/******************************************/
/* Adding 27-06-2015 */
typedef enum _enumRFAppFrame_TAGRFCommand
{
	eTAGRFCommandStatus 	= 1,
	eTAGRFCommandFindBike 	= 2,
	eTAGRFCommandQuerry 	= 3,
	eTAGRFCommandRunning 	= 4,
	eTAGRFCommandTrunkActive 		= 5,
	eTAGRFCommandChangeMode		= 6,
	eTAGRFCommandALarm		= 7,
	/* For config main */
	eTAGRFCommandChangeModeConfigMain = 60,
	eTAGRFCommandConfigParameterMain = 61,
	eTAGRFCommandEndParameterMain = 62,
	eTAGRFCommandNull 		= 0xff,
}enumRFAppFrame_TAGRFCommand;

typedef enum _enumModeBikeState
{
	eModeBikeMODE1			= 1,/* User Tag    */
	eModeBikeMODE2			= 2,/* Replace Key */
	
	eModeBikeNull 		= 0xff,
}enumModeBikeState;

typedef enum _enumBikeState
{
	eBikeStateRunning		= 1,
	eBikeStateStand			= 2,
	eBikeStateAlarm			= 3,
	eBikeStateLowPower		= 4,
	eBikeStateWaitTagRunCommand	= 5,
	eBikeStateDeepPower		= 6,
	eBikeStateConfig		= 7,
	
	eBikeStateNull 			= 0xff,
}enumBikeState;

/* enum key leg active deactive */
typedef enum
{ 
	eKey_Leg_Signal_DeActive 	= 1,
    eKey_Leg_Signal_Active		= 2,
	eKey_Leg_Signal_Noise		= 3,
}eKey_Leg_Signal;
/******************************************/
/* Typedef Define for Function */
#define SIZE_IMAGE 1024
#define SIZE_TEXT  100
#define SIZE_ADDR  200
#define SIZE_PHONE 20
#define SIZE_HW_VERSION 	10
#define SIZE_FW_VERSION 	10
#define SIZE_PRODUCT_NAME 	20
#define SIZE_ID				4
#define SIZE_MANUFACTUER_INFO SIZE_IMAGE+SIZE_ADDR+SIZE_PHONE+SIZE_HW_VERSION+SIZE_FW_VERSION+SIZE_ID+SIZE_PRODUCT_NAME

/* Feature of Product */
typedef union
{
    int16_t uFeature_Config_Value;
    struct{
        enumbool CONFIG_CHANGE_MODE     :1;/* Config for change mode */
        enumbool CONFIG_LEG_BIKE        :1;/* Config for absent leg bike */
        enumbool CONFIG_ANTI_DROP       :1;/* Config for anti drop function */
        enumbool CONFIG_ANTI_MOVE       :1;/* Config for anti move function */
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool                        :1;
        enumbool LAST_DEEP_MODE_ACTIVE  :1;
        enumbool MANUAL_BOOTLOADER      :1;
    }bits;
}StructFeature_Config;

/* Struct for User Information */
__packed
typedef struct
{
    uint32_t    ID_User;                            /* For Activate */
    uint8_t     Name[20];                           /* User Name    */
    uint32_t    Phone[15];                          /* User Phone   */
    uint32_t    Activate_Day;                       /* Day */
    char		RF_SW_MAIN_ADDR[2];					/* RF Mainboard Software Address */
	char		RF_SW_TAG_ADDR_1[2];				/* RF Tagboard Software Address */
	char		RF_SW_TAG_ADDR_2[2];				/* RF Tagboard Software Address */
	char		RF_HW_USER_ADDR[5];					/* RF HW User Address */
	char		RF_HW_MESHNET_ADDR[5];				/* RF HW MeshNet Address */
	uint8		RF_Channel;							/* RF Channel */
    StructFeature_Config StrConfig;
}Struct_User_Information;

/* Struct for System Information */
__packed
typedef struct
{
	/* For Main Info */
    uint16_t				MAIN_VBAT;	/* Voltage */
	uint16_t				MAIN_VACCU;	/* Voltage */
	uint16_t				MAIN_COVER;
	uint16_t				MAIN_IC_SENS;
	uint8_t 				MAIN_IC_Start_Signal;	/* eTRUE mean Start_Signal_Event, eFALSE mean not get Start_Signal_Event*/
	/* For main key and main leg */
	eButtonState			MAIN_Key;
	eButtonState			MAIN_Leg_Bike;
	eKey_Leg_Signal			MAIN_Key_Active;
	eKey_Leg_Signal			MAIN_Leg_Active;
	/* Accelometer */
	eAcceBikeStatus			MAIN_Acce_State;
	enumBikeState			MAIN_Bike_State;
	enumModeBikeState		MAIN_Mode_Bike_State;
	/* For Tag Info */
	uint8_t								TAG_BAT_Percent;	/* 1 byte */
	enumRFAppFrame_TAGRFCommand			TAG_RF_Command; 	/* 1 byte */
	eButtonState						TAG_Button_State; 	/* 1 byte */
	uint8_t								TAG_ACCE_x; 		/* 1 byte */
	uint8_t								TAG_ACCE_y; 		/* 1 byte */
	uint8_t								TAG_ACCE_z; 		/* 1 byte */
	uint8_t 							TAG_NEAR;			/* 1 byte */
	uint8_t								TAG_RTC_Time; 		/* 4 byte */
	/* User Current Parameter */
	char StrRF_Current_SW_TAG_ADDR[2];
	enumbool bFlagRFTagAddressAuthen;
	uint8_t bEmergencyCurrentCode;
}Struct_System_Information;
    
/* Struct for Manufacturer Infomation */
__packed
typedef struct
{
    char        Address[SIZE_ADDR];
    char        Phone[SIZE_PHONE];
    char        HW_Version[SIZE_HW_VERSION];
    char        FW_Version[SIZE_FW_VERSION];
    char    	ID_HW_Product[SIZE_ID];
    char    	Product_Name[SIZE_PRODUCT_NAME];
}Struct_Manufactuer_Information;

/* Define Size config */
#define PRODUCT_NAME_SIZE       12
#define PRODUCT_ID_SIZE         4
#define FW_VERSION_SIZE         12
#define BL_VERSION_SIZE         12
#define SW_RF_ADDRESS_SIZE      2
#define HW_RF_ADDRESS_SIZE      5
#define HW_VERSION_SIZE			12
#define EMERGENCY_CODE_SIZE		5
#define CONFIG_PARA_SIZE		2
#define ALL_SIZE      			256

/* Struct for Config Infomation */
typedef struct
{
	/* Product info */
    char      	ProductID[PRODUCT_ID_SIZE];
    char        ProductName[PRODUCT_NAME_SIZE];
	char		BL_Version[BL_VERSION_SIZE];
    char        FW_Version[FW_VERSION_SIZE];
    char		RF_SW_MAIN_ADDR[SW_RF_ADDRESS_SIZE];
	char		RF_SW_TAG_ADDR_1[SW_RF_ADDRESS_SIZE];/* Active TAG Adress 1*/
	char		RF_SW_TAG_ADDR_2[SW_RF_ADDRESS_SIZE];/* Active TAG Adress 2*/
	char		RF_HW_USER_ADDR[HW_RF_ADDRESS_SIZE];
	char		RF_HW_MESHNET_ADDR[HW_RF_ADDRESS_SIZE];
	char 		HW_Version[HW_VERSION_SIZE];
	/*Bytes: 4 + 12 + 12 + 12 + 2 + 2 + 2 + 5 + 5 + 12 = 68 */
	/* Config Parameter */
	char		Bell_Para[CONFIG_PARA_SIZE];
	char		Xinhan_Para[CONFIG_PARA_SIZE];
	char		ICFire_Para[CONFIG_PARA_SIZE];
	char		ReplaceKey_Para[CONFIG_PARA_SIZE];
	/*Bytes: 2 + 2 + 2 + 2 = 8 */
	/* User Parameter */
	uint32_t	uTimeTurnOffRFBoard;
	uint32_t	uTimeTurnOffAllBoard;
	char		EmergencyCode1[EMERGENCY_CODE_SIZE];
	char		EmergencyCode2[EMERGENCY_CODE_SIZE];
	char		EmergencyCode3[EMERGENCY_CODE_SIZE];
	char		EmergencyCode4[EMERGENCY_CODE_SIZE];
	char		EmergencyCode5[EMERGENCY_CODE_SIZE];
	uint8_t		EmergencyUsingCounts;
	uint8_t		EmergencyCurrentCode;
	uint32_t	ubCountTimingLowPower;
	uint8_t		bModeBike;/* Mode 1, Mode 2*/
	uint8_t		uResetCounting;/* Mode 1, Mode 2*/
	/* RF Channel Parameter */
	uint8		RF_Run_Channel;							/* RF Channel */
    /* Config Product Features */
    StructFeature_Config StrConfig;
    /* ACCE sensitive */
    uint8_t uACCESensitive;
	/* Reverse for back up */
	char		Reverse[ALL_SIZE-PRODUCT_ID_SIZE-PRODUCT_NAME_SIZE-BL_VERSION_SIZE-FW_VERSION_SIZE-SW_RF_ADDRESS_SIZE*3-HW_RF_ADDRESS_SIZE*2-HW_VERSION_SIZE
							-CONFIG_PARA_SIZE*4-4-4-EMERGENCY_CODE_SIZE*5-1-1-4-1-1-1-sizeof(StructFeature_Config)-1];
}Struct_Flash_Config_Parameter;
/******************************************/
#endif