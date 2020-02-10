/**
* @file
* @brief Utility-functions. Mostly about getting and adding the current
* floor and orders for the elevator
*/

#ifndef UTILITY_H
#define UTILITY_H

#include "includes.h"

/**
* @brief Find and return the current floor of the elevator
*
* @param [out] The current floor the elevator is at. If the elevator is
* between floors, or out of range, the function will return -1.
*/
int at_floor();

/**
* @brief Reset the order's to a pointer containing only 
* invalid orders. In this case, we use -1 to symbolize
* invalid floors/orders.
*
* @param[in] p_order The pointer we would like to reset/
* invalidate
*/
void reset_orders(int* p_order);

/**
* @brief Reset the order's to a pointer containing only 
* invalid orders. In this case, we use -1 to symbolize
* invalid floors/orders.
*
* @param[in] p_order The pointer we would like to reset/
* invalidate
*/

/**
* @brief Add floors the elevator should visit. The function will differentiate
* between orders going up and orders going down. 
*
* @param[in] p_up Elevator queue up
*
* @param[in] p_down Elevator queue down
*/
void add_floor_orders(int* p_up, int* p_down);


/**
* @brief Add the floor orders to the cab.
*
* @param[in] p_cab Floor-orders for the cab
*/
void add_cab_orders(int* p_cab);

#endif