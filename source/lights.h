/**
* @file
* @brief Functions for updating the elevator's lights. 
*/

#ifndef LIGHTS_H
#define LIGHTS_H

#include "includes.h"

/**
* @brief Update the floor lights (up/down).
*
* @param[in] p_up
*
* @param[in] p_down
*
*/
void update_floor_lights(int* p_up, int* p_down);

/**
* @brief Set the cab-lights according to the elevator's 
* orders. 
*
* @param[in] p_cab a pointer to an int-array consisting
* of the elevator's orders
*/
void update_cab_lights(int* p_cab);

/**
* @brief 
*
* @param[in] floor
*/
void update_floor_button_lights(int floor);

/**
* @brief Set the stop light.
*
* @param[in] stop_light Setting the light; 1 = on, 0 = off
*/
void set_stop_light(int stop_light);

/**
* @brief Set the door-light.
*
* @param[in] door_open Setting the light; 1 = on, 0 = off
*/
void set_door_open_light(int door_open);

#endif

