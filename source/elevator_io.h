/**
* @file
* @brief IO library for elevator, spanning set/get-functions for buttons and lights
*/
#ifndef ELEVATOR_IO_H
#define ELEVATOR_IO_H

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
 * @brief Set the floor indicator light. The lights will only be set, if the elevator is at a defined
 * floor. Therefore, the light will not be changed when the elevator is between floors
 * 
 * @param[in] floor    The floor whos indicator we would like to set  
 */
void set_floor_indicator_light(int floor);



/**
 * @brief Poll all floor buttons for input
 * 
 * @param[in, out] p_data    Pointer to @c elevator_data_t that contains the button arrays @c orders_up and @c ORDERS_DOW
 * 
 * @warning This function operates on the assumption that @c orders_up and @c orders_down are 
 * set with 0's and 1's, respectively for "button not clicked" and "button clicked"
 * 
 * @warning This function also uses @c set_floor_button_lights() to set the button lights
 * 
 * The function checks every external elevator button, from the first floor to the last floor.
 * Upon finding a button that is clicked, that has not already been clicked ( by checking
 * the @c orders_up and @c orders_down arrays), the corresponding value in the array is set to 1.
 */
void poll_floor_buttons(int* p_orders_up, int* p_orders_down);

/**
 * @brief Set the floor button lights in accordance to the values in @c orders_up and @c orders_down .
 * 
 * @param[in] p_data    A pointer to @c elevator_data_t that contains the @c orders_up and @c orders_down
 */
void set_floor_button_lights(int* p_orders_up, int* p_orders_down);


/**
 * @brief Poll all cab buttons for input
 * 
 * @param[in, out] p_data    A pointer to @c elevator_data_t that contains the @c orders_cab that
 * will be set according to the input
 *
 * @warning This function uses @c set_cab_button_lights() to set the cab button lights
 * The function updates the cab orders for the current Order 
 */
void poll_cab_buttons(int* p_orders_cab);

/**
 * @brief Set the cab button lights in accordance to the values in @c orders_cab
 * 
 * @param[in] p_data    A pointer to @c elevator_data_t that contains the @c orders_cab
 */
void set_cab_button_lights(int* p_orders_cab);


#endif //ELEVATOR_IO_H

