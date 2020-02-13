/**
* @file
* @brief Library for the elevator's timer.
*/
#ifndef TIMER_H
#define TIMER_H

#include "includes.h"


// TODO:
// If everything works as is, we can try to change the timer module to
// only use one timer, and update a variable in timer.h, as well as letting
// check_timer take a double as a "time passed" argument instead of NORMAL_WAIT_TIME
// in the function


/**
 * @brief Start the timer with the current time-value. 
 * 
 * @param[in/out] t_timer The time of initializing
*/
void start_timer(time_t* p_timer);


/**
 * @brief Return true when a spesific time has passed
 * 
 * @param[in] p_timer The initial time we would like to check 
 * against as a reference.
 * 
 * @param[out] int If time > compare, return 1. Else return 0
 */
// Return true when sec has passed since the timer was started
int check_timer(time_t* p_timer);

#endif