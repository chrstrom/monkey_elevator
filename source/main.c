/**
 * @file
 * @brief main linker point of elevator program
 */
#include "timer.h"
#include "queue.h"
#include "elevator_fsm.h"


int elevator_init() {

    // Turn off all button lights, and set the floor indicator
    for(int floor_up = 0; floor_up < MAX_FLOOR; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, LIGHT_OFF);
    }

    for(int floor_down = 0; floor_down < MAX_FLOOR; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, LIGHT_OFF);
    }
    hardware_command_stop_light(LIGHT_OFF);
    hardware_command_floor_indicator_on(at_floor());


    // Close the door and, if not in a floor already, move to a floor
    hardware_command_door_open(DOOR_CLOSED);
    while(at_floor() == -1) {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
   
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

    int order_up[MAX_FLOOR + 1] = {0, 0, 0, 0};
    int order_down[MAX_FLOOR + 1] = {0, 0, 0, 0};

    int door_open = DOOR_CLOSED;
    int next_action  = STOP_MOVEMENT;
    int last_floor = at_floor(); //should reach a valid floor during elevator_init
    HardwareMovement last_dir = HARDWARE_MOVEMENT_STOP;
    elevator_state_t elevator_state = STATE_IDLE;

    time_t stop_button_timer = time(NULL);
    time_t        door_timer = time(NULL);
    start_timer(&stop_button_timer);
    start_timer(&door_timer);

    // ELEVATOR PROGRAM LOOP
    while(1){

        // The check for the stop button is done independently of the FSM, as it is the most important safety feature to get right
        // Although if it can seamlessly be incoorporated in the FSM, that is to be preferred.
        if(hardware_read_stop_signal()) {
            hardware_command_stop_light(LIGHT_ON);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
            // If the elevator is at a floor, open the doors
            if(at_floor() != -1) {
                hardware_command_door_open(DOOR_OPEN);
                door_open = DOOR_OPEN;
            }
            //delete the queue and the orders
            erase_queue(queue);
            //erase_order(order_up);
            //erase_order(order_down);
            start_timer(&stop_button_timer);
            continue;
        }
        else if(check_timer(&stop_button_timer) == 0) {
            continue;
           // If the stop button has been released, but less than NORMAL_WAIT_TIME has passed
           // According to specifications, we shouldnt take in orders here
        }
       
 
        poll_floor_buttons(order_up, order_down);
        set_floor_button_lights(order_up, order_down);

        next_action = update_state(&elevator_state, &door_timer, queue, last_dir, last_floor, &door_open);

        switch(next_action) {
            case DO_NOTHING:
                break;

            // case CHECK_OBSTRUCTION:
            //     break;

            case START_DOOR_TIMER:
                start_timer(&door_timer);
                break;

            case OPEN_DOOR:
                hardware_command_door_open(DOOR_OPEN);
                door_open = DOOR_OPEN;
                break;

            case CLOSE_DOOR:
                hardware_command_door_open(DOOR_CLOSED);
                door_open = DOOR_CLOSED;
                break;

            case MOVE_UP:
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
                break;
            
            case MOVE_DOWN:
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                break;

            case STOP_MOVEMENT:
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                break;
            
            default:
                fprintf(stderr, "Default case reached in switch in main. This should not happen\n");
        }

        
    }
}
