#include "elevator_io.h"
#include "globals.h"
#include "queue.h"

int at_floor() {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
       if(hardware_read_floor_sensor(floor)) {
           return floor;
       }
    }
    return -1;
}

void set_floor_indicator_light(int last_floor) {
    if(last_floor != -1) {
        hardware_command_floor_indicator_on(last_floor);
    }
}


// Floor buttons
void poll_floor_buttons(elevator_data_t* p_data) {
  
    for(int floor_up = MIN_FLOOR ; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        if(p_data->ORDERS_UP[floor_up] == 0 && hardware_read_order(floor_up, HARDWARE_ORDER_UP) == 1){
            push_back_queue(floor_up, HARDWARE_ORDER_UP);
            p_data->ORDERS_UP[floor_up] = 1;
        }
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        if(p_data->ORDERS_DOWN[floor_down] == 0 && hardware_read_order(floor_down, HARDWARE_ORDER_DOWN) == 1){
            push_back_queue(floor_down, HARDWARE_ORDER_DOWN);
            p_data->ORDERS_DOWN[floor_down] = 1;
        }
    }
    // After getting all button events, add orders to queue and set the button lights
    set_floor_button_lights(p_data);
}

void set_floor_button_lights(elevator_data_t* p_data) {
    // The last floor does not have an up-button: Start at 0.
    for(int floor_up = MIN_FLOOR; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, p_data->ORDERS_UP[floor_up]);
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, p_data->ORDERS_DOWN[floor_down]);
    }
}


// Cab buttons
void poll_cab_buttons(elevator_data_t* p_data) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(p_data->ORDERS_CAB[floor] == 0 && hardware_read_order(floor, HARDWARE_ORDER_INSIDE)) {
            push_back_queue(floor, HARDWARE_ORDER_INSIDE);
            p_data->ORDERS_CAB[floor] = 1;
        }
    }

    set_cab_button_lights(p_data);
}

void set_cab_button_lights(elevator_data_t* p_data) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, p_data->ORDERS_CAB[floor]);
    }
}

