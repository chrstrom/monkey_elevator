#include <stdio.h>

#include "elevator_fsm.h"
#include "elevator_io.h"
#include "globals.h"
#include "queue.h"
#include "timer.h"


int check_floor_diff(int target_floor, int current_floor) {
    return (current_floor != target_floor && current_floor != BETWEEN_FLOORS);
}


void update_button_state(elevator_data_t* p_elevator_data){
    update_cab_buttons(p_elevator_data->orders_cab);
    update_floor_buttons(p_elevator_data->orders_up, p_elevator_data->orders_down);
}


void emergency_action(elevator_data_t* p_elevator_data){
    queue_erase(p_elevator_data->orders_up, p_elevator_data->orders_down, p_elevator_data->orders_cab);
    timer_start();
    if (get_current_floor() != BETWEEN_FLOORS && hardware_read_stop_signal()){
        p_elevator_data->door_open = DOOR_OPEN;
        hardware_command_door_open(DOOR_OPEN);
    }
}


elevator_data_t elevator_init() {
    //Turn off all button lights and clear all order light arrays (just in case)
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        hardware_command_order_light(floor, HARDWARE_ORDER_UP,     LIGHT_OFF);
        hardware_command_order_light(floor, HARDWARE_ORDER_DOWN,   LIGHT_OFF);
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, LIGHT_OFF);
    }

    //we assume the obstruction will never be active during setup!
    hardware_command_stop_light(LIGHT_OFF);
    hardware_command_door_open(DOOR_CLOSE); 

    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    while(get_current_floor() == BETWEEN_FLOORS) {}
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);

    hardware_command_floor_indicator_on(get_current_floor());

    queue_init();

    elevator_data_t elevator_data = {.door_open = DOOR_CLOSE,
                                    .last_floor = get_current_floor(),
                                    .last_dir = HARDWARE_MOVEMENT_STOP,
                                    .state = STATE_IDLE,
                                    .next_action = ACTION_STOP_MOVEMENT};

    return elevator_data;
}


void elevator_execute_next_action(elevator_data_t* p_elevator_data){
    switch (p_elevator_data->next_action){
    case ACTION_DO_NOTHING:
        break;

    case ACTION_START_DOOR_TIMER:
        timer_start();
        break;

    case ACTION_OPEN_DOOR:
        hardware_command_door_open(DOOR_OPEN);
        p_elevator_data->door_open = DOOR_OPEN;
        break;

    case ACTION_CLOSE_DOOR:
        queue_update();
        hardware_command_door_open(DOOR_CLOSE);
        p_elevator_data->door_open = DOOR_CLOSE;
        break;

    case ACTION_MOVE_UP:
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        p_elevator_data->last_dir = HARDWARE_MOVEMENT_UP;
        p_elevator_data->state = STATE_MOVING_UP;
        break;

    case ACTION_MOVE_DOWN:
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        p_elevator_data->last_dir = HARDWARE_MOVEMENT_DOWN;
        p_elevator_data->state = STATE_MOVING_DOWN;
        break;

    case ACTION_STOP_MOVEMENT:
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        p_elevator_data->last_dir = HARDWARE_MOVEMENT_STOP;
        p_elevator_data->state = STATE_IDLE;
        break;

    case ACTION_EMERGENCY:
        emergency_action(p_elevator_data);
        break;

    default:
        fprintf(stderr, "Default case reached in switch in main. This should not happen\n");
        break;
    }
}


elevator_guard_t elevator_update_guards(elevator_data_t* p_elevator_data) {
    elevator_guard_t guards;

    int target = QUEUE[0].target_floor;
    int current_floor = get_current_floor();
    int last_valid_floor = p_elevator_data->last_floor;
                  
    guards.DIRECTION = queue_check_order_match(current_floor, p_elevator_data->last_dir);                  
    guards.AT_FLOOR = (current_floor != BETWEEN_FLOORS);              
    guards.NOT_AT_FLOOR = (current_floor == BETWEEN_FLOORS);
    guards.TIMER_DONE = timer_check(DOOR_TIME_REQ);

    // Normal check for the usual case (elevator at floor)
    if(current_floor != BETWEEN_FLOORS) {
        guards.TARGET_FLOOR_ABOVE = (target > current_floor);
        guards.TARGET_FLOOR_EQUAL = (target == current_floor);
        guards.TARGET_FLOOR_BELOW = (target < current_floor);
        return guards;
    }

    // Perform no checks for invalid orders
    if(target == FLOOR_NOT_INIT) {
        guards.TARGET_FLOOR_ABOVE = 0;       
        guards.TARGET_FLOOR_EQUAL = 0;    
        guards.TARGET_FLOOR_BELOW = 0;
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


elevator_event_t elevator_update_event(elevator_data_t* p_elevator_data) {
    // Update truth values for all possible events
    int queue_is_empty = queue_empty();
    int target_floor_diff = check_floor_diff(QUEUE[0].target_floor, p_elevator_data->last_floor);
    int floor_match = queue_check_order_match(get_current_floor(), p_elevator_data->last_dir);
    int obstruction_state = hardware_read_obstruction_signal();
    int stop_button_state = hardware_read_stop_signal();
    int timer_done = timer_check(DOOR_TIME_REQ);

    switch(p_elevator_data->state) {
        case STATE_IDLE: {
            if(stop_button_state) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(queue_is_empty) {
                return EVENT_QUEUE_EMPTY;
            }
            if(!queue_is_empty) {
                return EVENT_QUEUE_NOT_EMPTY;
            }
            break;
        }                        
        case STATE_DOOR_OPEN: {
            if(stop_button_state) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(obstruction_state) {
                return EVENT_OBSTRUCTION_HIGH;
            }
            if(queue_is_empty && timer_done) {
                return EVENT_QUEUE_EMPTY;
            }
            if(target_floor_diff) {
                return EVENT_TARGET_FLOOR_DIFF;
            }
            break;
        }
        case STATE_MOVING_UP: {
            if(stop_button_state) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(floor_match) {
                return EVENT_FLOOR_MATCH;
            }

            break;
        }        
        case STATE_MOVING_DOWN: {
            if(stop_button_state) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(floor_match) {
                return EVENT_FLOOR_MATCH;
            }
            break;
        }             
        case STATE_EMERGENCY: {
            if(stop_button_state) {
                hardware_command_stop_light(LIGHT_ON);
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(!stop_button_state) {
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
            queue_clear_order_at_floor(p_elevator_data->orders_cab, p_elevator_data->orders_up, p_elevator_data->orders_down, current_floor);

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