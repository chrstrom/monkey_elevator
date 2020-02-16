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
// If QUEUE empty, do nothing
// If QUEUE[0].target_floor == current_floor
    // """Handle the cab orders"""
// If QUEUE[0].target_floor > current_floor
    // MOVE UP
// If QUEUE[0].target_floor < current_floor
    // MOVE DOWN

/**
 * List of all possible resulting commands from the fsm
 */
#define ACTION_DO_NOTHING 0
#define ACTION_CHECK_OBSTRUCTION 1
#define ACTION_START_DOOR_TIMER 2
#define ACTION_OPEN_DOOR 3             
#define ACTION_CLOSE_DOOR 4
#define ACTION_MOVE_UP 5
#define ACTION_MOVE_DOWN 6
#define ACTION_STOP_MOVEMENT 7
#define ACTION_EMERGENCY 8
#define ACTION_NOT_EMERGENCY 9
#define ACTION_NOT_OBSTRUCTION 10
#define ACTION_OBSTRUCTION 11

/**
 * Enum containing the possible states of the FSM 
 */
typedef enum{
    STATE_IDLE,         /**< Elevator standing still*/
    STATE_MOVING_UP,    /**< Elevator moving up*/
    STATE_MOVING_DOWN,  /**< Elevator moving down*/
} elevator_state_t;

// Med denne structen kan vi kanskje også holde alle arrays med knapper, samt queue?
typedef struct{
    int door_open;
    int next_action;
    int last_floor;
    HardwareMovement last_dir;
    elevator_state_t state;
} elevator_data_t;

/**
 * @brief Update the elevator state
 * 
 * @param[in][out]  p_elevator_data    A pointer to the elevator data, updates on transitions.
 * @param[in][out]  p_door_timer        A pointer to the door_timer, used to control the door open/close sequence                  
 * 
 * @return One of the possible commands resulting from the current state.
 * 
 * This function updates the elevator's state machine, and yields a resulting
 * function to be executed for any given state. It contains most of the logic flow
 * used to control the elevator's movements, depending on the given inputs.
 */
int update_state(elevator_data_t* p_elevator_data, time_t* p_door_timer);


/**
 * @brief Calculate the next direction the elevator must drive in
 * 
 * @param[in]  p_elevator_state    A pointer to the elevator state.
 * @param[in]  p_current_order     A pointer to the elevator's current order being handled                    
 * @param[in]  current_floor       The current floor of the elevator
 * 
 * @return The next direction the elevator will drive in
 * 
 * This function updates the elevator's state machine, and yields a resulting
 * function to be executed for any given state. It contains most of the logic flow
 * used to control the elevator's movements, depending on the given inputs.
 */
int determine_direction(elevator_state_t* p_elevator_state, Order* p_current_order, int current_floor);

int emergency_action(time_t* p_door_timer, int* p_door_open);                

int obstruction_check(time_t* p_door_timer, int* p_door_open);

#endif
