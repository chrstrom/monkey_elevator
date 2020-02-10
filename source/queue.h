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
    int dir;                    // 1 = up, -1 = down
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
*/
void set_cab_orders(Order* p_current_order);

/**
 * @brief Clear a given @c Order of a cab order  a given floor
 * 
*/
void clear_cab_orders(Order* p_current_order, int current_floor);


void add_floor_orders(int* p_up, int* p_down) {

}


#endif