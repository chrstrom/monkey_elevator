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

// By passing p_order up/down, we make sure to not add a new order for floors that already
// have orders to them. Note that these arrays are only updated by external buttons.
// The internal cab-buttons will have no impact on this.
void floor_button_event_handler(elevator_data_t* data) {
  
    for(int floor_up = MIN_FLOOR ; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        if(data->ORDERS_UP[floor_up] == 0 && hardware_read_order(floor_up, HARDWARE_ORDER_UP) == 1){
            add_order_to_queue(floor_up, HARDWARE_ORDER_UP);
            data->ORDERS_UP[floor_up] = 1;
        }
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        if(data->ORDERS_DOWN[floor_down] == 0 && hardware_read_order(floor_down, HARDWARE_ORDER_DOWN) == 1){
            add_order_to_queue(floor_down, HARDWARE_ORDER_DOWN);
            data->ORDERS_DOWN[floor_down] = 1;
        }
    }
    // After getting all button events, add orders to queue and set the button lights
    set_floor_button_lights(data);
}


//May want to change the in-argument to QUEUE
void cab_button_event_handler(elevator_data_t* data) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(data->ORDERS_CAB[floor] == 0 && hardware_read_order(floor, HARDWARE_ORDER_INSIDE)) {
            add_order_to_queue(floor, HARDWARE_ORDER_INSIDE);
            data->ORDERS_CAB[floor] = 1;
        }
    }

    set_cab_button_lights(data);
}

void set_floor_indicator_light(int last_floor) {
    if(last_floor != -1) {
        hardware_command_floor_indicator_on(last_floor);
    }
}

void set_floor_button_lights(elevator_data_t* data) {
    // The last floor does not have an up-button: Start at 0.
    for(int floor_up = MIN_FLOOR; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, data->ORDERS_UP[floor_up]);
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, data->ORDERS_DOWN[floor_down]);
    }
}

void set_cab_button_lights(elevator_data_t* data) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, data->ORDERS_CAB[floor]);
    }
}

