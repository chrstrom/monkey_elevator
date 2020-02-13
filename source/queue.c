#include "queue.h"

void update_queue(Order* p_queue) {
    //tar inn en kø og skal så slette dette elementet, ved at det er ferdig håndtert.
    //skal da left-skifte resten
    if(QUEUE_SIZE < 2){
        return;
    }
    Order current_order = p_queue[0];
    Order next_order = p_queue[1];
    for(int i = 0; i < QUEUE_SIZE - 1; i++){
        current_order.target_floor = next_order.target_floor;
        // for(int floor = 0; floor < SIZEOF_ARR(current_order.cab_orders); floor++){
        //     current_order.cab_orders[floor] = next_order.cab_orders[floor];
        // }
        p_queue[i] = current_order;
        if (i + 2 <= QUEUE_SIZE - 1){
            next_order = p_queue[i + 2];
        }
        else{
            //might be error-prone. Here 
            next_order = initialize_new_order();
        }
        current_order = p_queue[i + 1];
    }
}

void add_order_to_queue(Order* p_queue) {
    for(int idx = 0; idx < SIZEOF_ARR(UP_ORDERS); idx++) {
        if(UP_ORDERS[idx] == 1 && check_) {
        }
    }

}

int check_queue_for_order(Order* p_queue, int floor, HardwareMovement dir) {
    
}



void set_cab_orders(){
    for(int floor = MIN_FLOOR; floor < MAX_FLOOR; floor++){
        CAB_ORDERS[floor] = hardware_read_order(floor, HARDWARE_ORDER_INSIDE);
    }
}


void clear_cab_orders(int current_floor){
    if(current_floor > MAX_FLOOR || current_floor < MIN_FLOOR) {
        printf("current_floor out of bounds in clear_cab_order()!");
    }

    for(int floor = MIN_FLOOR; floor <= MAX_FLOOR; floor++) {
        if(CAB_ORDERS[floor] == current_floor){
            CAB_ORDERS[floor] = 0;
        }
    }
}

void erase_queue(Order* p_queue){
    //skal her slette hele køen
    //skal kun bli kalt dersom man trykker på stop
    for(int qu = 0; qu < QUEUE_SIZE; qu++){
        p_queue[qu].dir = HARDWARE_ORDER_INSIDE;
        p_queue[qu].target_floor = -1;

    }
}

int queue_is_empty(Order* p_queue) {
    Order temp = p_queue[0]; //We assume that the first element is always updated and thus always correct
    if(temp.target_floor == -1){
        return 1;
    }
    return 0;
}

void update_queue_target_floor(Order* p_current_order, int current_floor) {
    for(int floor; floor < SIZEOF_ARR(CAB_ORDERS); floor++) {
        if(CAB_ORDERS[floor] == 1 & floor != current_floor) {
            p_current_order->target_floor = floor;
            CAB_ORDERS[floor] = 0;
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

        for(int floor = 0; floor < SIZEOF_ARR(CAB_ORDERS); floor++) {
            // Only handle cab orders if we have a cab order AT THE CURRENT FLOOR
            if(CAB_ORDERS[floor] && floor == current_floor) {
                return 1;
            }
        }
    }
    return 0;
}

Order initialize_new_order(){
    Order new_order = {.target_floor = -1, .dir = HARDWARE_ORDER_INSIDE};

}

void push_back_queue(Order* queue, int floor, HardwareMovement dir) {
    Order new_order = {.target_floor = floor, .dir = dir};
     for(int order = 0; order < SIZEOF_ARR(queue); order++) {
        if(queue[order].target_floor == -1) {
            queue[order] = new_order;
        }
    }
}