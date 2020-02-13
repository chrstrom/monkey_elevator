/**
 * @file
 * @brief Library for managing the elevator's queue system
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "includes.h"

// !! Not 100% sure if we need to dereference the pointers in sizeof
#define SIZEOF_ARR(X) sizeof(*X)/sizeof(X[0])

static int UP_ORDERS[MAX_FLOOR + 1] = {0, 0, 0, 0};
static int DOWN_ORDERS[MAX_FLOOR + 1] = {0, 0, 0, 0};
static int CAB_ORDERS[MAX_FLOOR + 1] = {0, 0, 0, 0};

/**
 * @struct Order
 * 
 * @brief  A struct for holding information about an order
 */
typedef struct{
    int target_floor;                   /**< The floor at which the order comes from */
    HardwareMovement dir;
} Order;


/**
 * @brief Update a queue by removing the first element.
 * 
 * @param[in] p_queue Pointer to the queue.
 */
void update_queue(Order* p_queue);

/**
 * 
 */
void add_order_to_queue(Order* p_queue);

int check_queue_for_order(Order* p_queue, int floor, HardwareMovement dir);

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


/**
 * @brief Set the cab orders according to cab buttons pressed
 * 
 * Whenever we are idle at a floor and are accepting orders, we need to set the cab orders.
 */
void set_cab_orders();


/**
 * @brief Clear a cab order for a given floor
 * 
 * @param[in]  current_floor    The floor to be used for clearing the cab orders in the 
 */
void clear_cab_orders(int current_floor);


/**
 * @brief Update the @c target_floor value for a the current order 
 * 
 * @param[out] p_current_order  A pointer to the current order we are updating
 * @param[in]  floor            The floor we wish to update the order's @c target_floor value to.
 * 
 * This function "handles" part of an @c Order by changing the @c target_floor value of the current
 * order we are dealing with, to one of the floors @c CAB_ORDERS
 */
void update_queue_target_floor(Order* p_current_order, int current_floor);


/**
 * @brief Check if the queue has a valid order to handle  
 * 
 * @param[in] p_queue           The queue which is checked for valid orders
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
int check_order_match(Order* p_queue, int current_floor, HardwareMovement last_dir);

/**
 * @brief Add a new order on the back of a 
 * 
 * @param[in/out] p_queue       Pointer to the queue
 * @param[in] floor             The destination of the order
 * @param[in] dir               The direction the order will travel in
 */
void push_back_queue(Order* p_queue, int floor, HardwareMovement dir);


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