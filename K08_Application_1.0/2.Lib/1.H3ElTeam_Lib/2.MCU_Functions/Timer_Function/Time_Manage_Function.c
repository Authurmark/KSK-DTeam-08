/******************** (C) COPYRIGHT 2014 H3ElTeam ********************
* File Name          : Timer_Manage_Function.c
* Author             : Hai Dang
* Date First Issued  : 21-Sep-2014
* Description        : global prototype, extern variable to init, manage time function

********************************************************************************
* History: DD/MM/YY
* 21/09/2014: V0.1
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef	_Timer_Manage_Function__C
#define _Timer_Manage_Function__C
#include "Time_Manage_Function.h"
/*---------------------------------------------------------------------------*/
/**
 * Return clock_time high resolution: US. Time from timer.
 *
*/
clock_time_t clock_time_timer_us(void)
{
	#warning: [BUILD_WARNING]: use time clock get from TIME BASE
  	return vGetCurrentCounterTimeBaseUs();
}

/*---------------------------------------------------------------------------*/
/**
 *  Return clock_time Medium resolution: MS.
 *
*/
clock_time_t clock_time_timer_ms(void)
{
	#warning: [BUILD_WARNING]: use time clock get from TIME BASE
  	return vGetCurrentCounterTimeBaseMs();
}

/*---------------------------------------------------------------------------*/
/**
 *  Return clock_time Medium resolution: S.
 *
*/
clock_time_t clock_time_timer_s(void)
{
	#warning: [BUILD_WARNING]: use time clock get from TIME BASE
  	return vGetCurrentCounterTimeBaseS();
}

/*---------------------------------------------------------------------------*/
/**
 *  Return clock_time Medium resolution: RTC.
 *
*/
clock_time_t clock_time_timer_rtc(void)
{
  	//return vGetCurrentCounterTimeBaseS();
	/* Return RTC counter */
	return RTC_GetCounter();
}
/*---------------------------------------------------------------------------*/
/**
 * Set a timer.
 *
 * This function is used to set a timer for a time sometime in the
 * future. The function timer_expired() will evaluate to true after
 * the timer has expired.
 *
 * \param t A pointer to the timer
 * \param interval The interval before the timer expires.
 *
 */
void timer_set(timer *t, clock_time_t interval, enum_clock_type type)
{
	/* Set type */
	t->clock_type	= type;
  	t->interval 	= interval;
	/* Set start time base on Clock type */
	if(t->clock_type == CLOCK_TYPE_US)
 		t->clock_time = &clock_time_timer_us;
	else if(t->clock_type == CLOCK_TYPE_MS)
  		t->clock_time = &clock_time_timer_ms;
	else if(t->clock_type == CLOCK_TYPE_S)
  		t->clock_time = &clock_time_timer_s;
	else if(t->clock_type == CLOCK_TYPE_RTC)
		t->clock_time = &clock_time_timer_rtc;
}
/*---------------------------------------------------------------------------*/
/**
 * Reset the timer with the same interval.
 *
 * This function resets the timer with the same interval that was
 * given to the timer_set() function. The start point of the interval
 * is the exact time that the timer last expired. Therefore, this
 * function will cause the timer to be stable over time, unlike the
 * timer_restart() function.
 *
 * \param t A pointer to the timer.
 *
 * \sa timer_restart()
 */
void timer_reset(timer *t)
{
  	t->start += t->interval;
}
/*---------------------------------------------------------------------------*/
/**
 * Restart the timer from the current point in time
 *
 * This function restarts a timer with the same interval that was
 * given to the timer_set() function. The timer will start at the
 * current time.
 *
 * \note A periodic timer will drift if this function is used to reset
 * it. For preioric timers, use the timer_reset() function instead.
 *
 * \param t A pointer to the timer.
 *
 * \sa timer_reset()
 */
void timer_restart(timer *t)
{
	t->start = t->clock_time();
}
/*---------------------------------------------------------------------------*/
/**
 * Check if a timer has expired.
 *
 * This function tests if a timer has expired and returns true or
 * false depending on its status.
 *
 * \param t A pointer to the timer
 *
 * \return Non-zero if the timer has expired, zero otherwise.
 *
 */
int timer_expired(timer *t)
{
  /* Note: Can not return diff >= t->interval so we add 1 to diff and return
     t->interval < diff - required to avoid an internal error in mspgcc. */
	clock_time_t diff = (t->clock_time() - t->start) + 1;
  return t->interval < diff;
}
/*---------------------------------------------------------------------------*/
/**
 * The time until the timer expires
 *
 * This function returns the time until the timer expires.
 *
 * \param t A pointer to the timer
 *
 * \return The time until the timer expires
 *
 */
clock_time_t timer_remaining(timer *t)
{
  return t->start + t->interval - t->clock_time();
}
/*---------------------------------------------------------------------------*/
#endif
/** @} */
