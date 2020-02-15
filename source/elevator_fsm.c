#include "elevator_fsm.h"

int update_state(elevator_state_t* p_elevator_state, time_t* p_door_timer, HardwareMovement last_dir, int last_floor, int* p_door_open) {

    int current_floor = at_floor(); 
    Order current_order = QUEUE[0];
    hardware_command_floor_indicator_on(last_floor);

    switch(*p_elevator_state) {
        case STATE_IDLE: {
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);

            if(queue_is_empty(QUEUE)) {
                return ACTION_DO_NOTHING;
            }

            // If the QUEUE is not empty, we firstly need to check for the obstruction signal
            // and whether or not the door is open.
            if(hardware_read_obstruction_signal() && *p_door_open == DOOR_OPEN) {
                return ACTION_START_DOOR_TIMER;
            }
  
            // If the do, close the doors and start moving
            if(check_timer(p_door_timer) && !hardware_read_obstruction_signal()){ 
                // Then determine which direction we should move in
                return determine_direction(p_elevator_state, &current_order, current_floor);
 
            }
            else {
                // Not enough time has passed => Doors remain open and we do nothing
                *p_elevator_state = STATE_IDLE;
                return ACTION_DO_NOTHING;
            }

            return ACTION_DO_NOTHING;  // We shouldnt get to this point
        }

        case STATE_MOVING_UP: {
            
            if(current_floor >= HARDWARE_NUMBER_OF_FLOORS) {
                *p_elevator_state = STATE_IDLE;
                return ACTION_STOP_MOVEMENT;
            }

            // We start the loop at last_floor because we only wish to check for floors we are moving towards.
            for(int floor = last_floor; floor < HARDWARE_NUMBER_OF_FLOORS; floor++){
                if (current_floor == ORDERS_CAB[floor] && check_order_match(current_floor, last_dir)){
                    // Here we have found a valid floor to stop at!

                    update_target_floor(&current_order, current_floor);
                    clear_cab_orders(current_floor);

                    *p_elevator_state = STATE_IDLE;
                    if ((*p_door_open) == DOOR_CLOSED) {
                        hardware_command_door_open(DOOR_OPEN);
                        *p_door_open = DOOR_OPEN;
                    }

                    return ACTION_STOP_MOVEMENT;
                }
            }
            return ACTION_MOVE_UP;
        }


        case STATE_MOVING_DOWN: {
            // må ta hensyn til at at_floor() = -1 ved 
            if(current_floor <= MIN_FLOOR) {
                *p_elevator_state = STATE_IDLE;
                return ACTION_STOP_MOVEMENT;
            }

            // We start the loop at last_floor because we only wish to check for floors we are moving towards.
            for(int floor = last_floor; floor > MIN_FLOOR; floor--) {
                if (current_floor == ORDERS_CAB[floor] && check_order_match(current_floor, last_dir)) {
                    // Here we have found a valid floor to stop at!

                    update_target_floor(&current_order, current_floor);
                    clear_cab_orders(current_floor);

                    *p_elevator_state = STATE_IDLE;

                    return ACTION_STOP_MOVEMENT;
                } 
            }

            return ACTION_MOVE_DOWN;
        }
        
    } // EOF switch

    return ACTION_DO_NOTHING;
}

int determine_direction(elevator_state_t* p_elevator_state, Order* p_current_order, int current_floor) {
    // This needs to be a two part operation.
    // Before moving for the second time (after going to the floor we clicked the floor button at),
    // we need to update p_current_order->target floor with one of the cab orders,
    // using update_target_floor()
    
    if(current_floor < MIN_FLOOR || current_floor > HARDWARE_NUMBER_OF_FLOORS) {
        return -1;
    }
    
    if(current_floor < p_current_order->target_floor) {
        *p_elevator_state= STATE_MOVING_UP;
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

int emergency_action(time_t* p_door_timer, int* p_door_open){
    if(hardware_read_stop_signal()){
        erase_queue(QUEUE);
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        start_timer(p_door_timer);
        if (at_floor() != -1){
            *p_door_open = DOOR_OPEN;
            hardware_command_door_open(DOOR_OPEN);
        }
        return ACTION_EMERGENCY;
    }
    else{
        if(at_floor() == -1){
            return ACTION_DO_NOTHING;
        }
        else if(check_timer(p_door_timer)){
            return ACTION_CHECK_OBSTRUCTION;
        }
        return ACTION_EMERGENCY;
    }
}

int obstruction_check(time_t* p_door_timer, int* p_door_open){
    if(hardware_read_obstruction_signal()){
        return ACTION_CHECK_OBSTRUCTION;
    }
    if(check_timer(p_door_timer) && *p_door_open == DOOR_OPEN){
        return ACTION_CLOSE_DOOR;
    }
    return ACTION_CHECK_OBSTRUCTION;
}

      // In STATE_IDLE:
      // If the QUEUE is not empty, we will transition to IDLE with one of four cases: 
            // Case 1: Stop button was pressed
                // Because the QUEUE is emptied when the stop button is pressed, this
                // case will be caught by the first check

            // Case 2: We arrive at the given target_floor for the current order

            // Case 3: We stop to pick up an order thats been set in the same direction on the way to
            //         target_floor: We have transitioned into IDLE, but we are not at the target floor!


            // Case 2 and 3 will execute the same exact sequence, we can therefore add them
            // to the same check. 
            // check_order_match() will return a truth value if the QUEUE contains an order whos target_floor is equal
            // to the current floor AND is an order in the same direction as last_dir
            // if(current_order.target_floor == current_floor || check_order_match(p_queue, current_floor, last_dir)) {
            //     *p_elevator_state = STATE_HANDLE_ORDER;
            //     return STOP_MOVEMENT;
            // }



        // case STATE_HANDLE_ORDER: {
        //     if(!(*p_door_open)){
        //         hardware_command_door_open(DOOR_OPEN);
        //         *p_door_open = DOOR_OPEN;
        //     }
        //     // We firstly need to check for the obstruction signal
        //     if(hardware_read_obstruction_signal() && *p_door_open == DOOR_OPEN) {
        //         return START_DOOR_TIMER;
        //     }
  
        //     // If enough time has passed, close the doors and start moving
        //     if(check_timer(p_door_timer) && !hardware_read_obstruction_signal()){ 
        //         // Then determine which direction we should move in
        //         return determine_direction(p_elevator_state, &current_order, current_floor);
 
        //     }
        //     else {
        //         // Not enough time has passed => Doors remain open and we do nothing
        //         *p_elevator_state = STATE_HANDLE_ORDER;
        //         return DO_NOTHING;
        //     }
        //     break;
        // }
