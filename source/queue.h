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
* @brief Add the floor orders to the cab.
*
* @return The last button that was pressed
*/
void set_cab_orders(Order* order);

// Sets the element corresponding to current_floor in the order's floor_to[] 
// to 0
void clear_cab_order(Order* current_order, int current_floor);


#endif