/**
 * @file
 * @brief main linker point of elevator program
 */

#include "utility.h"
#include "queue.h"
#include <stdint.h>



typedef enum{
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_PREP_MOVE,
    STATE_SERVE_ORDER,
    STATE_ERROR
} elevator_state_t;


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

    int current_floor = -1; //invalid floor to set the elevator's intitial floor-value
    int door_open = -1;

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
        if(hardware_read_stop_signal()) {
            elevator_state = STATE_IDLE;
            erase_queue();
            if(at_floor()){
                hardware_command_door_open(1);
            }
            start_timer(stop_timer);

        }

        if(hardware_read_obstruction_signal() && door_open) {
            // Restart the door timer when obstruction is activated
            start_timer(door_timer);
        }
    
        poll_floor_buttons();
    
        update_floor_lights(current_floor);

        // update next_action in each case
        // After finishing one order, set the elevator back to idle.
        Order current_order = queue[0];
        switch(elevator_state) {
            case STATE_IDLE: {
                
                // Transition into IDLE after every completed order
                // Transition out immediatly if queue is not empty
                // Remain in idle if queue is empty
                break;
            }
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
                    start_timer(door_timer);
                }
                else {
                    elevator_state = STATE_ERROR;
                }

                break;
            }
            case STATE_MOVING_UP: {
                if(current_floor == current_order.floor_at) {
                    elevator_state = STATE_SERVE_ORDER;
                    start_timer(door_timer);
                }
                break;  
            }
            case STATE_MOVING_DOWN: {
                if(current_floor == current_order.floor_at) {
                    elevator_state = STATE_SERVE_ORDER;
                    start_timer(door_timer);
                }
                break;  
            }
        
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
                }
  
                break;
            }
         
        }
    }
}
