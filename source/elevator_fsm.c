#include "elevator_fsm.h"

int update_state(elevator_data_t* p_elevator_data, time_t* p_door_timer) {

    int current_floor = at_floor();
    Order current_order = QUEUE[0];
    //need some functions to calculate this exactly/more precise
    //the current way for us to calculate, must take into account the importance for some functions 
    //over other
    elevator_event_t current_event = elevator_event_handler(p_elevator_data);
    elevator_guard_t guards = elevator_guard_handler(p_elevator_data, p_door_timer);

    switch(p_elevator_data->state) {
        case STATE_IDLE: {
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
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            hardware_command_door_open(DOOR_OPEN);
            switch (current_event) {
                
                case EVENT_STOP_BUTTON_HIGH:{
                    p_elevator_data->state = STATE_EMERGENCY;
                    //hardware_command_door_open(DOOR_CLOSE); 
                    //Here we have to check if the elevator is at a floor, and then open the door
                    //might be better to have an action_emergency, that will run through every action we have to do 
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
                    // could maybe return close door and not transition
                    // and then return move up/down in the 2nd run through
                    if(guards.TARGET_FLOOR_ABOVE && guards.TIMER_DONE) {
                        p_elevator_data->state = STATE_MOVING_UP;
                        hardware_command_door_open(DOOR_CLOSE);
                        return ACTION_MOVE_UP;
                    }

                    if(guards.TARGET_FLOOR_BELOW && guards.TIMER_DONE) {
                        p_elevator_data->state = STATE_MOVING_DOWN;
                        hardware_command_door_open(DOOR_CLOSE);
                        return ACTION_MOVE_DOWN;
                    }  
                }

                case EVENT_NO_EVENT: {
                    p_elevator_data->state = STATE_DOOR_OPEN;
                    return ACTION_DO_NOTHING;
                }
            }    
        }

        case STATE_MOVING_UP: {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            switch (current_event) {

                case EVENT_STOP_BUTTON_HIGH:{
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_EMERGENCY;
                }
                
                case EVENT_FLOOR_MATCH: {
                    if(guards.DIRECTION) {
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
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            switch (current_event) {
            
                case EVENT_STOP_BUTTON_HIGH:{
                    p_elevator_data->state = STATE_EMERGENCY;
                    return ACTION_EMERGENCY;
                }
                
                case EVENT_FLOOR_MATCH: {
                    if(guards.DIRECTION) {
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
        }//
    } //switch(p_elevator_data->state)
    
    return ACTION_DO_NOTHING; // Default action if no hits (shouldnt be possible to get here)
}

elevator_event_t elevator_event_handler(elevator_data_t* p_elevator_data) {
    // Update truth values for all possible events
    int queue_empty = queue_is_empty();
    int target_floor_diff = check_floor_diff(QUEUE[0].target_floor, p_elevator_data->last_floor);
    int floor_match = check_order_match(p_elevator_data->last_floor, p_elevator_data->last_dir);
    int obstruction_state = hardware_read_obstruction_signal();
    int stop_button_state = hardware_read_stop_signal();

    // Return event depending on what state we are in;
    // The events we check for depend entirely on the state of the elevator
    switch(p_elevator_data->state) {
        case STATE_IDLE: {
            //Cases we care about: EVENT_STOP_BUTTON_HIGH, EVENT_QUEUE_EMPTY, EVENT_QUEUE_NOT_EMPTY
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
            //Cases we care about: EVENT_STOP_BUTTON_HIGH, EVENT_OBSTRUCTION_HIGH, EVENT_QUEUE_EMPTY, EVENT_TARGET_FLOOR_DIFF:
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(obstruction_state == 1) {
                return EVENT_OBSTRUCTION_HIGH;
            }
            if(queue_empty == 1) {
                return EVENT_QUEUE_EMPTY;
            }
            if(target_floor_diff == 1) {
                return EVENT_TARGET_FLOOR_DIFF;
            }
            break;
        }
        case STATE_MOVING_UP: {
            //Cases we care about: EVENT_STOP_BUTTON_HIGH, EVENT_FLOOR_MATCH
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(floor_match == 1) {
                return EVENT_FLOOR_MATCH;
            }
            break;
        }        
        case STATE_MOVING_DOWN: {
            //Cases we care about: EVENT_STOP_BUTTON_HIGH, EVENT_FLOOR_MATCH
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(floor_match == 1) {
                return EVENT_FLOOR_MATCH;
            }
            break;
        }             
        case STATE_EMERGENCY: {
            //Cases we care about: EVENT_STOP_BUTTON_HIGH, EVENT_STOP_BUTTON_LOW
            if(stop_button_state == 1) {
                return EVENT_STOP_BUTTON_HIGH;
            }
            if(stop_button_state == 0) {
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

elevator_guard_t elevator_guard_handler(elevator_data_t* p_elevator_data, time_t* p_door_timer) {
    elevator_guard_t guards;
    int floor = p_elevator_data->last_floor;
    int target = QUEUE[0].target_floor;
    int current_floor = at_floor();
    
    guards.TIMER_DONE = check_timer(p_door_timer, NORMAL_WAIT_TIME);              
    guards.DIRECTION = check_order_match(floor, p_elevator_data->last_dir);           
    guards.TARGET_FLOOR_ABOVE = (target > floor);       
    guards.TARGET_FLOOR_EQUAL = (target == floor);       
    guards.TARGET_FLOOR_BELOW = (target < floor);       
    guards.AT_FLOOR = (current_floor != -1);              
    guards.NOT_AT_FLOOR = (current_floor == -1);   

    return guards;
}

int check_floor_diff(int target_floor, int current_floor) {
    if(current_floor != target_floor && current_floor != -1) {
        return 1;
    }

    return 0; 
}

void emergency_action(elevator_data_t* p_elevator_data, time_t* p_timer){
    erase_queue();
    start_timer(p_timer);
    if (at_floor() != -1 && hardware_read_stop_signal()){
        p_elevator_data->door_open == DOOR_OPEN;
        hardware_command_door_open(DOOR_OPEN);
    }
}

// int obstruction_check(time_t* p_door_timer, int* p_door_open){
//     if(hardware_read_obstruction_signal()){
//         return ACTION_CHECK_OBSTRUCTION;
//     }
//     if(check_timer(p_door_timer, NORMAL_WAIT_TIME) == 1 && *p_door_open == DOOR_OPEN){
//         return ACTION_CLOSE_DOOR;
//     }
//     return ACTION_CHECK_OBSTRUCTION;
// }
