/**
* @file
* @brief Updating the elevator's queue. 
*/

#ifndef QUEUE_H
#define QUEUE_H

#include "includes.h"

typedef struct {
    int floor_at;
    int floor_to;
    int next_action;
} task_t;

/**
 * @brief Update a queue by removing the first element.
 * 
 * @param[in] p_queue Pointer to the queue.
*/
void update_queue(int* p_queue);

/**
 * @brief Add a new order to the back of the queue.
 * 
 * @param[in] floor_at The floor the elevator goes from
 * 
 * @param[in] floor_to Where the elevator will go to
 * 
 * @param[in] cmd The nececarry command to do the task
 * 
 * @param[in] p_tasks Pointer to the tasks-queue. Will add
 * to the back of this queue
*/
void queue_push_back(int floor_at, int floor_to, CmdType_t cmd, task_t* p_tasks);

/**
 * @brief Add a new order to the front of the queue.
 * 
 * @param[in] floor_at The floor the elevator goes from
 * 
 * @param[in] floor_to Where the elevator will go to
 * 
 * @param[in] cmd The nececarry command to do the task
 * 
 * @param[in] p_tasks Pointer to the tasks-queue. Will add
 * to the front of this queue
*/
void queue_push_front(int floor_at, int floor_to, CmdType_t cmd, task_t* p_tasks);

/**
 * @brief Update the tasks-queue and find the next task.
 * 
 * @param[in] p_tasks Pointer to the necessary tasks.
*/
void update_current_task(task_t* p_tasks);

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