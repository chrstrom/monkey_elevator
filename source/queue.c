#include "queue.h"

void update_queue(Order* p_queue) {
    //slette første element i køen
}

void set_cab_orders(Order* p_current_order){
    //Funksjonen 
    
}


void clear_cab_orders(Order* p_current_order, int current_floor){
    for(int fl = MIN_FLOOR; fl <= MAX_FLOOR; fl++) {
        if(p_current_order->cab_orders[fl] == current_floor){
            p_current_order->cab_orders[fl] = 0;
        }
    }
}

void erase_queue(Order* p_queue){

}

int queue_is_empty(Order* p_queue) {
    return 0;
}

void update_queue_target_floor(Order* p_current_order, int current_floor) {
    for(int cab_order; cab_order < SIZEOF_ARR(p_current_order); cab_order++) {
        if(p_current_order->cab_orders[cab_order] == 1) {
            p_current_order->target_floor = cab_order;
            p_current_order->cab_orders[cab_order] = 0;
            return;
        }
    }
    // If we make it to this point, all orders are 0
    // We can therefore set the order to be handled!
}


int check_order_match(Order* queue, int current_floor, HardwareMovement last_dir) {
    for(int order = 0; order < SIZEOF_ARR(queue); order++) {
        Order current_order = queue[order];
        // Only handle Orders if we have an order AT THIS FLOOR
        // We also need to check if its direction is the same as the last_dir of the elevator
        if(current_order.target_floor == current_floor && current_order.dir == last_dir) {
            return 1;
        }

        int* cab_orders = current_order.cab_orders;
        for(int floor = 0; floor < SIZEOF_ARR(cab_orders); floor++) {
            // Only handle cab orders if we have a cab order AT THE CURRENT FLOOR
            if(cab_orders[floor] && floor == current_floor) {
                return 1;
            }
        }
    }
    return 0;
}
