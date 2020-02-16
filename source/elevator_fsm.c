#include "elevator_fsm.h"

int update_state(elevator_data_t* p_elevator_data, time_t* p_door_timer) {

    int current_floor = at_floor();
    
    Order current_order = QUEUE[0];

    // If the stop signal is high, we return right away, no matter what
    
    switch(p_elevator_data->state) {
        case STATE_IDLE: {
            // Entry action
            if(p_elevator_data->last_dir != HARDWARE_MOVEMENT_STOP){
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                p_elevator_data->last_dir = HARDWARE_MOVEMENT_STOP;
            }
            
            if(queue_is_empty(QUEUE)) {
                return ACTION_DO_NOTHING;
            }

            if(hardware_read_obstruction_signal() == 1 && p_elevator_data->door_open == DOOR_OPEN) {
                return ACTION_START_DOOR_TIMER;
            }
    

            return ACTION_DO_NOTHING; // If no valid transitions, do nothing and remain in STATE_IDLE
        }

        case STATE_DOOR_OPEN: {
            // Entry actions:
            hardware_command_door_open(DOOR_OPEN);

            if(p_elevator_data->door_open == DOOR_CLOSE && current_order.target_floor != INVALID_ORDER) {
                        
                if(current_floor == current_order.target_floor && current_floor != -1){
                    update_target_floor(&current_order, current_floor);
                    p_elevator_data->state = STATE_DOOR_OPEN;
                    return ACTION_START_DOOR_TIMER;
                }
                
                if(current_floor < current_order.target_floor && current_floor != -1) {
                    p_elevator_data->state = STATE_MOVING_UP;
                    return ACTION_CHECK_OBSTRUCTION;
                }

                if(current_floor > current_order.target_floor && current_floor != -1) {
                    p_elevator_data->state = STATE_MOVING_DOWN;
                    return ACTION_CHECK_OBSTRUCTION;
                }
            }

            // Exit actions:
        }

        case STATE_MOVING_UP: {
            // Entry actions:
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            if(current_floor > HARDWARE_NUMBER_OF_FLOORS && current_floor != -1) {
                p_elevator_data->state = STATE_IDLE;
                return ACTION_STOP_MOVEMENT;
            }


            // Exit condition
            if(check_order_match(current_floor, p_elevator_data->last_dir) == 1){
                update_target_floor(&current_order, current_floor);
                clear_cab_orders(current_floor);
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);

                p_elevator_data->state = STATE_IDLE;
                return ACTION_OPEN_DOOR;
            }
        
            
            break;
        }


        case STATE_MOVING_DOWN: {
            if(current_floor <= MIN_FLOOR && current_floor != -1) {
                p_elevator_data->state = STATE_IDLE;
                return ACTION_STOP_MOVEMENT;
            }

            if(check_order_match(current_floor, p_elevator_data->last_dir)) {
                // Here we have found an order at the current floor that we can stop for!
                update_target_floor(&current_order, current_floor);
                clear_cab_orders(current_floor);

                p_elevator_data->state = STATE_IDLE;
                return ACTION_OPEN_DOOR;
            }
            return ACTION_MOVE_DOWN;
        }
        

        case STATE_EMERGENCY: {
            return emergency_action(p_elevator_data, p_door_timer);
            
        }
    } // EOF switch

    return ACTION_DO_NOTHING;
}

int determine_direction(elevator_state_t* p_elevator_state, Order* p_current_order, int current_floor) {    
    if(current_floor < MIN_FLOOR || current_floor > HARDWARE_NUMBER_OF_FLOORS) {
        return -1;
    }
    
    if(current_floor < p_current_order->target_floor) {
        *p_elevator_state = STATE_MOVING_UP;
        return ACTION_CLOSE_DOOR;
    }
    else if(current_floor > p_current_order->target_floor) {
        *p_elevator_state = STATE_MOVING_DOWN;
        return ACTION_CLOSE_DOOR;
    }
    else if(current_floor == p_current_order->target_floor){
        // This is incomplete. We need to update target_floor for the current order
        // so to not be stuck in this. If we do not update target_floor, the check above
        // will always return true.

        update_target_floor(p_current_order, current_floor);
        *p_elevator_state = STATE_IDLE;
        return ACTION_START_DOOR_TIMER;
    }

    // We should never reach this point
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
