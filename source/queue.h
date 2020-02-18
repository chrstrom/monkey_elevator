/**
 * @file
 * @brief Library for managing the elevator's QUEUE system
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "driver/hardware.h"
#include "elevator_fsm.h"
#include "globals.h"

#define INVALID_ORDER -2

/**
 * @struct Order
 * 
 * @brief  A struct for holding information about an order
 */
typedef struct{
    int target_floor;                   /**< The floor at which the order comes from */
    HardwareOrder order_type;           /**< The type of order */
} Order;

Order QUEUE[QUEUE_SIZE];                /**< The elevator's queue */


/**
 * @brief Initialize the @c QUEUE with all invalid orders
 */
void init_queue();


/**
 * @brief Update a QUEUE by removing the first element.
 * The function will invoke delete_holes_in_queue(), such that the queue's valid orders
 * are in serial, and not split between invalid orders.
 * 
 * @warning Will only work if the elevator is at a defined floor
 */
void update_queue();


/**
 * @brief Add orders to the @c QUEUE if an existing order with the same parameters is not in it
 *
 * 
 * The function checks each element in the  @c QUEUE, and pushes a new @c Order
 * to the @c QUEUE with the corresponding @p target_floor and @p order_type
 */
void add_order_to_queue(int target_floor, HardwareOrder order_type);


/**
 * @brief Checks the @c QUEUE for an order with specified parameters
 *
 * @param[in] floor         The floor to check the @c QUEUE for
 * @param[in] order_type    The order_type to check the @c QUEUE for
 * 
 * @return 1 if the QUEUE contains a matching order, and 0 if it does not
 */
int check_queue_for_order(int target_floor, HardwareOrder order_type);


/**
 * @brief Set the cab orders according to cab buttons pressed
 * 
 * @param[in/out] p_data Pointer to @c elevator_data_t that contains the array for cab-orders 
 */
void set_cab_orders(elevator_data_t* p_data);


/**
 * @brief Clear a cab order for a given floor
 * 
 * @param[in] current_floor     The floor to be used for clearing the cab orders
 * @param[in/out] p_data        Pointer to @c elevator_data_t that will have it's cab-orders reset
 */
void clear_cab_orders(elevator_data_t* p_data, int current_floor);


/**
 * @brief Clear all orders in the @c QUEUE connected to @p current_floor
 * 
 * @param[in] current_floor The floor to delete the @c Order
 * @param[in/out] p_data    Pointer to @c elevator_data_t that contains the orders for the cab, up and down
 */
void clear_orders_at_floor(elevator_data_t* p_data, int current_floor);


/**
 * @brief Empty the QUEUE by removing all elements
 */
void erase_queue(elevator_data_t* p_data);


/**
 * @brief Check if the QUEUE is empty
 * 
 * @return 1 if empty and 0 if not
 * 
 * @warning We assume that the first element in the queue is always updated! If not, this
 * function will return false results
 */
int queue_is_empty();


/**
 * @brief Check if the QUEUE has a valid order to handle at a floor
 * 
 * @param[in] p_data    Pointer to @c elevator_data_t . Used to validate the checks
 * 
 * @return 1 if any @c Order in @c QUEUE has a valid order at the floor, and 0 if not.
 * 
 * @warning A cab order will ALWAYS be handled if the elevator drives past it, but the function will
 * differentiate between orders-up and orders-down.
 */
int check_order_match(elevator_data_t* p_data);


/**
 * @brief Pushes a new @c Order to the @c QUEUE
 * 
 * @param[in] floor         The floor that the @c Order came from
 * @param[in] order_type    The direction of the @c Order
 * 
 * The function finds the first element in the QUEUE that has its @c target_floor
 * value set to INVALID_ORDER (= -2), and updates this element with @p floor and @p order_type .
 */
void push_back_queue(int floor, HardwareOrder order_type);


/**
 * @brief Pushes a new @c Order to the front of the @c QUEUE
 * 
 * @param[in] floor         The floor that the @c Order came from
 * @param[in] order_type    The direction of the @c Order
 * 
 * The function right-shifts every element int the @c QUEUE and sets the first element to
 * @p floor and @p order_type 
 */
void push_front_queue(int floor, HardwareOrder order_type);


/**
* @brief Reset a spesific order in the queue, with given @p floor and @ order_type. 
* Will call another function to remove the hole left in the queue
*    
* @param[in] floor      The floor we would like to reset an order to
*
* @param[in] order_type The @c HardwareOrder we want to search for
*/
void erase_single_order(int floor, HardwareOrder order_type);


/**
 * @brief Delete all occurencec of "holes" in the @c QUEUE
 * 
 * The function will left-shift all orders, until the @c QUEUE contains no holes
 */
void delete_holes_in_queue();

#endif //QUEUE_H