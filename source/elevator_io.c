#include "elevator_io.h"
#include "globals.h"
#include "queue.h"

//Kanskje ønskelig å bruke disse funksjonene, slik at vi ikke trenger å kjenne til hardware i andre moduler!
int check_stop_button(){
    return hardware_read_stop_signal();
}


int check_obstruction(){
    return hardware_read_obstruction_signal();
}


void drive_elevator(HardwareMovement dir){
    hardware_command_movement(dir);
}


void set_cab_button_lights(int* p_orders_cab) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, p_orders_cab[floor]);
    }
}


void set_floor_indicator_light(int floor) {
    if(floor != BETWEEN_FLOORS) {
        hardware_command_floor_indicator_on(floor);
    }
}


int get_current_floor() {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
       if(hardware_read_floor_sensor(floor)) {
           return floor;
       }
    }
    return BETWEEN_FLOORS;
}


void update_cab_buttons(int* p_orders_cab) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(p_orders_cab[floor] == 0 && hardware_read_order(floor, HARDWARE_ORDER_INSIDE)) {
            push_back_queue(floor, HARDWARE_ORDER_INSIDE);
            p_orders_cab[floor] = 1;
        }
    }

    set_cab_button_lights(p_orders_cab);
}


void set_floor_button_lights(int* p_orders_up, int* p_orders_down) {
    // The last floor does not have an up-button: Start at 0.
    for(int floor_up = MIN_FLOOR; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, p_orders_up[floor_up]);
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, p_orders_down[floor_down]);
    }
}



void update_floor_buttons(int* p_orders_up, int* p_orders_down) {
  
    for(int floor_up = MIN_FLOOR; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        if(p_orders_up[floor_up] == 0 && hardware_read_order(floor_up, HARDWARE_ORDER_UP) == 1){
            push_back_queue(floor_up, HARDWARE_ORDER_UP);
            p_orders_up[floor_up] = 1;
        }
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        if(p_orders_down[floor_down] == 0 && hardware_read_order(floor_down, HARDWARE_ORDER_DOWN) == 1){
            push_back_queue(floor_down, HARDWARE_ORDER_DOWN);
            p_orders_down[floor_down] = 1;
        }
    }
    // After getting all button events, add orders to queue and set the button lights
    set_floor_button_lights(p_orders_up, p_orders_down);
}


int init_elevator() {
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
    while(get_current_floor() == BETWEEN_FLOORS) {}
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);

    hardware_command_floor_indicator_on(get_current_floor());

    init_queue();
    return 0;
}