/**
* @file
* @brief Utility-functions. Mostly about getting and adding the current
* floor and orders for the elevator
*/

#ifndef UTILITY_H
#define UTILITY_H

#include <time.h>
/**
* @brief Find and return the current floor of the elevator
*
* @return The current floor the elevator is at. If the elevator is
* between floors, or out of range, the function will return 0.
*/
int at_floor();

/**
* @brief Add floors the elevator should visit. The function will differentiate
* between orders going up and orders going down. 
*
* @param[in] p_up Elevator queue up
*
* @param[in] p_down Elevator queue down
*/
void add_floor_orders(int* p_up, int* p_down);





// Set the timer to the current time
void start_timer(time_t* timer);

// Return true when sec has passed since the timer was started
int check_timer(time_t* timer, double sec);
#endif