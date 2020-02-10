/**
* @file
* @brief IO library for elevator, spanning set/get-functions for buttons and lights
*/
#ifndef ELEVATOR_IO_H
#define ELEVATOR_IO_H

#include "includes.h"
#include "queue.h"


/**
 * @brief Find what floor the elevator is at
 * 
 * @return -1 if the elevator is between floors, and an int corresponding to the current floor
 * if the elevator is at a floor
 * 
 * @warning Because the elevator hardware's floors are indexed at 0, a non-truthy value (0) will
 * be returned when the elevator is at the first floor
 */
int at_floor();


/**
 * @brief Poll all floor buttons
 * 
 * @param[in][out] p_order_up       A pointer to an array containing the states of the up-buttons
 * @param[in][out] p_order_down     A pointer to an array containing the states of the down-buttons
 * 
 * @warning This function operates on the assumption that @p p_order_up and @p p_order_down are 
 * set with 0's and 1's, respectively for "button not clicked" and "button clicked"
 * 
 * The function checks every external elevator button, from the first floor to the last floor.
 * Upon finding a button that is clicked, that has not already been clicked ( by checking the @p p_order_up, @p p_order_down arrays ), a new @c Order
 * is added to the @p queue .
 */
void poll_floor_buttons(int* p_order_up, int* p_order_down);


/**
 * @brief Set the floor button lights 
 * 
 * @param[in] p_order_up       A pointer to an array containing the states of the up-buttons
 * @param[in] p_order_down     A pointer to an array containing the states of the down-buttons
 * 
 * Set the lights on/off for each floor button, in accordance to the values 1/0 in @p p_order_up and @p p_order_down
 */
void set_floor_button_lights(int* p_order_up, int* p_order_down);


/**
 * @brief Set the cab button requests for an @c Order
 * 
 * @param[out] p_queue  A pointer to the queue
 * 
 * @warning If the first element of the queue is not passed to this function, we might
 * encounter problems. This function also handles all the lights for the cab buttons.
 * 
 * The function updates the cab orders for the current Order 
 */
void update_cab_buttons(Order* p_queue);


#endif

