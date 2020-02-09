#ifndef QUEUE_H
#define QUEUE_H
#include "includes.h"


/**
 * @brief Update a queue by removing first element
 * @param p_queue   Pointer to an int array
*/

typedef struct {
    int floor_at;
    int floor_to;
    int next_action;
} task_t;

void update_queue(int* p_queue);

void queue_push_back(int floor_at, int floor_to, CmdType_t cmd, task_t* p_tasks);
void queue_push_front(int floor_at, int floor_to, CmdType_t cmd, task_t* p_tasks);

void update_current_task(task_t* p_tasks);

#endif