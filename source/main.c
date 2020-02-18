/**
 * @file
 * @brief main linker point of elevator program
 */

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "driver/hardware.h"
#include "elevator_fsm.h"
#include "queue.h"
#include "elevator_io.h"
#include "timer.h"

int elevator_init() {

    //Turn off all button lights and clear all order light arrays (just in case)
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        hardware_command_order_light(floor, HARDWARE_ORDER_UP,     LIGHT_OFF);
        hardware_command_order_light(floor, HARDWARE_ORDER_DOWN,   LIGHT_OFF);
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, LIGHT_OFF);
    }

    //we assume the obstruction will never be active during setup!
    hardware_command_stop_light(LIGHT_OFF);
    hardware_command_door_open(DOOR_CLOSE); 

    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    while(at_floor() == -1) {}
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);

    hardware_command_floor_indicator_on(at_floor());
   
    init_queue();
    return 0;
}

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
    elevator_data_t elevator_data = {.door_open = DOOR_CLOSE, .last_floor = at_floor(), .last_dir = HARDWARE_MOVEMENT_STOP, .state = STATE_IDLE, .next_action = ACTION_STOP_MOVEMENT, .check_time = NOT_CHECK_DOOR_TIME};
    time_t timer = time(NULL);

    // ELEVATOR PROGRAM LOOP
    while(1){
        elevator_data.last_floor = (at_floor() == -1 ? elevator_data.last_floor : at_floor());

        // Set floor light   
        set_floor_indicator_light(at_floor());

        // Handle button press events
        poll_floor_buttons(&elevator_data);
        poll_cab_buttons(&elevator_data);

        // Determine next action
        elevator_data.next_action = update_state(&elevator_data, &timer);

        //Execute next action
        //possibly it's own function
        switch(elevator_data.next_action) {
            case ACTION_DO_NOTHING:
                break;

            case ACTION_START_DOOR_TIMER:
                start_timer(&timer);
                elevator_data.check_time = CHECK_DOOR_TIME;
                break;

            case ACTION_OPEN_DOOR:
                hardware_command_door_open(DOOR_OPEN);
                elevator_data.door_open = DOOR_OPEN;
                break;

            case ACTION_CLOSE_DOOR:
                update_queue();
                hardware_command_door_open(DOOR_CLOSE);
                elevator_data.door_open = DOOR_CLOSE;
                elevator_data.check_time = NOT_CHECK_DOOR_TIME;
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
                emergency_action(&elevator_data, &timer);
                elevator_data.check_time = CHECK_DOOR_TIME;
                break;

            default:
                fprintf(stderr, "Default case reached in switch in main. This should not happen\n");
                break;
        }   
    }
}
