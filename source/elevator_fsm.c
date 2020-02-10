#include "elevator_fsm.h"

int update_state(elevator_state_t* p_current_state, time_t* p_ref_time, Order* p_current_order) {
    int next_action; //ikke lenger nødvendig??
    int current_floor = at_floor(); 
    switch(*(p_current_state)) {
        case STATE_IDLE: {
            if(p_current_order = NULL){
                return;
            }
            //int time_diff = check_timer(p_ref_time); //Men må da vite om hva som var forrige timer!!
            //Burde ikke være et problem, siden vi kun skal endre den verdien inne i state_moving_up/down
            //Pekeren burde altså ha rett verdi
            if(check_timer(p_ref_time)){
                hardware_command_door_open(DOOR_CLOSED);
                //skal her lukke dør
            }
            else{
                //enda ikke gått x antall sekunder
                hardware_command_door_open(DOOR_OPEN);
            }

            // Transition into IDLE after every completed order
            // Transition out immediatly if queue is not empty
            // Remain in idle if queue is empty

            //må sjekke om rekkefølgen i order. Her skal vi da ta for oss plutselig endring
            //slik at man sikrer at rekkefølgen håndteres av programmet

            break;
        }
        /*
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
                next_action = START_DOOR_TIMER;
            }
            else {
                elevator_state = STATE_ERROR;
            }

            break;
        }
        */
        case STATE_MOVING_UP: {
            for(int fl = MIN_FLOOR; fl <= MAX_FLOOR; fl++){
                if (current_floor == p_current_order->floor_to[fl]){
                    p_current_state = STATE_IDLE;
                    start_timer(p_ref_time);
                    hardware_command_floor_indicator_on(fl);
                    
                    break;
                    //må her legge til en mulighet/endring for å slette dette
                    //elementet i køen
                }
            }
            break;  
        }
        case STATE_MOVING_DOWN: {
            for(int fl = MIN_FLOOR; fl <= MAX_FLOOR; fl++){
                if (current_floor == p_current_order->floor_to[fl]){ //må også sjekke at det er i riktig retning
                    p_current_state = STATE_IDLE;
                    start_timer(p_ref_time);
                    //må her legge til en mulighet/endring for å slette dette
                    //elementet i køen
                }
                break;
            }
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

    return next_action;