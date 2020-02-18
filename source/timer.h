/**
* @file
* @brief Library for the elevator's timer.
*/
#ifndef TIMER_H
#define TIMER_H

#include <time.h>

time_t door_timer;

// void reset_timer();

// int check_timer();

/**
 * @brief Start the timer with the current time-value. 
 * 
 * @param[in, out] t_timer The time of initializing
*/
void start_timer(time_t* p_timer);


/**
 * @brief Check if a certain amount of time has passed
 * 
 * @param[in] p_timer The initial time we would like to check 
 * against as a reference.
 * 
 * @return 1 if enough time has passed, 0 if not.
 */
// Return true when sec has passed since the timer was started
int check_timer(time_t* p_timer, unsigned int time_req);


#endif //TIMER_H