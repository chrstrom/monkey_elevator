#include "elevator_fsm.h"

int update_state(elevator_state_t* p_elevator_state, time_t* door_timer, Order* p_queue, HardwareMovement last_dir, int last_floor) {

    int current_floor = at_floor(); 
    Order current_order = p_queue[0];
    int last_dir;
    switch(*p_elevator_state) {
        case STATE_IDLE: {
          
            if(queue_is_empty(p_queue)) {
                return DO_NOTHING;
            }
        
            // If the queue is not empty, we will transition to IDLE with one of four cases:
            // Case 1: Stop button was pressed
                // Because the queue is emptied when the stop button is pressed, this
                // case will be caught by the first check

            // Case 2: We arrive at the given floor_at for the current order

            // Case 3: We stop to pick up an order thats been set in the same direction on the way to
            //         floor_at: We have transitioned into IDLE, but we are not at the target floor!

            // Case 4: obstruction? idk yet really

            // Case 2 and 3 will execute the same exact sequence, we can therefore add them
            // to the same check. 
            // check_order_match() will return a truth value if the queue contains an order whos floor_at is equal
            // to the current floor AND is an order in the same direction as last_dir
            if(current_order.floor_at == current_floor || check_order_match(p_queue, current_floor, last_dir)) {
                p_elevator_state = STATE_PREP_MOVE;
                return OPEN_DOOR;
            } 

            // Because of prep_move we dont need to mess with timers here

            return DO_NOTHING;  // We shouldnt get to this point
        }

        case STATE_PREP_MOVE: {
            // If enough time has passed, close the doors and start moving
            if(check_timer(door_timer)){ 
                // Then determine which direction we should move in
                if(current_floor < current_order.floor_at) {
                    p_elevator_state= STATE_MOVING_UP;
                    return CLOSE_DOOR;
                }
                else if(current_floor > current_order.floor_at) {
                    p_elevator_state = STATE_MOVING_DOWN;
                    return CLOSE_DOOR;
                }
                else if(current_floor == current_order.floor_at){
                    // This is incomplete. We need to update floor_at for the current order
                    // so to not be stuck in this. If we do not update floor_at, the check above
                    // will always return true.

                    update_queue_floor_at(&current_order, current_floor);
                    p_elevator_state = STATE_PREP_MOVE;
                    return START_DOOR_TIMER;
                }
            }
            else {
                // Not enough time has passed => Doors remain open and we do nothing
                p_elevator_state = STATE_PREP_MOVE;
                return DO_NOTHING;
            }
            break;
        }


        case STATE_MOVING_UP: {
            // We start the loop at last_floor because we only wish to check for floors we are moving towards.
            for(int floor = last_floor; floor <= MAX_FLOOR; floor++){
                if (current_floor == current_order.floor_to[floor] && check_order_match(p_queue, current_floor, last_dir)){
                    // Here we have found a valid floor to stop at!
                    p_elevator_state = STATE_IDLE;
            
                    // Remember to:
                    // 1: update any Order's floor_at
                    // 2: update any Order->floor_to value that corresponds to the current floor. 
                    // "update" as in "we handled this"
                    return START_DOOR_TIMER;
                }
            }
            return MOVE_UP;
        }


        case STATE_MOVING_DOWN: {
            // We start the loop at last_floor because we only wish to check for floors we are moving towards.
            for(int floor = last_floor; floor > MIN_FLOOR; floor--) {
                if (current_floor == current_order.floor_to[floor] && check_order_match(p_queue, current_floor, last_dir)) {
                    // Here we have found a valid floor to stop at!
                    p_elevator_state = STATE_IDLE;

                    // Remember to:
                    // 1: update any Order's floor_at
                    // 2: update any Order->floor_to value that corresponds to the current floor. 
                    // "update" as in "we handled this"
                    return START_DOOR_TIMER;
                } 
            }

            return MOVE_DOWN;
        }
        
    } // EOF switch

    return DO_NOTHING;
}
