/**
 * This file contains static wrapper functions for the nrF5 SDK timers.
 *
 * @author  Ben Lynch
 */

#ifndef __CUSTOM_TIMER_H__
#define __CUSTOM_TIMER_H__

#include "app_timer.h"
#include "nrf_drv_clock.h"

// General application timer settings.
#define APP_TIMER_PRESCALER             0    	/** Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4     /** Size of timer operation queues. */
#define RTC1_FREQUENCY									32,000

static void timers_init(){
	//must be called before any other app timer API calls
	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
}

/**
 * Creates a new timer given a timer handle
 *
 * @param	timer_handle	 a handle to a application timer
 */
static void create_timers(app_timer_id_t const * timer_handle, void (* handler) (void *)){
	uint32_t err_code;

	// Create timer
	err_code = app_timer_create(timer_handle,
															APP_TIMER_MODE_SINGLE_SHOT,
															handler);
	APP_ERROR_CHECK(err_code);
}

/**
 * Determines the time elapsed in seconds between a start and end tick
 *
 * @param	start_ticks	 	the start ticks
 * @param	end_ticks	 		the end ticks
 * @return the time in seconds elapsed between both ticks
 */
static float elapsed_time_from_ticks(uint32_t start_ticks, uint32_t end_ticks){
	uint32_t elapsed_ticks = 0;
	app_timer_cnt_diff_compute(start_ticks, end_ticks, &elapsed_ticks);
	float seconds_elapsed = ((float)elapsed_ticks / RTC1_FREQUENCY);
	return seconds_elapsed;
}

#endif //__CUSTOM_TIMER_H__
