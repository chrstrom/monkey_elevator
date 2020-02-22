/**
* @file
* @brief IO library for elevator, spanning set/get-functions for buttons and lights
*/
#ifndef ELEVATOR_IO_H
#define ELEVATOR_IO_H

/**
 * @brief Initialize the elevator
 * 
 * @return A true value on successful initialization, 0 if it failed.
 * 
 * Initiailize the elevator by setting all connected values to its default values, and, 
 * if the elevator isn't already at a floor, drive the elevator down to the first valid floor.
 */
int elevator_init();


// /**
//  * @brief Set the cab button lights in accordance with the values in @p p_orders_cab
//  * 
//  * @param[in, out] p_orders_cab      A pointer to an array of the cab-button states
//  */
// void set_cab_button_lights(int* p_orders_cab);


/**
 * @brief Set the floor indicator light
 * 
 * @param[in] floor    The floor whos indicator we would like to set  
 * 
 * The lights will only be set, if the elevator is at a defined floor. Therefore,
 * the light will not be changed when the elevator is between floors.
 */
void set_floor_indicator_light(int floor);


/**
 * @brief Find what floor the elevator is at
 * 
 * @return -1 if the elevator is between floors, and an int corresponding to the current floor
 * if the elevator is at a floor
 * 
 * @warning Because the elevator hardware's floors are indexed at 0, a non-truthy value (0) will
 * be returned when the elevator is at the first floor
 */
int get_current_floor();


/**
 * @brief Updates the cab buttons based on current input
 * 
 * @param[in, out] p_orders_cab      A pointer to an array of the cab-button states
 *
 * @warning This function uses @c set_cab_button_lights() to set the cab button lights
 * 
 * The function updates the cab orders for the current Order 
 */
void update_cab_buttons(int* p_orders_cab);


// /**
//  * @brief Set the floor button lights in accordance with the values @p p_orders_up and @p p_orders_down .
//  * 
//  * @param[in, out] p_orders_up      A pointer to an array of the up-button states
//  * @param[in, out] p_orders_down    A pointer to an array of the down-button states
//  * 
//  * Sets all floor button lights on or off, 
//  */
// void set_floor_button_lights(int* p_orders_up, int* p_orders_down);


/**
 * @brief Updates the floor buttons based on current input
 * 
 * @param[in, out] p_orders_up      A pointer to an array of the up-button states
 * @param[in, out] p_orders_down    A pointer to an array of the down-button states
 * 
 * @warning This function operates on the assumption that @p p_orders_up and @p p_orders_down are 
 * set with 0's and 1's, respectively for "button not clicked" and "button clicked"
 * 
 * @warning This function also uses @c set_floor_button_lights() to set the button lights
 * 
 * The function checks every external elevator button, from the first floor to the last floor.
 * Upon finding a button that is clicked, that has not already been clicked ( by checking
 * the @p p_orders_up and @p p_orders_down arrays), the corresponding value in the array is set to 1.
 */
void update_floor_buttons(int* p_orders_up, int* p_orders_down);

#endif //ELEVATOR_IO_H
