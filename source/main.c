/**
 * @file
 * @brief main linker point of elevator program
 */
#include <stdio.h>
#include <stdlib.h>

#include "driver/hardware.h"
#include "globals.h"
#include "elevator_fsm.h"
#include "queue.h"
#include "elevator_io.h"
#include "timer.h"


int main(){
    // ELEVATOR INITIAL SETUP
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

    elevator_data_t elevator_data = {.door_open = DOOR_CLOSE,
                                     .last_floor = get_current_floor(),
                                     .last_dir = HARDWARE_MOVEMENT_STOP,
                                     .state = STATE_IDLE,
                                     .next_action = ACTION_STOP_MOVEMENT};

    while (1){
        elevator_data.last_floor = update_valid_floor(elevator_data.last_floor);
        set_floor_indicator_light(get_current_floor());

        update_button_state(&elevator_data);

        elevator_data.next_action = elevator_update_state(&elevator_data);
        elevator_execute_next_action(&elevator_data);
    }
}