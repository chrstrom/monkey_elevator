/**
* @file
* @brief IO library for elevator, spanning set/get -functions for buttons and lights
*/

#ifndef ELEVATOR_IO_H
#define ELEVATOR_IO_H

#include "includes.h"
#include "queue.h"


/**
 * @brief Find what floor the elevator is at
 * 
 * @return 0 if the elevator is between floors, and an int corresponding to the current floor
 * if the elevator is at a floor
 */
int at_floor();


/**
 * @brief Poll all floor buttons
 * 
 * @param[out] queue                A pointer to the queue, to which an @c Order object will be added if a
 * floor button is pressed
 * @param[in][out] p_order_up       A pointer to an array containing the states of the up-buttons
 * @param[in][out] p_order_down     A pointer to an array containing the states of the down-buttons
 */
void poll_floor_buttons(Order* queue, int* p_order_up, int* p_order_down) 


/**
 * @brief Set the cab button requests for an @c Order
 * 
 * @param[out] p_current_order  A pointer to the order to be updated
 */
void update_cab_buttons(Order* p_current_order);


#endif

