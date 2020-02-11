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
        for(int floor = 0; floor < SIZEOF_ARR(current_order.cab_orders); floor++){
            current_order.cab_orders[floor] = next_order.cab_orders[floor];
        }
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

void set_cab_orders(Order* p_current_order, int current_idx){
    //skal polle alle knappene og sjekke om cab_orders_skal bli satt
    //alle knappene som skal bli satt, må endres fra -1 til 1. Alle som ikke er endret på,
    //skal da bli satt til 0
    if(current_idx > QUEUE_SIZE){
        fprintf(stderr, "Out of range-error for set_cab_orders!");
        exit(1);
    }
    Order current_order = p_current_order[current_idx];
    for(int fl = MIN_FLOOR; fl < MAX_FLOOR; fl++){
        current_order.cab_orders[fl] = hardware_read_order(fl,HARDWARE_ORDER_INSIDE);
    }
}


void clear_cab_orders(Order* p_current_order, int current_floor){
    for(int fl = MIN_FLOOR; fl <= MAX_FLOOR; fl++) {
        if(p_current_order->cab_orders[fl] == current_floor){
            p_current_order->cab_orders[fl] = 0;
        }
    }
}

void erase_queue(Order* p_queue){
    //skal her slette hele køen
    //skal kun bli kalt dersom man trykker på stop
    for(int qu = 0; qu < QUEUE_SIZE; qu++){
        Order temp = p_queue[qu];
        temp.target_floor = -1;
        temp.dir = HARDWARE_ORDER_INSIDE;
        for(int fl = MIN_FLOOR; fl < MAX_FLOOR; fl++){
            temp.cab_orders[fl] = -1;
        }
    }
}

int queue_is_empty(Order* p_queue) {
    Order temp = p_queue[0]; //We assume that the first element is always updated and thus always correct
    if(temp.target_floor == -1){
        return 1;
    }
    return 0;
}

void update_queue_target_floor(Order* p_current_order, int floor) {

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

Order initialize_new_order(){
    Order new_order = {.target_floor = -1, .dir = HARDWARE_ORDER_INSIDE, .cab_orders = { -1, -1, -1, -1 }};
}