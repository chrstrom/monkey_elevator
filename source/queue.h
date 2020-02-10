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

// Set the cab orders for the current_order
void set_cab_orders(Order* current_order);

// Sets the element corresponding to current_floor in EVERY ORDER'S
// floor_to[] to 0
void clear_cab_orders(Order* queue, int current_floor);


void add_floor_orders(int* p_up, int* p_down) {

}


int at_floor() {
    // returns the floor the elevator is at, or 0 if the elevator is between floors
}

#endif