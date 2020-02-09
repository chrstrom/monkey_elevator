#ifndef QUEUE_H
#define QUEUE_H
#include "includes.h"


/**
 * @brief Update a queue by removing first element
 * @param p_queue   Pointer to an int array
*/
void update_queue(int* p_queue);

void queue_push_back(CmdType_t cmd, int* p_queue);
void queue_push_front(CmdType_t cmd, int* p_queue);

void add_floor_orders(int* p_up, int* p_down);
void add_cab_orders(int* p_cab);

#endif