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
// If queue[0].target_floor == current_floor
    // """Handle the cab orders"""
// If queue[0].target_floor > current_floor
    // MOVE UP
// If queue[0].target_floor < current_floor
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
    //STATE_HANDLE_ORDER, 
    //STATE_SERVE_ORDER,
    //STATE_ERROR
} elevator_state_t;


/**
 * @brief Update the elevator state
 * 
 * @param[in][out]  p_elevator_state    A pointer to the elevator state, updates on transitions.
 * @param[in][out]  p_door_timer        A pointer to the door_timer, used to control the door open/close sequence
 * @param[in][out]  p_queue             A pointer to the elevator's queue                     
 * @param[in]       last_dir            The last direction of the elevator
 * @param[in]       last_floor          The last floor the elevator was at
 * @param[in]       p_door_open         A pointer to an int representing whether or not the door is open
 * 
 * @return One of the possible commands resulting from the current state.
 * 
 * This function updates the elevator's state machine, and yields a resulting
 * function to be executed for any given state. It contains most of the logic flow
 * used to control the elevator's movements, depending on the given inputs.
 */
int update_state(elevator_state_t* p_elevator_state,
                 time_t* p_door_timer,
                 Order* p_queue,
                 HardwareMovement last_dir,
                 int last_floor,
                 int* p_door_open);


#endif
