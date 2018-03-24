/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : ComFunction.c
* Author             : OS Team
* Date First Issued  : 03/11/2015
* Description        : This file contains function hardware.
The programer just comment or uncomment to enable, disable, config
hardware function.

********************************************************************************
* History: DD/MM/YY
* 03/11/2015: v0.1	- Start 
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Project includes. */
#include "ComFunction.h"
#include "Project_Function.h"
#include "Time_Manage_Function.h"
#include "IO_Kernel_Function.h"

/* Variable */
uint16 CRCData,cntData;
uint8 UART_BUFFER_RX[i_MAX_UART];
uint8 UART_BUFFER_TX[i_MAX_UART];
uint8 PACKAGE_BUFF[PACKAGE_SIZE];
uint8_t bLast_Idx_Low = 0, bLast_Idx_High = 0;
uint16   i_UART_TX=0, i_UART_RX=0;
uint16   uPackageLeng=0;
uint32   UART_Tick;
enumbool eUARTDetectEnCMD=eFALSE;
enumbool EnableUARTComm=eFALSE;
extern enumbool bFlag_USART_RX;
uint16 FirmwareSize;
uint16 FirmwareBlockIndex;
uint16 FirmwareBlock;
#ifdef USE_LED_SEGMENT
extern uint8_t bLED7Value[NUMBER_LED_7_SEGMENT];
#endif /* NUMBER_LED_7_SEGMENT */
extern Struct_Flash_Config_Parameter	StrConfigPara;
enumbool bFlagGetCommandLEDConfigUART1;
structIO_Manage_Output bLEDConfigCommand;
/************************************************/
void UART_Comm(void)
{
    uint8_t iIndex;
	/*Process Command.................*/
    /*FRAME FORMAT+++++++++++++++++++++++++++++++++++++++++++++
    Preamble(5B) |Len(2B)     |CMD(1B)        | Indec(2B)   | DATA(len B)   | CS(1B)
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*Reset index*/
    i_UART_RX = 0;    
    CRCData = 0;
    /* Check framemable */
    if((UART_BUFFER_RX[iUART_PRE_1]==PREMABLE_BYTE_1)
        &&(UART_BUFFER_RX[iUART_PRE_2]==PREMABLE_BYTE_2)
            &&(UART_BUFFER_RX[iUART_PRE_3]==PREMABLE_BYTE_3)
               &&(UART_BUFFER_RX[iUART_PRE_4]==PREMABLE_BYTE_4)
                  &&(UART_BUFFER_RX[iUART_PRE_5]==PREMABLE_BYTE_5))
    {
        /* Command Info */
        if ((cmd_type)UART_BUFFER_RX[iUART_CMD_TYPE]==P2TCMD_INFO)
        {
            /* Send Info command */
            UART_Send_STRING("\r\n\r\nINFO COMMAND: OK\r\n");
            UART_Send_STRING("PRODUCT: ");
            UART_Send_BUF(StrConfigPara.ProductName,sizeof(StrConfigPara.ProductName));
            UART_Send_STRING("\r\n");
            UART_Send_STRING("PRODUCT ID: ");
            UART_Send_BUF(StrConfigPara.ProductID,sizeof(StrConfigPara.ProductID));
            UART_Send_STRING("\r\n");
            UART_Send_STRING("HARDWARE: ");
            UART_Send_BUF(StrConfigPara.HW_Version,sizeof(StrConfigPara.HW_Version));
            UART_Send_STRING("\r\n");
            UART_Send_STRING("BOOTLOADER: ");
            UART_Send_BUF(StrConfigPara.BL_Version,sizeof(StrConfigPara.BL_Version));
            UART_Send_STRING("\r\n");
            UART_Send_STRING("FIRMWARE: ");
            UART_Send_BUF(StrConfigPara.FW_Version,sizeof(StrConfigPara.FW_Version));
            UART_Send_STRING("\r\n");
        }
        /* Process command */
        vComCommandProcess();
        /* Check enable command */
        if (!EnableUARTComm)
              return;
    }
    /*Reset index*/
    i_UART_RX=0;
}
/**********************************************************************************/
void UART_MakeFeedback(cmd_type CMD, uint16 CODE, uint16 uLengByte)
{    
	memset(UART_BUFFER_TX,0,i_MAX_UART);
	UART_BUFFER_TX[iUART_PRE_1] = PREMABLE_BYTE_1;   /*data size LB*/
	UART_BUFFER_TX[iUART_PRE_2] = PREMABLE_BYTE_2;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_3] = PREMABLE_BYTE_3;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_4] = PREMABLE_BYTE_4;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_5] = PREMABLE_BYTE_5;   /*data size LB*/
    UART_BUFFER_TX[iUART_SIZE_LOW] = (uint8)(uLengByte&0x00FF);/*data size HB*/
    UART_BUFFER_TX[iUART_SIZE_HIGH] = (uint8)(uLengByte>>8);   	/*data size LB*/
    UART_BUFFER_TX[iUART_CMD] = CMD;/*command*/
    UART_BUFFER_TX[iUART_IDX_LOW] = bLast_Idx_Low;      /*index frame LB*/
    UART_BUFFER_TX[iUART_IDX_HIGH] = bLast_Idx_High;      /*index frame HB*/
    UART_BUFFER_TX[iUART_DATA] 		= CODE;
    UART_BUFFER_TX[iUART_DATA+1] 	= CODE>>8;
    UART_BUFFER_TX[iUART_DATA+2] = CRC8Bits(UART_BUFFER_TX,iUART_DATA+2);
	i_UART_TX = iUART_DATA+3;
}
/**********************************************************************************/
void UART_MakeData(cmd_type CMD, uint16 CODE, uint16 uLengByteContent, char* pSend)
{    
	uint16_t uLengBytePackage;
    /* Clear buffer */
	memset(UART_BUFFER_TX,0,i_MAX_UART);
	UART_BUFFER_TX[iUART_PRE_1] = PREMABLE_BYTE_1;   /*data size LB*/
	UART_BUFFER_TX[iUART_PRE_2] = PREMABLE_BYTE_2;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_3] = PREMABLE_BYTE_3;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_4] = PREMABLE_BYTE_4;   /*data size LB*/
    UART_BUFFER_TX[iUART_PRE_5] = PREMABLE_BYTE_5;   /*data size LB*/
	/* Increase for CODE */
	uLengBytePackage=uLengByteContent+2;
    UART_BUFFER_TX[iUART_SIZE_LOW] = (uint8)(uLengBytePackage&0x00FF);/*data size HB*/
    UART_BUFFER_TX[iUART_SIZE_HIGH] = (uint8)(uLengBytePackage>>8);   	/*data size LB*/
    UART_BUFFER_TX[iUART_CMD] = CMD;/*command*/
    UART_BUFFER_TX[iUART_IDX_LOW] = bLast_Idx_Low;      /*index frame LB*/
    UART_BUFFER_TX[iUART_IDX_HIGH] = bLast_Idx_High;      /*index frame HB*/
	UART_BUFFER_TX[iUART_DATA] 		= CODE;
    UART_BUFFER_TX[iUART_DATA+1] 	= CODE>>8;
    /* Copy data content */
    memcpy(&UART_BUFFER_TX[iUART_DATA+2],pSend,uLengByteContent);
	UART_BUFFER_TX[iUART_DATA+uLengByteContent+2] = CRC8Bits(UART_BUFFER_TX,iUART_DATA+uLengByteContent+2);
	i_UART_TX = iUART_DATA+uLengByteContent+3;
}
/* Feed back command & content */
void UART_Comm_Feedback_Command_Content(cmd_type CMD, uint16 CODE, uint16 uLengByte, char* pSend)
{
	/* Feedback Content */
	if(uLengByte!=0)
	{
		UART_MakeData(CMD,CODE,uLengByte,pSend);
		UART_Send_BUF(UART_BUFFER_TX,i_UART_TX);
	}
	else
	{
		/* Feedback Command */
		UART_MakeFeedback(CMD,CODE,NORMAL_FEEDBACK_LENGTH);
		/* Feedback */
		UART_Send_BUF(UART_BUFFER_TX,i_UART_TX);
	}
}
/*-----------------------------------------------------------*/
enumbool vComPortProcess(void)
{
    static uint8 bUSARTReceive;
    enumbool bReturn;
    bReturn = eFALSE;
    if(pUART_CONFIG.read_byte(&bUSARTReceive)==eTRUE)
    {
        if (i_UART_RX>=i_MAX_UART)       i_UART_RX=0;
        UART_BUFFER_RX[i_UART_RX++] = bUSARTReceive;
        UART_Tick = T1Us_Tick1Ms;
        eUARTDetectEnCMD = eTRUE;
        bReturn = eFALSE;
    }
    if(eUARTDetectEnCMD&&(T1Us_Tick1Ms>(UART_Tick+UART_INTERVAL)))
    {
        /*Receive sequence*/
        eUARTDetectEnCMD = eFALSE;
        UART_Comm();
        bReturn = eTRUE;
    }
    return bReturn;
}
/*************************************************************/
void vComCommandProcess()
{
    /* Check PC command */
    switch ((cmd_type)UART_BUFFER_RX[iUART_CMD])
    {
        case P2TCMD_SET_RELAY_PARA:
            /* Send Feedback */
            UART_Comm_Feedback_Command_Content(P2TCMD_SET_RELAY_PARA,FBCODE_OK,0,PACKAGE_BUFF);
        break;
        case P2TCMD_SET_LED_PARA:
            /* Send Feedback */
			UART_Comm_Feedback_Command_Content(P2TCMD_SET_LED_PARA,FBCODE_OK,0,PACKAGE_BUFF);
            /* Set flag eTRUE*/
            bFlagGetCommandLEDConfigUART1 = eTRUE;
            /* Update parameter */
            bLEDConfigCommand.uCountToggle  = UART_BUFFER_RX[iUART_DATA];
            bLEDConfigCommand.uFrequency    = UART_BUFFER_RX[iUART_DATA+1];
            bLEDConfigCommand.bStartState 	= UART_BUFFER_RX[iUART_DATA+2];
            bLEDConfigCommand.bEndState 	= UART_BUFFER_RX[iUART_DATA+3];
            bLEDConfigCommand.bFlagStart    = UART_BUFFER_RX[iUART_DATA+4];
        break;
        /*Check CMD*/
        case P2TCMD_SET_RELAY_DIRECT:
            /* Send Feedback */
			UART_Comm_Feedback_Command_Content(P2TCMD_SET_RELAY_DIRECT,FBCODE_OK,0,PACKAGE_BUFF);
        break;
        case P2TCMD_SET_LED_DIRECT:
            /* Send Feedback */
			UART_Comm_Feedback_Command_Content(P2TCMD_SET_LED_DIRECT,FBCODE_OK,0,PACKAGE_BUFF);
            /* Get data and process */
            if(UART_BUFFER_RX[iUART_DATA]==eTRUE)
            {
//                UART_Send_STRING("\r\n TURN ON LED!");
                LED_USER_ON;
            }
            else if(UART_BUFFER_RX[iUART_DATA]==eFALSE)
            {
//                UART_Send_STRING("\r\n TURN OFF LED!");
                LED_USER_OFF;
            }
        break;
        default:
        break;
    }
}