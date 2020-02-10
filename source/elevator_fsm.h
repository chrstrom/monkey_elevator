/**
 * @file
 * @brief Interface for the elevator's finite-state machine.
 */

#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

#include "includes.h"
#include "queue.h"
#include "timer.h"
#include "elevator_io.h"
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

/**
 * Enum containing the possible states of the FSM 
 */
typedef enum{
    STATE_IDLE,         /**< Elevator standing still*/
    STATE_MOVING_UP,    /**< Elevator moving up*/
    STATE_MOVING_DOWN,  /**< Elevator moving down*/
    STATE_PREP_MOVE,    /**< Elevator preparing to move in any direction*/
    //STATE_SERVE_ORDER,
    //STATE_ERROR
} elevator_state_t;


/**
 * @brief Update the elevator state
 * 
 * @param[in][out]  p_elevator_state    A pointer to the elevator state, updates on transitions.
 * @param[in][out]  p_ref_time,         Not sure what this is
 * @param[in][out]  queue               
 * @param[in]       last_dir
 * @param[in]       last_floor
 */
int update_state(elevator_state_t* p_elevator_state,
                 time_t* p_ref_time,
                 Order* p_queue,
                 HardwareMovement last_dir,
                 int last_floor);


#endif
