/**
 * @file
 * @brief main linker point of elevator program
 */

#include "timer.h"
#include "queue.h"
#include "elevator_fsm.h"


// Antar:

    // At cab-knappene kun kan trykkes inn med "Folk" i heisen



int elevator_init() {
    hardware_command_door_open(0);
    
    if(!at_floor()) {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    
    return 0;
}


int main(){

    // ELEVATOR INITIAL SETUP
    Order queue[QUEUE_SIZE];

    int order_up[MAX_FLOOR + 1];
    int order_down[MAX_FLOOR + 1];

    int current_floor = -1; //invalid floor to set the elevator's intitial floor-value
    int door_open = -1;
    int next_action  = -1;

    time_t stop_timer = time(NULL);
    time_t door_timer = time(NULL);

    elevator_state_t elevator_state = STATE_IDLE;
    
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    error = elevator_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize software\n");
        exit(1);
    }


    // ELEVATOR PROGRAM LOOP
    // If we made new commands for set/get, we can set/get multiple connected things at once
    // ex: elevator_open_doors() can both open/close the door, and also set the lamp
    while(1){


        // Can be part of update_state and next action!!!
        if(hardware_read_stop_signal()) {
            elevator_state = STATE_IDLE;
            erase_queue();
            if(at_floor() != -1){
                hardware_command_door_open(1);
            }
            start_timer(stop_timer);

        }

        if(hardware_read_obstruction_signal() && door_open) {
            // Restart the door timer when obstruction is activated
            start_timer(door_timer);
        }
    
        poll_floor_buttons();
    
        set_floor_button_lights(order_up, order_down);

        // update next_action in each case
        // After finishing one order, set the elevator back to idle.
        // !! Do we use next_action to split output from transition in the FSM, or do we 
        // transition and execute output in FSM?

        Order current_order = queue[0];
        next_action = update_state(elevator_state);

        switch(next_action) {
            case START_DOOR_TIMER:
                start_timer(door_timer);
                
        }

    }
}
