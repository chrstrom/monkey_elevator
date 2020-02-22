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

    // //hardcoding the shit to test some functions!
    // QUEUE[0].target_floor = 1; QUEUE[0].order_type = HARDWARE_ORDER_INSIDE;
    // QUEUE[1].target_floor = 1; QUEUE[1].order_type = HARDWARE_ORDER_UP;
    // QUEUE[2].target_floor = 0; QUEUE[2].order_type = HARDWARE_ORDER_UP;
    // QUEUE[3].target_floor = 3; QUEUE[3].order_type = HARDWARE_ORDER_DOWN;
    // QUEUE[4].target_floor = 2; QUEUE[4].order_type = HARDWARE_ORDER_DOWN;
    // QUEUE[5].target_floor = 2; QUEUE[5].order_type = HARDWARE_ORDER_INSIDE;
    // QUEUE[6].target_floor = 3; QUEUE[6].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[7].target_floor = 1; QUEUE[7].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[8].target_floor = 1; QUEUE[8].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[9].target_floor = 1; QUEUE[9].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[10].target_floor = 1; QUEUE[10].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[11].target_floor = 1; QUEUE[11].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[12].target_floor = 1; QUEUE[12].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[13].target_floor = 1; QUEUE[13].order_type = HARDWARE_ORDER_INSIDE;
    // // QUEUE[14].target_floor = 1; QUEUE[14].order_type = HARDWARE_ORDER_INSIDE;

    // while(1){
    //     sort_queue(HARDWARE_MOVEMENT_UP);
    //     sort_queue(HARDWARE_MOVEMENT_STOP);
    //     sort_queue(HARDWARE_MOVEMENT_DOWN);
    // }






    // ELEVATOR PROGRAM LOOP
    while (1){
        // Temporary
        elevator_data.last_floor = (get_current_floor() == BETWEEN_FLOORS ? elevator_data.last_floor : get_current_floor());

        // Set floor light
        set_floor_indicator_light(get_current_floor());

        //update the state of the buttons. I.e. poll all the buttons and add to queue
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

