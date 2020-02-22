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
    
    error = init_elevator();
    if(error != 0){
        fprintf(stderr, "Unable to initialize software\n");
        exit(1);
    }

    elevator_data_t elevator_data = {.door_open = DOOR_CLOSE, .last_floor = get_current_floor(), .last_dir = HARDWARE_MOVEMENT_STOP, .state = STATE_IDLE, .next_action = ACTION_STOP_MOVEMENT};

    // ELEVATOR PROGRAM LOOP
    while (1){
        // Temporary
        elevator_data.last_floor = (get_current_floor() == BETWEEN_FLOORS ? elevator_data.last_floor : get_current_floor());

        // Set floor light
        set_floor_indicator_light(get_current_floor());

        // Handle button press events
        update_button_state(&elevator_data);

        // Determine next action
        elevator_data.next_action = elevator_update_state(&elevator_data);

        //Execute next action
        //possibly it's own function
        switch (elevator_data.next_action){
        case ACTION_DO_NOTHING:
            break;

        case ACTION_START_DOOR_TIMER:
            start_timer();
            break;

        case ACTION_OPEN_DOOR:
            hardware_command_door_open(DOOR_OPEN);
            elevator_data.door_open = DOOR_OPEN;
            break;

        case ACTION_CLOSE_DOOR:
            update_queue();
            hardware_command_door_open(DOOR_CLOSE);
            elevator_data.door_open = DOOR_CLOSE;
            break;

        case ACTION_MOVE_UP:
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            elevator_data.last_dir = HARDWARE_MOVEMENT_UP;
            elevator_data.state = STATE_MOVING_UP;
            break;

        case ACTION_MOVE_DOWN:
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            elevator_data.last_dir = HARDWARE_MOVEMENT_DOWN;
            elevator_data.state = STATE_MOVING_DOWN;
            break;

        case ACTION_STOP_MOVEMENT:
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            elevator_data.last_dir = HARDWARE_MOVEMENT_STOP;
            elevator_data.state = STATE_IDLE;
            break;

        case ACTION_EMERGENCY:
            emergency_action(&elevator_data);
            break;

        default:
            fprintf(stderr, "Default case reached in switch in main. This should not happen\n");
            break;
        }
    }
}

