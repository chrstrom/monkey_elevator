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


/**
 * Enum containing the possible states of the FSM 
 */
typedef enum{
    STATE_IDLE,         /**< Elevator standing still*/
    STATE_DOOR_OPEN,    /**< Elevator's door is open, handling a floor order */
    STATE_MOVING_UP,    /**< Elevator moving up*/
    STATE_MOVING_DOWN,  /**< Elevator moving down*/
    STATE_EMERGENCY     /**< Elevator !!EMERGENCY!!*/
} elevator_state_t;


typedef enum{
    EVENT_QUEUE_EMPTY,
    EVENT_QUEUE_NOT_EMPTY,
    EVENT_TARGET_FLOOR_ABOVE,
    EVENT_TARGET_FLOOR_BELOW,
    EVENT_FLOOR_MATCH,
    EVENT_OBSTRUCTION_HIGH,
    EVENT_STOP_BUTTON_HIGH,
    EVENT_STOP_BUTTON_LOW,
} elevator_event_t;


typedef enum {
    ACTION_DO_NOTHING,
    ACTION_CHECK_OBSTRUCTION,
    ACTION_START_DOOR_TIMER,
    ACTION_OPEN_DOOR,             
    ACTION_CLOSE_DOOR,
    ACTION_MOVE_UP,
    ACTION_MOVE_DOWN,
    ACTION_STOP_MOVEMENT,
    ACTION_EMERGENCY,
    ACTION_NOT_EMERGENCY,
    ACTION_NOT_OBSTRUCTION,
    ACTION_OBSTRUCTION
} elevator_action_t;


typedef enum{
    GUARD_TIMER,
    GUARD_DIRECTION,
    GUARD_TARGET_FLOOR_BELOW,
    GUARD_TARGET_FLOOR_ABOVE,
    GUARD_TARGET_FLOOR_EQUAL,
    GUARD_AT_FLOOR,
    GUARD_NOT_AT_FLOOR
} elevator_guard_t;


// Med denne structen kan vi kanskje også holde alle arrays med knapper, samt queue?
typedef struct{
    int door_open;
    int last_floor;
    HardwareMovement last_dir;
    elevator_state_t state;
    elevator_action_t next_action;
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
int determine_direction(elevator_data_t* p_elevator_data, Order* p_current_order, int current_floor);

/**
 * @brief Solve the different tasks that much be done if the elevator is in an emergency. This includes
 * deleting the queue, making sure the engine is stopped, and open the door if the elevator is at a floor
 * 
 * @param[in/out] p_elevator_data Pointer to the @c elevator_data that contain fundamental data about 
 * the elevator
 * @param[in] p_door_timer Pointer to the time. Used to check if a certain amount of time has passed
 *
 * @return The function returns the next action the elevator will perform next. 
 */ 
int emergency_action(elevator_data_t* p_elevator_data, time_t* p_door_timer);            

/**
 * @brief The function will check if the obstruction is activated and if 3 seconds has passed. If the obstruction
 * is activated, it will reset the timer. 
 * 
 * @param[in] p_door_timer Pointer to @c time_t containing the time since the door was opened. Will be reset
 * if the obstruction is high
 * @param[in/out] p_door_open Pointer to the door. Will be closed if a certain amount of time has passed since
 * the obstruction went low, or the door was opened. 
 */ 
int obstruction_check(time_t* p_door_timer, int* p_door_open);

elevator_event_t calculate_next_event(elevator_data_t* p_elevator_data);

#endif
