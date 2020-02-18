/**
 * @file
 * @brief Interface for the elevator's finite-state machine.
 */

#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

#include "driver/hardware.h"
#include <time.h>


/**
 * Enum for the possible states of the FSM.
 */
typedef enum{
    STATE_IDLE,                 /**< Elevator standing still*/
    STATE_DOOR_OPEN,            /**< Elevator's door is open, handling a floor order */
    STATE_MOVING_UP,            /**< Elevator moving up*/
    STATE_MOVING_DOWN,          /**< Elevator moving down*/
    STATE_EMERGENCY             /**< Elevator !!EMERGENCY!!*/
} elevator_state_t;


/**
 * Enum for the possible events that can occur during runtime 
 */
typedef enum{
    EVENT_QUEUE_EMPTY,          /**< No valid elements in the queue*/
    EVENT_QUEUE_NOT_EMPTY,      /**< At least 1 valid element has been added to the queue*/
    EVENT_TARGET_FLOOR_DIFF,    /**< We have a target floor that is not at the current floor*/
    EVENT_FLOOR_MATCH,          /**< We have a target floor at the current floor*/
    EVENT_OBSTRUCTION_HIGH,     /**< The obstruction signal is high*/
    EVENT_STOP_BUTTON_HIGH,     /**< The stop button is high/is pressed in*/
    EVENT_STOP_BUTTON_LOW,      /**< The stop button is low/is not pressed in*/
    EVENT_NO_EVENT,             /**< No particular event has occured*/
} elevator_event_t;


/**
 * Enum for the possible actions to be set to execute by the FSM.
 */
typedef enum {
    ACTION_DO_NOTHING,          /**< Do nothing, corresponding to " - " in the state diagram output slot*/
    ACTION_START_DOOR_TIMER,    /**< Start the door timer*/
    ACTION_OPEN_DOOR,           /**< Open the elevator's doors*/
    ACTION_CLOSE_DOOR,          /**< Close the elevator's doors*/
    ACTION_MOVE_UP,             /**< Start moving upwards*/
    ACTION_MOVE_DOWN,           /**< Start moving downwards*/
    ACTION_STOP_MOVEMENT,       /**< Halt movement*/
    ACTION_EMERGENCY            /**< EMERGENCY-actions*/
} elevator_action_t;


/**
 * A struct holding all the data related to the elevator 
 */
typedef struct{
    int door_open;                              /**< An int representing the door's state: 1 = open, 0 = closed*/
    int last_floor;                             /**< An int holding the elevator's last valid floor*/
    HardwareMovement last_dir;                  /**< The last direction the elevator was moving in*/
    elevator_state_t state;                     /**< The state of the elevator*/
    elevator_action_t next_action;              /**< The next action to be performed by the elevator*/
    int ORDERS_UP[HARDWARE_NUMBER_OF_FLOORS];   /**< The elevator's orders going up*/
    int ORDERS_DOWN[HARDWARE_NUMBER_OF_FLOORS]; /**< The elevator's orders going down*/
    int ORDERS_CAB[HARDWARE_NUMBER_OF_FLOORS];  /**< The elevator's cab-orders.*/
    int check_time;                             /**< An int representing if we should check the time or not. Stupid bugfix*/
} elevator_data_t;


/**
 * Struct of transition guards for the FSM. 0 = not fulfilled  1 = fulfilled.
 * If a guard is not fulfilled, any state transition depending on it will not happen.
 */
typedef struct{
    int TIMER_DONE;               /**< Guard for whether or not the timer done counting. 1 = timer is done, 0 = timer still going */
    int DIRECTION;                /**< Guard for checking matching elevator direction. 1 = going in the same direction as order, 0 = not going in same dir as order*/
    int TARGET_FLOOR_ABOVE;       /**< Guard for checking target floor location, 1 = target floor above current floor. 0 = target floor at/below current floor*/
    int TARGET_FLOOR_EQUAL;       /**< Guard for checking target floor location, 1 = target floor at current floor. 0 = target floor not at current floor*/
    int TARGET_FLOOR_BELOW;       /**< Guard for checking target floor location, 1 = target floor below current floor. 0 = target floor at/above current floor */
    int AT_FLOOR;                 /**< Guard for checking elevator location. 1 = elevator at floor. 0 = elevator not at floor*/
    int NOT_AT_FLOOR;             /**< Guard for checking elevator location. 1 = elevator not at floor. 0 = elevator at floor*/
} elevator_guard_t;

// Could also add guards for "BELOW_MIN_FLOOR" and "ABOVE_MAX_FLOOR"


/**
 * @brief Update the elevator state
 * 
 * @param[in, out] p_elevator_data     A pointer to the elevator data, updates on transitions.
 * @param[in, out] p_door_timer        A pointer to the door_timer, used to control the door open/close sequence                  
 * 
 * @return One of the possible commands resulting from the current state.
 * 
 * This function updates the elevator's state machine, and yields a resulting
 * function to be executed for any given state. It contains most of the logic flow
 * used to control the elevator's movements, depending on the given inputs.
 */
int update_state(elevator_data_t* p_elevator_data, time_t* p_door_timer);


/**
 * @brief Calculate the next event, based on the elevator's current state @c elevator_data_t . The events will be used
 * to differentiate between the states in the fsm.
 * 
 * @param[in] p_elevator_data Pointer to an @c elevator_data_t that contains the required data to calculate the event
 * 
 * @return One of the possible events resulting from the elevator's state
 */ 
elevator_event_t elevator_calculate_event(elevator_data_t* p_elevator_data);


/**
 * @brief Calculate the next guards. The guards will be used to differentiate between the events in the fsm
 * 
 * @param[in] p_elevator_data Pointer to an @c elevator_data_t that contains the required data to calculate the guards
 * 
 * @return All of the guards, depending on the elevator's state, input and queue
 */ 
elevator_guard_t elevator_calculate_guard(elevator_data_t* p_elevator_data, time_t* p_door_timer);


/**
 * @brief Check if the @p current_floor is equal to @p target_floor
 * 
 * @param[in] current_floor The floor the elevator is at 
 * 
 * @param[in] target_floor The current order's target_floor
 */ 
int check_floor_diff(int target_floor, int current_floor);


/**
 * @brief Solve the different tasks that much be done if the elevator is in an emergency. This includes
 * deleting the queue, making sure the engine is stopped, and open the door if the elevator is at a floor
 * 
 * @param[in/out] p_elevator_data   Pointer to the @c elevator_data that contain fundamental data about 
 * the elevator
 * @param[in] p_door_timer          Pointer to the time. Used to check if a certain amount of time has passed
 */     
void emergency_action(elevator_data_t* p_elevator_data, time_t* p_timer);            

#endif //ELEVATOR_FSM_H
