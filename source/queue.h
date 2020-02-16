/**
 * @file
 * @brief Library for managing the elevator's QUEUE system
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "includes.h"

#define SIZEOF_ARR(X) sizeof(*X)/sizeof(X[0])

/**
 * @struct Order
 * 
 * @brief  A struct for holding information about an order
 */
typedef struct{
    int target_floor;                   /**< The floor at which the order comes from */
    HardwareMovement dir;               /**< The direction of the order */
} Order;

#define INVALID_ORDER -2

// We choose to make these arrays global, as we operate on them
// in almost every single major function. The alternative would be to pass
// pointers to every function, which in this case here unneccesarily clutters
// the code.
// Problem with threading/compiler optimization?

// En cab-knapp == opp OG ned trykket i samme etasje
static int ORDERS_UP[HARDWARE_NUMBER_OF_FLOORS] = {0, 0, 0, 0};
static int ORDERS_DOWN[HARDWARE_NUMBER_OF_FLOORS] = {0, 0, 0, 0};
static int ORDERS_CAB[HARDWARE_NUMBER_OF_FLOORS] = {0, 0, 0, 0};

static Order QUEUE[QUEUE_SIZE] = {{.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP},
                                  {.target_floor = INVALID_ORDER, .dir = HARDWARE_MOVEMENT_STOP}};


/**
 * @brief Update a QUEUE by removing the first element.
 */
void update_queue();

/**
 * @brief Add orders to the @p p_queue according to the @c ORDERS_UP and @c ORDERS_DOWN arrays
 
 * 
 * The function checks each element in the  @c QUEUE, and pushes a new @c Order
 * to it for each truthy value in @c ORDERS_UP and @c ORDERS_DOWN , if the @c QUEUE does not
 * have an @c Order for either of them already.
 */
void add_order_to_queue();

/**
 * @brief Checks the @c QUEUE for an order with specified parameters
 *
 * @param[in] floor     The floor to check the QUEUE for
 * @param[in] dir       The direction to check the QUEUE for
 * 
 * @return 1 if the QUEUE contains a matching order, and 0 if it does not
 */
int check_queue_for_order(int floor, HardwareMovement dir);

/**
 * @brief Empty the QUEUE by removing all elements
 */
void erase_queue();


/**
 * @brief Clear the order by removing all elements
 *
 */
void erase_order();


/**
 * @brief Check if the QUEUE is empty
 * 
 * @return 1 if empty and 0 if not
 */
int queue_is_empty();


/**
 * @brief Set the cab orders according to cab buttons pressed
 * 
 * Whenever we are idle at a floor and are accepting orders, we need to set the cab orders.
 */
void set_cab_orders();


/**
 * @brief Clear a cab order for a given floor
 * 
 * @param[in]  current_floor    The floor to be used for clearing the cab orders
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
void update_target_floor(Order* p_current_order, int current_floor);


/**
 * @brief Check if the QUEUE has a valid order to handle  
 * 
 * @param[in] current_floor     The floor of which we check for valid orders
 * @param[in] last_dir          The last direction of the elevator, for validity check
 * 
 * @return 1 if any @c Order in @c QUEUE has a valid order, and 0 if no @c Order
 * is valid for the @p current_floor .
 * 
 * The function checks if any @c Order in the @c QUEUE has an order set for the @p current_floor .
 * @p last_dir is used to check whether or not we should stop for it. We also check if any
 * of the orders have a cab-order that we can handle.
 */
int check_order_match(int current_floor, HardwareMovement last_dir);

/**
 * @brief Pushes a new @c Order to the @c QUEUE
 * 
 * @param[in]       floor   The floor that the @c Order came from
 * @param[in]       dir     The direction of the @c Order
 * 
 * The function finds the first element in the QUEUE that has its @c target_floor
 * value set to -1, and updates this element with @p floor and @p dir .
 */
void push_back_queue(int floor, HardwareMovement dir);


Order initialize_new_order();

// /* ALT DETTE KAN LIGGE I UTILITIES OG I QUEUE
// Men det er bedre at det ligger i QUEUE, da alle disse 
// funskjonene opererer på QUEUE/ordrerekkefølge
// /**
// * @brief Reset the order's to a pointer containing only 
// * invalid orders. In this case, we use -1 to symbolize
// * invalid floors/orders.
// *
// * @param[in] p_order The pointer we would like to reset/
// * invalidate
// */
// //void reset_orders(int* p_order);

#endif //QUEUE_H