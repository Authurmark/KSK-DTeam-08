/******************** (C) COPYRIGHT 2013 H3ElTeam ********************
* File Name          : ringbuf.c
* Author             : Nguyen Van Hai, Modify: Hai Dang 
* Date First Issued  : 24-Sep-2014
* Description        : Define for global function struct to manage and prototype of pointer function.

********************************************************************************
* History: DD/MM/YY
* 24-Sep-2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include <stdint.h>
#include <stdlib.h>

/* RingBuf Define struct, include head, tail, size, pointer data */
 typedef struct
{
    volatile uint32_t head;
    volatile uint32_t tail;
    volatile uint32_t size;
    volatile uint8_t *pt;
}RINGBUF;

/* Prototype function */
int32_t RINGBUF_Init(RINGBUF *r, uint8_t* buf, uint32_t size);
int32_t RINGBUF_Put(RINGBUF *r, uint8_t c);
int32_t RINGBUF_Get(RINGBUF *r, uint8_t* c);
int32_t RINGBUF_GetFill(RINGBUF *r);
#endif
