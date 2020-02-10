/**
* @file
* @brief Updating the elevator's queue. 
*/
#ifndef QUEUE_H
#define QUEUE_H

#include "includes.h"

typedef struct{
    int floor_at;               // Floor at which the floor button was pressed
    int floor_to[MAX_FLOOR];    // The cab orders
    int dir;
} Order;

/**
 * @brief Update a queue by removing the first element.
 * 
 * @param[in] p_queue Pointer to the queue.
*/
void update_queue(Order* p_queue);

/**
 * @brief Set the cab orders for a given @c Order
 * 
 * @param[in][out] p_current_order  A pointer to the @c Order to which the cab orders will be set
 * 
 * Upon arrival at the target floor for an @c Order , we need to set the cab orders for this current order.
*/
void set_cab_orders(Order* p_current_order);


/**
 * @brief Clear a given @c Order of a cab order for a given floor
 * 
 * @param[out] p_queue  A pointer to the queue
 * @param[in]  floor    The floor to be used for clearing the cab orders in the 
 * 
 * Any given order can have MAX_FLOOR amount of cab orders. Upon arrival at a floor,
 * all cab orders for the queue shall be considered handled; thereby clearing the order.
*/
void clear_cab_orders(Order* p_queue, int floor);



#endif