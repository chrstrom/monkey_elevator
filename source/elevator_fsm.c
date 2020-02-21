#include <stdio.h>
#include "elevator_fsm.h"
#include "elevator_io.h"
#include "globals.h"
#include "queue.h"
#include "timer.h"

elevator_action_t elevator_update_state(elevator_data_t* p_elevator_data) {

    int current_floor = get_current_floor();
    Order current_order = QUEUE[0];

    elevator_event_t current_event = elevator_update_event(p_elevator_data);
    elevator_guard_t guards = elevator_update_guards(p_elevator_data);

    switch(p_elevator_data->state) {
        case STATE_IDLE: {
            // Entry actions:
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            switch (current_event)  {
                case EVENT_STOP_BUTTON_HIGH:{
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_EMERGENCY;
                }

                case EVENT_QUEUE_EMPTY:{
                    p_elevator_data->state = STATE_IDLE;
                    return ACTION_DO_NOTHING;
                }
                
                case EVENT_QUEUE_NOT_EMPTY:{
                    if(guards.TARGET_FLOOR_ABOVE) {
                        p_elevator_data->state = STATE_MOVING_UP;
                        return ACTION_MOVE_UP;
                    }

                    if(guards.TARGET_FLOOR_EQUAL) {
                        p_elevator_data->state = STATE_DOOR_OPEN;
                        return ACTION_START_DOOR_TIMER;
                    }

                    if(guards.TARGET_FLOOR_BELOW) {
                        p_elevator_data->state = STATE_MOVING_DOWN;
                        return ACTION_MOVE_DOWN;
                    }
                }

                case EVENT_NO_EVENT: {
                    p_elevator_data->state = STATE_IDLE;
                    return ACTION_DO_NOTHING;
                }
            }
        }

        case STATE_DOOR_OPEN: {
            // Entry actions:
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            hardware_command_door_open(DOOR_OPEN);
            clear_orders_at_floor(p_elevator_data->orders_cab, p_elevator_data->orders_up, p_elevator_data->orders_down, current_floor);

            switch (current_event) {
                
                case EVENT_STOP_BUTTON_HIGH:{
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_EMERGENCY;
                }

                case EVENT_QUEUE_EMPTY:{
                    p_elevator_data->state = STATE_IDLE;
                    return ACTION_CLOSE_DOOR;
                }

                case EVENT_OBSTRUCTION_HIGH: {
                    p_elevator_data->state = STATE_DOOR_OPEN;
                    return ACTION_START_DOOR_TIMER;
                } 

                case EVENT_TARGET_FLOOR_DIFF: {

                    if(guards.TARGET_FLOOR_ABOVE && guards.TIMER_DONE) {
                        p_elevator_data->state = STATE_MOVING_UP;
                        hardware_command_door_open(DOOR_CLOSE);
                        p_elevator_data->door_open = DOOR_CLOSE;
                        return ACTION_MOVE_UP;
                    }

                    if(guards.TARGET_FLOOR_BELOW && guards.TIMER_DONE) {
                        p_elevator_data->state = STATE_MOVING_DOWN;
                        hardware_command_door_open(DOOR_CLOSE);
                        p_elevator_data->door_open = DOOR_CLOSE;
                        return ACTION_MOVE_DOWN;
                    }  
                }

                case EVENT_NO_EVENT: {
                    if(guards.TIMER_DONE) {
                        p_elevator_data->state = STATE_IDLE;
                        return ACTION_CLOSE_DOOR;
                    }

                    p_elevator_data->state = STATE_DOOR_OPEN;
                    return ACTION_DO_NOTHING;
                }
            }    
        }

        case STATE_MOVING_UP: {
            switch (current_event) {

                case EVENT_STOP_BUTTON_HIGH:{
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_EMERGENCY;
                }
                
                case EVENT_FLOOR_MATCH: {
                    if(guards.DIRECTION) {
                        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                        p_elevator_data->state = STATE_DOOR_OPEN;
                        return ACTION_START_DOOR_TIMER;
                    }
                }
                
                case EVENT_NO_EVENT: {
                    p_elevator_data->state = STATE_MOVING_UP;
                    return ACTION_DO_NOTHING;
                }
            }
        }

        case STATE_MOVING_DOWN: {
            switch (current_event) {
            
                case EVENT_STOP_BUTTON_HIGH:{
                    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_EMERGENCY;
                }
                
                case EVENT_FLOOR_MATCH: {
                    if(guards.DIRECTION) {
                        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                        p_elevator_data->state = STATE_DOOR_OPEN;
                        return ACTION_START_DOOR_TIMER;
                    }
                }

                case EVENT_NO_EVENT: {
                    p_elevator_data->state = STATE_MOVING_DOWN;
                    return ACTION_DO_NOTHING;
                }  
            }
        }

        case STATE_EMERGENCY: {
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            switch(current_event) {
                case EVENT_STOP_BUTTON_HIGH: {
                    return ACTION_EMERGENCY;
                }

                case EVENT_STOP_BUTTON_LOW: {
                    if(guards.TIMER_DONE && guards.AT_FLOOR) {
                        p_elevator_data->state = STATE_DOOR_OPEN;
                        return ACTION_DO_NOTHING;
                    }

                    if(guards.TIMER_DONE && !guards.AT_FLOOR) {
                        p_elevator_data->state = STATE_IDLE;
                        return ACTION_DO_NOTHING;
                    }
                }

                case EVENT_NO_EVENT: {
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_DO_NOTHING;
                }
            }
        }
    } //switch(p_elevator_data->state)
    
    return ACTION_DO_NOTHING; // Default action if no hits (shouldnt be possible to get here)
}

elevator_event_t elevator_update_event(elevator_data_t* p_elevator_data) {
    // Update truth values for all possible events
    int queue_empty = check_queue_empty();
    int target_floor_diff = check_floor_diff(QUEUE[0].target_floor, p_elevator_data->last_floor);
    int floor_match = check_order_match(get_current_floor(), p_elevator_data->last_dir);
    int obstruction_state = hardware_read_obstruction_signal();
    int stop_button_state = hardware_read_stop_signal();
    int timer_done = check_timer(DOOR_TIME_REQ);

    switch(p_elevator_data->state) {
        case STATE_IDLE: {
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(queue_empty == 1) {
                return EVENT_QUEUE_EMPTY;
            }
            if(queue_empty == 0) {
                return EVENT_QUEUE_NOT_EMPTY;
            }
            break;
        }                        
        case STATE_DOOR_OPEN: {
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(obstruction_state == 1) {
                return EVENT_OBSTRUCTION_HIGH;
            }
            if(queue_empty == 1 && timer_done == 1) {
                return EVENT_QUEUE_EMPTY;
            }
            if(target_floor_diff == 1) {
                return EVENT_TARGET_FLOOR_DIFF;
            }
            break;
        }
        case STATE_MOVING_UP: {
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(floor_match == 1) {
                return EVENT_FLOOR_MATCH;
            }

            break;
        }        
        case STATE_MOVING_DOWN: {
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(floor_match == 1) {
                return EVENT_FLOOR_MATCH;
            }
            break;
        }             
        case STATE_EMERGENCY: {
            if(stop_button_state == 1) {
                hardware_command_stop_light(LIGHT_ON);
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(stop_button_state == 0) {
                hardware_command_stop_light(LIGHT_OFF);
                return EVENT_STOP_BUTTON_LOW;
            }
            break;
        } 
        default: {
            fprintf(stderr, "Default reached in elevator_event_handler; should not happen");
        }
    }//switch(p_elevator_data->state)

    //If we get here, we did not get a hit on the specific events we checked for, so we simply do nothing
    return EVENT_NO_EVENT;
}

elevator_guard_t elevator_update_guards(elevator_data_t* p_elevator_data) {
    elevator_guard_t guards;

    int target = QUEUE[0].target_floor;
    int current_floor = get_current_floor();
    int last_valid_floor = p_elevator_data->last_floor;
                  
    guards.DIRECTION = check_order_match(current_floor, p_elevator_data->last_dir);                  
    guards.AT_FLOOR = (current_floor != BETWEEN_FLOORS);              
    guards.NOT_AT_FLOOR = (current_floor == BETWEEN_FLOORS);
    guards.TIMER_DONE = check_timer(DOOR_TIME_REQ);

    // Perform no checks for invalid orders
    if(target == FLOOR_NOT_INIT) {
        guards.TARGET_FLOOR_ABOVE = 0;       
        guards.TARGET_FLOOR_EQUAL = 0;    
        guards.TARGET_FLOOR_BELOW = 0;
        return guards;
    }

    // Normal check for the usual case (elevator at floor)
    if(current_floor != BETWEEN_FLOORS) {
        guards.TARGET_FLOOR_ABOVE = (target > current_floor);
        guards.TARGET_FLOOR_EQUAL = (target == current_floor);
        guards.TARGET_FLOOR_BELOW = (target < current_floor);
        return guards;
    }
  
    // Stopped between floors (emergency)
    if(current_floor == BETWEEN_FLOORS && p_elevator_data->next_action == ACTION_DO_NOTHING) {
        guards.TARGET_FLOOR_EQUAL = 0;
        if (p_elevator_data->last_dir == HARDWARE_MOVEMENT_UP) {
            last_valid_floor++;
            guards.TARGET_FLOOR_ABOVE = (target >= last_valid_floor);
            guards.TARGET_FLOOR_BELOW = (target < last_valid_floor);   
        }
        if (p_elevator_data->last_dir == HARDWARE_MOVEMENT_DOWN) {
            last_valid_floor--;
            guards.TARGET_FLOOR_ABOVE = (target > last_valid_floor);
            guards.TARGET_FLOOR_BELOW = (target <= last_valid_floor);   
        }
    }

    return guards;
}


void emergency_action(elevator_data_t* p_elevator_data){
    erase_queue(p_elevator_data->orders_up, p_elevator_data->orders_down, p_elevator_data->orders_cab);
    start_timer();
    if (get_current_floor() != BETWEEN_FLOORS && hardware_read_stop_signal()){
        p_elevator_data->door_open = DOOR_OPEN;
        hardware_command_door_open(DOOR_OPEN);
    }
}


int check_floor_diff(int target_floor, int current_floor) {
    return (current_floor != target_floor && current_floor != BETWEEN_FLOORS);
}

void update_button_state(elevator_data_t* p_elevator_data){
    update_cab_buttons(p_elevator_data->orders_cab);
    update_floor_buttons(p_elevator_data->orders_up, p_elevator_data->orders_down);
}

