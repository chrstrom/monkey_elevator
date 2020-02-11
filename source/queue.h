/**
 * @file
 * @brief Library for managing the elevator's queue system
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "includes.h"

// !! Not 100% sure if we need to dereference the pointers in sizeof
#define SIZEOF_ARR(X) sizeof(*X)/sizeof(X[0])

/**
 * @struct Order
 * 
 * @brief  A struct for holding information about an order
 */
typedef struct{
    int target_floor;                   /**< The floor at which the order comes from */
    int cab_orders[MAX_FLOOR + 1];        /**< an array of truthy values representing cab buttons pressed */
    HardwareMovement dir;
} Order;


/**
 * @brief Update a queue by removing the first element.
 * 
 * @param[in] p_queue Pointer to the queue.
 */
void update_queue(Order* p_queue);


/**
 * @brief Empty the queue by removing all elements
 *
 * @param p_queue Pointer to the queue
 */
void erase_queue(Order* p_queue);

/**
 * @brief Clear the order by removing all elements
 *
 * @param p_order Pointer to the orders up or down
 */
void erase_order(int* p_order);

/**
 * @brief Check if the queue is empty
 * 
 * @param p_queue Pointer to the @c Order queue. 
 * 
 * Return 1 if empty and 0 if not
 */
int queue_is_empty(Order* p_queue);

int queue_is_empty(Order* p_queue);

/**
 * @brief Set the cab orders for a given @c Order
 * 
 * @param[in][out] p_current_order  A pointer to the @c Order to which the cab orders will be set
 * 
 * Upon arrival at the target floor for an @c Order , we need to set the cab orders for this current order.
 */
void set_cab_orders(Order* p_current_order, int current_idx);


/**
 * @brief Clear a given @c Order of a cab order for a given floor
 * 
 * @param[out] p_queue  A pointer to the queue
 * @param[in]  floor    The floor to be used for clearing the cab orders in the 
 * 
 * Any given order can have MAX_FLOOR amount of cab orders. Upon arrival at a floor,
 * all cab orders corresponding to this floor shall be marked as handled. 
 */
void clear_cab_orders(Order* p_queue, int floor);


/**
 * @brief Update the @c target_floor value for a the current order 
 * 
 * @param[out] p_current_order  A pointer to the current order we are updating
<<<<<<< HEAD
 * @param[in]  current_floor    The floor we wish to update the order's @c target_floor value to.
 * 
 * This function "handles" part of an @c Order by changing the @c target_floor value of the current
 * order we are dealing with, to one of the floors in @c cab_orders. If this value is set to -1,
=======
 * @param[in]  floor            The floor we wish to update the order's @c target_floor value to.
 * 
 * This function "handles" part of an @c Order by changing the @c target_floor value of the current
 * order we are dealing with, to one of the floors in @c floor_to. If this value is set to -1,
>>>>>>> 2daa69d4315502c1ba22d737928012a8f1a41d29
 * the entire order is considered fully handled.
 */
void update_queue_target_floor(Order* p_current_order, int floor);


/**
 * @brief Check if the queue has a valid order to handle  
 * 
 * @param[in] queue             The queue which is checked for valid orders
 * @param[in] current_floor     The floor of which we check for valid orders
 * @param[in] last_dir          The last direction of the elevator, for validity check
 * 
 * @return 1 if any @c Order in @p queue has a valid order, and 0 if no @c Order
 * is valid for the @p current_floor .
 * 
 * The function checks if any @c Order in the @p queue has an order set for the @p current_floor .
 * @p last_dir is used to check whether or not we should stop for it. We also check if any
 * of the orders have a cab-order that we can handle.
 */
int check_order_match(Order* queue, int current_floor, HardwareMovement last_dir);


Order initialize_new_order();

// /* ALT DETTE KAN LIGGE I UTILITIES OG I QUEUE
// Men det er bedre at det ligger i queue, da alle disse 
// funskjonene opererer på queue/ordrerekkefølge
// /**
// * @brief Reset the order's to a pointer containing only 
// * invalid orders. In this case, we use -1 to symbolize
// * invalid floors/orders.
// *
// * @param[in] p_order The pointer we would like to reset/
// * invalidate
// */
// //void reset_orders(int* p_order);

#endif