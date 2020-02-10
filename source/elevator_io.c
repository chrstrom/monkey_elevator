#include "elevator_io.h"

int at_floor() {
    for(int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
       if(hardware_read_floor_sensor(floor)) {
           return floor;
       }
    }

    return 0;
}

// By passing p_order up/down, we make sure to not add a new order for floors that already
// have orders to them. Note that these arrays are only updated by external buttons.
// The internal cab-buttons will have no impact on this.
void poll_floor_buttons(Order* queue, int* p_order_up, int* p_order_down) {
    
    int order_floor = 0;
  
    for(int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
        int order_up = 0;
        int order_down = 0;

        if(!p_order_up[floor]){
            order_up = hardware_read_order(floor, HARDWARE_ORDER_UP);
            p_order_up[floor] = 1;
        }
     
        if(!p_order_down[floor]){
            order_down = hardware_read_order(floor, HARDWARE_ORDER_DOWN);
            p_order_down[floor] = 1;
        }

        if(order_up || order_down) {
            order_floor = floor;
        }
    }

    if(order_floor != 0){ 
        Order order = {.floor_at = order_floor};
        queue->push_back(order);
    }

}

//May want to change the in-argument to queue
void update_cab_buttons(Order* p_current_order) {
    for(int cab_button = MIN_FLOOR; cab_button <= MAX_FLOOR; cab_button++) {
        p_current_order->floor_to[cab_button] = hardware_read_order(cab_button, HARDWARE_ORDER_INSIDE);
    }
}