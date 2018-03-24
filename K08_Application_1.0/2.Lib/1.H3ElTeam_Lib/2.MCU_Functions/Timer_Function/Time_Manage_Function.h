/******************** (C) COPYRIGHT 2014 H3ElTeam ********************
* File Name          : Timer_Manage_Function.h
* Author             : Hai Dang
* Date First Issued  : 21-Sep-2014
* Description        : global prototype, extern variable to init, manage time function

********************************************************************************
* History: DD/MM/YY
* 21/09/2014: V0.1
* 21/09/2014: V0.2 Add CLOCK_TYPE_S
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Timer_Manage_Function__H
#define _Timer_Manage_Function__H
#include "Common.h"
#include "Timer_Function.h"
#include "Timer_Function_Profile.h"
/* Define new type */
#define clock_time_t uint32_t
#define CLOCK_CONF_SECOND (1000)

/* Clock type define struct */
typedef enum
{
	CLOCK_TYPE_US	=1,
	CLOCK_TYPE_MS	=2,
	CLOCK_TYPE_S	=3,
	CLOCK_TYPE_RTC	=4,
}enum_clock_type;
/**
 * A timer.
 *
 * This structure is used for declaring a timer. The timer must be set
 * with timer_set() before it can be used.
 *
 * \hideinitializer
 */
typedef struct {
  	clock_time_t 	start;
  	clock_time_t 	interval;
	enum_clock_type	clock_type;
	clock_time_t (*clock_time)(void);
}timer;

/* Prototype for function use */
void timer_set(timer *t, clock_time_t interval, enum_clock_type type);
void timer_reset(timer *t);
void timer_restart(timer *t);
int timer_expired(timer *t);
clock_time_t timer_remaining(timer *t);
clock_time_t clock_time_timer_us(void);
clock_time_t clock_time_timer_ms(void);
clock_time_t clock_time_timer_s(void);
clock_time_t clock_time_timer_rtc(void);
#endif /* _Timer_Manage_Function__H */

/** @} */
/** @} */
