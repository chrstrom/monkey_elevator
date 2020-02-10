/**
* @file
* @brief Timer-functions. Mostly about getting and adding the current
* floor and orders for the elevator
*/

#ifndef TIMER_H
#define TIMER_H

#include <time.h>


// Set the timer to the current time
void start_timer(time_t* timer);

// Return true when sec has passed since the timer was started
int check_timer(time_t* timer, double sec);

#endif