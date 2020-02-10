#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H


// List of all possible resulting commands from the fsm
#define 
#define START_DOOR_TIMER 2


// In idle state:
// If queue empty, do nothing
// If queue[0].floor_at == current_floor
    // """Handle the cab orders"""
// If queue[0].floor_at > current_floor
    // MOVE UP
// If queue[0].floor_at < current_floor
    // MOVE DOWN

typedef enum{
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
} elevator_state_t;

int update_state(elevator_state_t* current_state);

// static int state_transition_table[][];
#endif
