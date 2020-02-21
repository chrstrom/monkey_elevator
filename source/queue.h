/**
 * @file
 * @brief Library for managing the elevator's QUEUE system
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "driver/hardware.h"
#include "globals.h"


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
 * 
 * @warning Will only work if the elevator is at a defined floor
 * 
 * The function will invoke delete_holes_in_queue(), such that the queue's valid orders
 * are in serial, and not split between invalid orders.
 */
void update_queue();

/**
 * @brief Empty the @c QUEUE by removing all elements
 */
void erase_queue(int* p_orders_up, int* p_orders_down, int* p_orders_cab);

// /**
//  * @brief Reset a spesific order in the queue.
//  *    
//  * @param[in] floor      The floor we would like to reset an order to
//  * @param[in] order_type The @c HardwareOrder we want to search for
//  *
//  * The function removes an @c Order from the @c QUEUE with given @p floor and @p order_type ,
//  * and cleans up the queue by removing the hole it leaves.
//  */

// void erase_single_order(int floor, HardwareOrder order_type);

/**
 * @brief Delete all occurencec of "holes" in the @c QUEUE
 * 
 * The function will left-shift orders until the @c QUEUE contains no holes
 */
void delete_holes_in_queue();

/**
 * @brief Add orders to the @c QUEUE if an existing order with the same parameters is not in it
 * 
 * @param[in] target_floor  The floor for the new @c Order
 * @param[in] order_type    The order_type for the new @c Order
 * 
 * The function uses @c check_queue_for_order() to add an @c Order
 * to the @c QUEUE with values given by @p target_floor and @p order_type if
 * and only if the queue does not contain an identical order already.
 */
void push_back_queue(int target_floor, HardwareOrder order_type);

/**
 * @brief Pushes a new @c Order to the front of the @c QUEUE
 * 
 * @param[in] floor         The floor that the @c Order came from
 * @param[in] order_type    The direction of the @c Order
 * 
 * The function right-shifts every element in the @c QUEUE and sets the first element to
 * @p floor and @p order_type 
 */
void push_front_queue(int floor, HardwareOrder order_type);

/**
 * @brief Check if the @c QUEUE is empty
 * 
 * @return 1 if the @c QUEUE is empty and 0 if not
 * 
 * @warning We assume that the first element in the queue is always updated! If not, this
 * function will return false results
 */
int check_queue_empty();

/**
 * @brief Checks the @c QUEUE for an order with specified parameters
 *
 * @param[in] target_floor  The floor used in the @c QUEUE check
 * @param[in] order_type    The order_type used in the @c QUEUE check
 * 
 * @return 1 if the @c QUEUE contains an order matching the given parameters
 * and 0 if there are no matching orders in the @c QUEUE .
 * 
 * The function checks each element in the @c QUEUE for an
 * element matching both the @p target_floor and @p order_type and returns
 * a truth value representing the result of this check.
 */
int check_queue_for_order(int target_floor, HardwareOrder order_type);

/**
 * @brief Check if the QUEUE has a valid order to handle at a floor
 *
 * @param[in] target_floor  The floor used in the @c QUEUE check
 * @param[in] order_type    The type of order we check for
 * 
 * @return 1 if any @c Order in @c QUEUE has a valid order at the floor, and 0 if not.
 * 
 * @warning A cab order will ALWAYS be handled if the elevator drives past it, but the function will
 * differentiate between orders-up and orders-down.
 */
int check_order_match(int target_floor, HardwareOrder order_type);

/**
 * @brief Clear all orders in the @c QUEUE for the @p current_floor
 * 
 * @param[in, out] p_data    Pointer to @c elevator_data_t that contains the orders for the cab, up and down 
 * @param[in] current_floor The floor to delete the @c Order
 */
void clear_orders_at_floor(int* p_orders_cab, int* p_orders_up, int* p_orders_down, int current_floor);


// /**
//  * @brief Set the cab orders according to cab buttons pressed
//  * 
//  * @param[in, out] p_data Pointer to @c elevator_data_t that contains the array for cab-orders 
//  */
// void set_cab_orders(int* p_order_array);

// /**
//  * @brief Clear a cab order for a given floor
//  * @param[in, out] p_data        Pointer to @c elevator_data_t that will have it's cab-orders reset
//  * @param[in] current_floor     The floor to be used for clearing the cab orders
//  */
// void clear_orders(int* p_array, int current_floor);
void sort_queue();

#endif //QUEUE_H