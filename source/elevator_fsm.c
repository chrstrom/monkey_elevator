#include "elevator_fsm.h"

int update_state(elevator_state_t* p_elevator_state, time_t* p_ref_time, Order* p_queue, HardwareMovement last_dir, int last_floor) {

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
            // Case 4:

            // Case 2 and 3 will execute the same exact sequence, we can therefor add them
            // to the same check. 
            // check_order_match() will return a truth value if the queue contains an order whos floor_at is equal
            // to the current floor AND is an order in the same direction as last_dir
            // Case 2 & 3
            if(current_order.floor_at == current_floor || check_order_match(p_queue, current_floor, last_dir)) {
                p_elevator_state = STATE_PREP_MOVE;
                return OPEN_DOOR;
            } 

            // Because of prep_move we dont need to mess with timers here
            //int time_diff = check_timer(p_ref_time); //Men må da vite om hva som var forrige timer!!
            //Burde ikke være et problem, siden vi kun skal endre den verdien inne i state_moving_up/down
            //Pekeren burde altså ha rett verdi
            // if(check_timer(p_ref_time)){
            //     next_action = CLOSE_DOOR;
            //     //skal her lukke dør
            // }
            // else{
            //     //enda ikke gått x antall sekunder
            //     next_action = OPEN_DOOR;
            // }
 
            // Transition into IDLE after every completed order
            // Transition out immediatly if queue is not empty
            // Remain in idle if queue is empty

            //må sjekke om rekkefølgen i order. Her skal vi da ta for oss plutselig endring
            //slik at man sikrer at rekkefølgen håndteres av programmet

            break;
        }

        // Need to add a way to check for last floor that was served
        case STATE_MOVING_UP: {
            for(int floor = last_floor; floor <= MAX_FLOOR; floor++){
                if (current_floor == current_order.floor_to[floor] && check_order_match(p_queue, current_floor, last_dir)){

                    p_elevator_state = STATE_IDLE;
                    return START_DOOR_TIMER;
            
                    //må her legge til en mulighet/endring for å slette dette
                    //elementet i køen
                }
            }
            return MOVE_UP;
        }


        case STATE_MOVING_DOWN: {
            for(int floor = last_floor; floor > MIN_FLOOR; floor--) {
                if (current_floor == current_order.floor_to[floor] && check_order_match(p_queue, current_floor, last_dir)) {

                    p_elevator_state = STATE_IDLE;
                    return START_DOOR_TIMER;
                    //må her legge til en mulighet/endring for å slette dette
                    //elementet i køen
                }
                
            }

            return MOVE_DOWN;
        }


   
        case STATE_PREP_MOVE: {
            // If enough time has passed, close the doors and start moving
            if(check_timer(p_ref_time)){ 
                if(current_floor < current_order.floor_at) {
                    p_elevator_state= STATE_MOVING_UP;
                    return CLOSE_DOOR;
                }
                else if(current_floor > current_order.floor_at) {
                    p_elevator_state = STATE_MOVING_DOWN;
                    return CLOSE_DOOR;
                }
                else if(current_floor == current_order.floor_at){
                    // We need to update floor_at for the current order
                    // with one of the cab buttons pressed
                    update_floor_at();
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
        


        /*
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
        */
        
    }

    return DO_NOTHING;
}
