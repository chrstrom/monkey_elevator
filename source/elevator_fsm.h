#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H




#define SET_FLOOR_LIGHT 1
#define START_TIMER 2
typedef enum{
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_PREP_MOVE,
    STATE_SERVE_ORDER,
    STATE_ERROR
} elevator_state_t;



int update_state(elevator_state_t* current_state);



static int state_transition_table[][];
#endif