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
 * @brief Clear a given @c Order of a cab order at a given floor
 * 
*/
void clear_cab_orders(Order* p_current_order, int current_floor);


void add_floor_orders(int* p_up, int* p_down);


/* ALT DETTE KAN LIGGE I UTILITIES OG I QUEUE
Men det er bedre at det ligger i queue, da alle disse 
funskjonene opererer på queue/ordrerekkefølge
/**
* @brief Reset the order's to a pointer containing only 
* invalid orders. In this case, we use -1 to symbolize
* invalid floors/orders.
*
* @param[in] p_order The pointer we would like to reset/
* invalidate
*/
//void reset_orders(int* p_order);
/*
/**
* @brief Reset the order's to a pointer containing only 
* invalid orders. In this case, we use -1 to symbolize
* invalid floors/orders.
*
* @param[in] p_order The pointer we would like to reset/
* invalidate
*/
/*
/**
* @brief Add floors the elevator should visit. The function will differentiate
* between orders going up and orders going down. 
*
* @param[in] p_up Elevator queue up
*
* @param[in] p_down Elevator queue down
*/
//void add_floor_orders(int* p_up, int* p_down);

/*
/**
* @brief Add the floor orders to the cab.
*
* @param[in] p_cab Floor-orders for the cab
*/
//void add_cab_orders(int* p_cab);

#endif