/**
 * @file
 * @brief main linker point of elevator program
 */
#include "timer.h"
#include "queue.h"
#include "elevator_fsm.h"


int elevator_init() {

    // Turn off all button lights
    for(int floor_up = 0; floor_up < MAX_FLOOR; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, LIGHT_OFF);
    }

    for(int floor_down = 0; floor_down < MAX_FLOOR; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, LIGHT_OFF);
    }

    // Clear all order arrays just in case;
    for(int floor = 0; floor < MAX_FLOOR + 1; floor++) {
        ORDERS_UP[floor] = 0;
        ORDERS_DOWN[floor] = 0;
        ORDERS_CAB[floor] = 0;
    }
    hardware_command_stop_light(LIGHT_OFF);
   


    // Close the door and, if not in a floor already, move to a floor
    // Set the floor-indicator

    //Kan man anta at obstruksjonsbryteren aldri skal være høy i inittialiseringsfasen?
    hardware_command_door_open(DOOR_CLOSED);
    while(at_floor() == -1) {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_floor_indicator_on(at_floor());
   
    return 0;
}


int main(){

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

    // ELEVATOR INITIAL SETUP

    Order queue[QUEUE_SIZE];

    int door_open = DOOR_CLOSED;
    int next_action  = CMD_STOP_MOVEMENT;
    int last_floor = at_floor(); //should reach a valid floor during elevator_init
    HardwareMovement last_dir = HARDWARE_MOVEMENT_STOP;
    elevator_state_t elevator_state = STATE_IDLE;

    time_t stop_button_timer = time(NULL);
    time_t door_timer = time(NULL);
    start_timer(&stop_button_timer);
    start_timer(&door_timer);

    // ELEVATOR PROGRAM LOOP
    while(1){
        
        // First check if the stop-button is pressed! This must be done before the fsm, as this is the most
        //important safety-check
        if(hardware_read_stop_signal()){
            hardware_command_stop_light(LIGHT_ON);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            next_action = emergency_action(queue, &door_timer, &door_open);
        }
        
        else{
            poll_floor_buttons();
            add_order_to_queue(queue);
            set_floor_button_lights();
            if(next_action != CMD_EMERGENCY && next_action != CMD_OBSTRUCTION){
                next_action = update_state(&elevator_state, &door_timer, queue, last_dir, last_floor, &door_open);
            }
        }

        switch(next_action) {
            case CMD_DO_NOTHING:
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                break;

            case CMD_EMERGENCY:
                next_action = emergency_action(queue, &door_timer, &door_open);
                break;
            
            case CMD_CHECK_OBSTRUCTION:
                next_action = obstruction_check(&door_timer, &door_open);
                break;

            /*
            case CMD_START_DOOR_TIMER:
                start_timer(&door_timer);
                break;
            */

            case CMD_OPEN_DOOR:
                hardware_command_door_open(DOOR_OPEN);
                start_timer(door_timer);
                door_open = DOOR_OPEN;
                next_action = CMD_CHECK_OBSTRUCTION;
                break;

            case CMD_CLOSE_DOOR:
                // We assume that the check for obstruction is done preemptivaly                
                // Here we go out of the obstruction-state with CMD_DO_NOTHING, such that
                // the elevator can calculate the next action/state
                hardware_command_door_open(DOOR_CLOSED);
                door_open = DOOR_CLOSED;
                next_action = CMD_DO_NOTHING;
                break;

            case CMD_MOVE_UP:
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
                break;
            
            case CMD_MOVE_DOWN:
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                break;

            case CMD_STOP_MOVEMENT:
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                break;
            
            default:
                fprintf(stderr, "Default case reached in switch in main. This should not happen\n");
                break;
        }   
    }
}
