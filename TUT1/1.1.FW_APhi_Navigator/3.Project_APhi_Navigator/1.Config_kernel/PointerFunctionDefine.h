/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : PointerFunctionDefine.h
* Author             : Hai Dang
* Date First Issued  : 20-Sep-2014
* Description        : Define for global function struct to manage and prototype of pointer function.

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
#ifndef	_PointerFunctionDefine__H
#define _PointerFunctionDefine__H

/* I/O Define struct, include read, write, writeStatus */
__packed
typedef struct
{
  	enumbool (*read)(void);
  	void(*write)(enumbool sta);
  	enumbool (*writeSta)(void);
}IO_Struct;

/* SPI struct, include init, on, off, read_write 1 byte data */
__packed
typedef struct
{
  	void (*open)(void);
	void (*close)(void);
	void (*start_send)(void);
	void (*stop_send)(void);
	void (*start_receive)(void);
	void (*stop_receive)(void);
	void (*send_buf)( unsigned char *buf, uint16_t uNumberByteSend);
	void (*send_str)( unsigned char *str);
	void (*send_byte)(unsigned char cChar);
	void (*get_buf)( unsigned char *buf, uint16_t uNumberByteGet);
	unsigned char (*read_byte)(void);
	unsigned char (*read_write_direct)( unsigned char data);
	unsigned char (*write_direct)( unsigned char data);
}SPI_Struct;

/* UART struct, include init, open, close, send & read function */
__packed
typedef struct
{
  	void (*open)(void);
	void (*close)(void);
	void (*start_send)(void);
	void (*stop_send)(void);
	void (*start_receive)(void);
	void (*stop_receive)(void);
	void (*send_buf)( unsigned char *buf, uint16_t uNumberByteSend);
	void (*send_str)( unsigned char *str);
	void (*send_byte)(unsigned char cChar);
	void (*get_buf)( unsigned char *buf, uint16_t uNumberByteGet);
	enumbool (*read_byte)(char *pChar);
}UART_Struct;
#endif