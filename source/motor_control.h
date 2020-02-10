#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "includes.h"

/**
 * @brief Will try to drive the elevator to a spesific floor
 * 
 * @param [in] last_known_floor The last floor the sensors
 * was in contact with, and thus the last known position of the
 * elevator.
 */
void go_to_floor(int last_known_floor, int target_floor);

/**
* @brief Find the engine's next state, depending on the
* current situation
*
* @param[in] p_up Pointer to every floor where people
* would like to travel up
*
* @param[in] p_down Pointer to the floors where people
* would like to travel down from
*
* @param[in] current_movement The elevator's current 
* movement (up, down or stop)
*
* @param[in] current_floor The elevator's current floor
*/
HardwareMovement calculate_next_movement(int* p_up, int* p_down, 
    HardwareMovement current_movement, int current_floor);

/**
* @brief Find the next floor to stop by.
* The elevator should only stop at orders 
* in the same direction
*
* @param[in] p_up Pointer to every floor where people
* would like to travel up
*
* @param[in] p_down Pointer to the floors where people
* would like to travel down from
*
* @param[in] current_movement The elevator's current 
* movement (up, down or stop)
*
* @param[in] current_floor The elevator's current floor
*/
int calculate_next_floor_stop(int* p_up, int* p_down, 
    HardwareMovement current_movement, int current_floor);

#endif