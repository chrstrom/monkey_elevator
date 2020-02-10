#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

/**
 * List of all possible resulting commands from the fsm
 */
#define CHECK_STOP 0
#define CHECK_OBSTRUCTION 1
#define START_DOOR_TIMER 2

#include "includes.h"
#include "queue.h"
#include "timer.h"
#include "elevator_io.h"

typedef enum{
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN
    //STATE_PREP_MOVE,
    //STATE_SERVE_ORDER,
    //STATE_ERROR
} elevator_state_t;

int update_state(elevator_state_t* p_current_state, time_t* p_ref_time, Order* p_current_order);

// static int state_transition_table[][];

#endif
