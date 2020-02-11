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
void poll_floor_buttons(int* p_order_up, int* p_order_down) {
  
    for(int floor_up = 0; floor_up < MAX_FLOOR; floor_up++) {
        if(!p_order_up[floor_up] && hardware_read_order(floor_up, HARDWARE_ORDER_UP)){
            p_order_up[floor_up] = 1;
        }
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = 1; floor_down <= MAX_FLOOR; floor_down++) {
        if(!p_order_down[floor_down] && hardware_read_order(floor_down, HARDWARE_ORDER_DOWN)){
            p_order_down[floor_down] = 1;
        }
    }

    // if(order_floor != 0){ 
    //     Order order = {.target_floor = order_floor};
    //     queue_push_back(queue, order);
    // }

}

void set_floor_button_lights(int* p_order_up, int* p_order_down) {
    // The last floor does not have an up-button: Start at 0.
    for(int floor_up = 0; floor_up < MAX_FLOOR; floor_up++) {
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, p_order_up[floor_up]);
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = 1; floor_down <= MAX_FLOOR; floor_down++) {
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, p_order_down[floor_down]);
    }
}



//May want to change the in-argument to queue
void update_cab_buttons(Order* p_queue) {
    for(int cab_button = MIN_FLOOR; cab_button <= MAX_FLOOR; cab_button++) {
        int order = hardware_read_order(cab_button, HARDWARE_ORDER_INSIDE);
        p_queue[0].cab_orders[cab_button] = order;
        hardware_command_order_light(cab_button, HARDWARE_ORDER_INSIDE, order);
    }
}


