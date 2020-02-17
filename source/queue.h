/**
 * @file
 * @brief Library for managing the elevator's QUEUE system
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "driver/hardware.h"
#include "elevator_fsm.h"
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

#define INVALID_ORDER -2

// We choose to make these arrays global, as we operate on them
// in almost every single major function. The alternative would be to pass
// pointers to every function, which in this case here unneccesarily clutters
// the code.
// Problem with threading/compiler optimization?

// En cab-knapp == opp OG ned trykket i samme etasje
// int ORDERS_UP[HARDWARE_NUMBER_OF_FLOORS] = {0, 0, 0, 0};
// int ORDERS_DOWN[HARDWARE_NUMBER_OF_FLOORS] = {0, 0, 0, 0};
// int ORDERS_CAB[HARDWARE_NUMBER_OF_FLOORS] = {0, 0, 0, 0};

Order QUEUE[QUEUE_SIZE];


/**
 * @brief Initialize the @c QUEUE with all invalid orders
 */
void init_queue();


/**
 * @brief Update a QUEUE by removing the first element.
 */
void update_queue();

/**
 * @brief Add orders to the @c QUEUE if an existing order with the same parameters is not in it
 
 * 
 * The function checks each element in the  @c QUEUE, and pushes a new @c Order
 * to the @c QUEUE with the corresponding ...
 */
void add_order_to_queue(int target_floor, HardwareOrder order_type);

/**
 * @brief Checks the @c QUEUE for an order with specified parameters
 *
 * @param[in] floor     The floor to check the QUEUE for
 * @param[in] dir       The direction to check the QUEUE for
 * 
 * @return 1 if the QUEUE contains a matching order, and 0 if it does not
 */
int check_queue_for_order(int target_floor, HardwareOrder order_type);

/**
 * @brief Empty the QUEUE by removing all elements
 */
void erase_queue(elevator_data_t* data);


/**
 * @brief Clear the order by removing all elements
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
void set_cab_orders(elevator_data_t* data);


/**
 * @brief Clear a cab order for a given floor
 * 
 * @param[in]  current_floor    The floor to be used for clearing the cab orders
 */
void clear_cab_orders(elevator_data_t* data, int current_floor);


void clear_orders_at_floor(elevator_data_t* data, int current_floor);


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
 * of the orders have a cab-order that we can handle. A cab order will ALWAYS be handled if the
 * elevator drives past it.
 */
int check_order_match(elevator_data_t* data, int current_floor, HardwareMovement last_dir);

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