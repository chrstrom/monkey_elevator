#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

// In idle state:
// If queue empty, do nothing
// If queue[0].floor_at == current_floor
    // """Handle the cab orders"""
// If queue[0].floor_at > current_floor
    // MOVE UP
// If queue[0].floor_at < current_floor
    // MOVE DOWN
/**
 * List of all possible resulting commands from the fsm
 */
#define DO_NOTHING 0
#define CHECK_OBSTRUCTION 1
#define START_DOOR_TIMER 2
#define OPEN_DOOR 3
#define CLOSE_DOOR 4
#define MOVE_UP 5
#define MOVE_DOWN 6
#define STOP_MOVEMENT 7

#include "includes.h"
#include "queue.h"
#include "timer.h"
#include "elevator_io.h"

typedef enum{
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_PREP_MOVE,
    //STATE_SERVE_ORDER,
    //STATE_ERROR
} elevator_state_t;



int update_state(elevator_state_t* p_elevator_state, time_t* p_ref_time, Order* queue);


#endif
