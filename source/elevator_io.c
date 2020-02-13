#include "elevator_io.h"

int at_floor() {
    for(int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
       if(hardware_read_floor_sensor(floor)) {
           return floor;
       }
    }

    return -1;
}

// By passing p_order up/down, we make sure to not add a new order for floors that already
// have orders to them. Note that these arrays are only updated by external buttons.
// The internal cab-buttons will have no impact on this.
void poll_floor_buttons() {
  
    for(int floor_up = 0; floor_up < MAX_FLOOR; floor_up++) {
        if(ORDERS_UP[floor_up] == 0 && hardware_read_order(floor_up, HARDWARE_ORDER_UP) == 1){
            ORDERS_UP[floor_up] = 1;
        }
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = 1; floor_down <= MAX_FLOOR; floor_down++) {
        if(ORDERS_DOWN[floor_down] == 0 && hardware_read_order(floor_down, HARDWARE_ORDER_DOWN) == 1){
            ORDERS_DOWN[floor_down] = 1;
        }
    }

    // if(order_floor != 0){ 
    //     Order order = {.target_floor = order_floor};
    //     queue_push_back(queue, order);
    // }

}

void set_floor_button_lights() {
    // The last floor does not have an up-button: Start at 0.
    for(int floor_up = 0; floor_up < MAX_FLOOR; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, ORDERS_UP[floor_up]);
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = 1; floor_down <= MAX_FLOOR; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, ORDERS_UP[floor_down]);
    }
}

void set_cab_button_lights() {
    for(int floor = 0; floor < SIZEOF_ARR(ORDERS_CAB); floor++) {
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, ORDERS_CAB[floor]);
    }
}



//May want to change the in-argument to queue
void update_cab_buttons(Order* p_queue) {
    for(int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
        int order = hardware_read_order(floor, HARDWARE_ORDER_INSIDE);
        ORDERS_CAB[floor] = order;
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, order);
    }
}


