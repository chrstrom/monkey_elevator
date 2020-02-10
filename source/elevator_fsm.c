
#include "elevator_fsm.h"

elevator_state_t update_state(elevator_state_t* elevator_state) {
    int next_action;
    switch(elevator_state) {
        case STATE_IDLE: {
            
            // Transition into IDLE after every completed order
            // Transition out immediatly if queue is not empty
            // Remain in idle if queue is empty
            break;
        }
        case STATE_PREP_MOVE: {
            hardware_command_door_open(0);
            if(current_floor < current_order.floor_at) {
                elevator_state = STATE_MOVING_UP;
            }
            else if(current_floor > current_order.floor_at) {
                elevator_state = STATE_MOVING_DOWN;
            }
            else if(current_floor == current_order.floor_at){
                elevator_state = STATE_SERVE_ORDER;
                //start_timer(door_timer);
                next_action = 2;
            }
            else {
                elevator_state = STATE_ERROR;
            }

            break;
        }
        case STATE_MOVING_UP: {
            if(current_floor == current_order.floor_at) {
                elevator_state = STATE_SERVE_ORDER;
                //start_timer(door_timer);    //return action, can use function pointer to return
                next_action = 2;
            }
            break;  
        }
        case STATE_MOVING_DOWN: {
            if(current_floor == current_order.floor_at) {
                elevator_state = STATE_SERVE_ORDER;
                start_timer(door_timer);
            }
            break;  
        }
    
        case STATE_SERVE_ORDER: {
            // Upon arrival at the target floor
            // Stop the elevator and open the doors
            // Then, if 
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            hardware_command_door_open(1);

            set_cab_orders(&current_order); 

            if(check_timer(door_timer, NORMAL_WAIT_TIME)) {
                elevator_state = STATE_PREP_MOVE;
                    // We need to clear the cab orders for all orders,
                // corresponding to the floor we currently are at.
                // This is done when idle at each floor, right before moving
                clear_cab_orders(queue, current_floor);
                // Set floor_at to the next floor_to -element that is to be executed
            }

            break;
        }
        
    }

    return next_action;