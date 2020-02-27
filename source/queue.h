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
void queue_init();


/**
 * @brief Check if the @c QUEUE is empty
 * 
 * @return 1 if the @c QUEUE is empty and 0 if not
 * 
 * @warning We assume that the first element in the queue is always updated! If not, the return value is invalid
 */
int queue_empty();


/**
 * @brief Sets a single order in the queue
 * 
 * @param[in] idx               The index of the element in the queue we wish to modify
 * @param[in] target_floor      The new target_floor for the queue element
 * @param[in] order_type        The new order type for the queue element
 * 
 * Set an @c Order in the @c QUEUE at index @p idx to the values of @p target_floor and @p order_type
 * regardless of what the @c QUEUE contains at the given @p idx .
 */
void queue_set_order(int idx, int target_floor, HardwareOrder order_type);


/**
 * @brief Empty the @c QUEUE by removing all elements
 * 
 * @param[out] p_orders_cab     A pointer to the array containing the cab button states
 * @param[out] p_orders_up      A pointer to the array containing the up-button button states
 * @param[out] p_orders_down    A pointer to the array containing the down-buttons button states
 * 
 * @warning This function invalidates every element in the queue unconditionally.
 */
void queue_erase(int* p_orders_up, int* p_orders_down, int* p_orders_cab);


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
void queue_push_back(int target_floor, HardwareOrder order_type);

/**
 * @brief Clear all orders in the @c QUEUE for the @p current_floor
 * 
 * @param[out] p_orders_up      A pointer to the array containing the up-button button states
 * @param[out] p_orders_down    A pointer to the array containing the down-buttons button states
 * @param[out] p_orders_cab     A pointer to the array containing the cab button states
 * @param[in]  current_floor    The current floor the elevator is at
 * 
 * The function clears all @c Order elements in the queue whos target_floor is equal to @p current_floor .
 * And clears the values of the button arrays at index @p current_floor
 */
void queue_clear_order_at_floor(int* p_orders_up, int* p_orders_down, int* p_orders_cab, int current_floor);


/**
 * @brief Delete all occurencec of "holes" in the @c QUEUE
 * 
 * The function will left-shift orders until the @c QUEUE contains no holes. A hole is defined as
 * an empty order that splits apart an otherwise connected part of the queue, starting from the leftmost
 * element at index 0
 */
void queue_refactor();


/**
 * @brief Check if the QUEUE has a valid order to handle at a floor
 *
 * @param[in] target_floor  The floor used in the @c QUEUE check
 * @param[in] order_type    The type of order we check for
 * 
 * @return 1 if any @c Order in @c QUEUE has a valid order at the floor, and 0 if not.
 * 
 * The function determines if the @c QUEUE contains an order with matching @p target_floor and @p order_type . 
 * Note that a cab order only needs a matching @p target_floor to count as matching, while an up/down order will
 * require both parameters matching.
 */
int queue_check_order_match(int target_floor, HardwareOrder order_type);


/**
 * @brief Update the @c QUEUE by removing the first element.
 * 
 * @warning Will only work if the elevator is at a defined floor
 * 
 * The function will both refactor and shift the entire queue one element to the left,
 * thereby deleting the first element; effectively "handling" an order.
 */
void queue_update();


#endif //QUEUE_H