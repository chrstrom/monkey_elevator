#include "elevator_fsm.h"

int update_state(elevator_data_t* p_elevator_data, time_t* p_door_timer) {

    int current_floor = at_floor();
    Order current_order = QUEUE[0];
    //need some functions to calculate this exactly/more precise
    //the current way for us to calculate, must take into account the importance for some functions 
    //over other
    elevator_event_t current_event;
    elevator_guard_t current_guard;

    switch(p_elevator_data->state) {
        case STATE_IDLE: {
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            switch (current_event)
            {
            case EVENT_QUEUE_NOT_EMPTY:{
                switch (current_guard)
                {
                case GUARD_TARGET_FLOOR_ABOVE:{
                    p_elevator_data->state = STATE_MOVING_UP;
                    return ACTION_MOVE_UP;
                }
                case GUARD_TARGET_FLOOR_BELOW:{
                    p_elevator_data->state = STATE_MOVING_DOWN;
                    return ACTION_MOVE_DOWN;
                }
                case GUARD_AT_FLOOR:{
                    p_elevator_data->state = STATE_DOOR_OPEN;
                    return ACTION_START_DOOR_TIMER;
                }
                default:{
                    break;
                }
                }
            }            
            case EVENT_QUEUE_EMPTY:{
                return ACTION_DO_NOTHING;
            }
            case EVENT_STOP_BUTTON_HIGH:{
                p_elevator_data->state = STATE_EMERGENCY;
                return ACTION_EMERGENCY;
            }
            default:
                p_elevator_data->state = STATE_IDLE;
                return ACTION_DO_NOTHING;
            }
        }

        case STATE_DOOR_OPEN: {
           hardware_command_movement(HARDWARE_MOVEMENT_STOP);
           hardware_command_door_open(DOOR_OPEN);
           switch (current_event)
           {
           case EVENT_QUEUE_EMPTY:{
               switch (current_guard)
               {
                   //guard to goto correct state depending on the timer
               case GUARD_TIMER:
                   
                   break;
               
               default:
                   
                   break;
               }
               p_elevator_data->state = STATE_IDLE;
               hardware_command_door_open(DOOR_CLOSE);
               return EVENT_QUEUE_EMPTY;
           }
           case EVENT_OBSTRUCTION_HIGH: {
               p_elevator_data->state = STATE_DOOR_OPEN;
               return ACTION_CHECK_OBSTRUCTION;
           } 
           case EVENT_TARGET_FLOOR_ABOVE:{
               hardware_command_door_open(DOOR_CLOSE);
               p_elevator_data->state = STATE_MOVING_UP;
               return ACTION_MOVE_UP;
           }
           case EVENT_TARGET_FLOOR_BELOW:{
               hardware_command_door_open(DOOR_CLOSE);
               return ACTION_MOVE_DOWN;
           }
           case EVENT_STOP_BUTTON_HIGH:{
               p_elevator_data->state = STATE_EMERGENCY;
               return ACTION_EMERGENCY;
           }
           default:
               break;
           }
        }

        case STATE_MOVING_UP: {
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            switch (current_event)
            {
            case EVENT_TARGET_FLOOR_ABOVE:{
                break;
            }
            case EVENT_FLOOR_MATCH:{
                //guard to check if the floor is in the correct direction
                p_elevator_data->state = STATE_DOOR_OPEN;
                return ACTION_START_DOOR_TIMER;
            }
            case EVENT_STOP_BUTTON_HIGH:{
                p_elevator_data->state = STATE_EMERGENCY;
                return ACTION_EMERGENCY;
            }

            default:
                break;
            }
        }


        case STATE_MOVING_DOWN: {
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            switch (current_event)
            {
            case EVENT_TARGET_FLOOR_BELOW:{
                break;
            }
            case EVENT_FLOOR_MATCH:{
                //guard to check if the floor is in the correct direction
                p_elevator_data->state = STATE_DOOR_OPEN;
                return ACTION_START_DOOR_TIMER;
            }
            case EVENT_STOP_BUTTON_HIGH:{
                p_elevator_data->state = STATE_EMERGENCY;
                return ACTION_EMERGENCY;
            }
            default:
                break;
            }
        }

        case STATE_EMERGENCY: {
           hardware_command_movement(HARDWARE_MOVEMENT_STOP);
           return ACTION_EMERGENCY;
           //need to use the guards to check for the floor order and thus later
           //set the correct state with the correct definition. 
           //Will change everything in main to set the correct definition and the orders of the elevator

        }
    } // EOF switch

    return ACTION_DO_NOTHING;
}

int determine_direction(elevator_data_t* p_elevator_data, time_t* p_door_timer, int current_floor) {    
    //should be invalid, since we should only call this in defined state
    int current_floor = at_floor();
    if(current_floor < MIN_FLOOR || current_floor > HARDWARE_NUMBER_OF_FLOORS) {
        return -1;
    }
    
    if(current_floor < current_order.target_floor) {
        p_elevator_data->state = STATE_MOVING_UP;
        return ACTION_MOVE_UP;

    }
    else if(current_floor > current_order.target_floor) {
        p_elevator_data->state = STATE_MOVING_DOWN;
        return ACTION_MOVE_DOWN;

    }
    else if(current_floor == current_order.target_floor){
        p_elevator_data->state = STATE_DOOR_OPEN;
        start_timer(p_door_timer);
        return ACTION_OPEN_DOOR;
    }
    return -1;
}

int emergency_action(elevator_data_t* p_elevator_data, time_t* p_door_timer){
    if(hardware_read_stop_signal()){
        erase_queue(QUEUE);
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        start_timer(p_door_timer);
        if (at_floor() != -1){
            p_elevator_data->door_open = DOOR_OPEN;
            hardware_command_door_open(DOOR_OPEN);
        }
        p_elevator_data->state = STATE_EMERGENCY;
    }

    else{
        p_elevator_data->state = STATE_IDLE;
        if(at_floor() == -1){
            return ACTION_DO_NOTHING;
        }
        else if(check_timer(p_door_timer, NORMAL_WAIT_TIME) == 1){
            return ACTION_CHECK_OBSTRUCTION;
        }
       
    }

}

int obstruction_check(time_t* p_door_timer, int* p_door_open){
    if(hardware_read_obstruction_signal()){
        return ACTION_CHECK_OBSTRUCTION;
    }
    if(check_timer(p_door_timer, NORMAL_WAIT_TIME) == 1 && *p_door_open == DOOR_OPEN){
        return ACTION_CLOSE_DOOR;
    }
    return ACTION_CHECK_OBSTRUCTION;
}
