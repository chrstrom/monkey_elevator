#include "queue.h"

void init_queue() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        QUEUE[i].target_floor = -2;
        QUEUE[i].dir = HARDWARE_MOVEMENT_STOP;
    }
}


void update_queue(){
    //tar inn en kø og skal så slette dette elementet, ved at det er ferdig håndtert.
    //skal da left-skifte resten
    if(QUEUE_SIZE < 2){
        return;
    }
    Order current_order = QUEUE[0];
    Order next_order = QUEUE[1];
    for(int i = 0; i < QUEUE_SIZE - 1; i++){
        current_order.target_floor = next_order.target_floor;
        // for(int floor = 0; floor < SIZEOF_ARR(current_order.cab_orders); floor++){
        //     current_order.cab_orders[floor] = next_order.cab_orders[floor];
        // }
        QUEUE[i] = current_order;
        if (i + 2 <= QUEUE_SIZE - 1){
            next_order = QUEUE[i + 2];
        }
        else{
            //might be error-prone. Here 
            next_order = initialize_new_order();
        }
        current_order = QUEUE[i + 1];
    }
}

void add_order_to_queue() {
    for(int floor_up = 0; floor_up < HARDWARE_NUMBER_OF_FLOORS; floor_up++) {
        if(ORDERS_UP[floor_up] == 1 && check_queue_for_order(floor_up, HARDWARE_MOVEMENT_UP) == 0) {
            push_back_queue(floor_up, HARDWARE_MOVEMENT_UP);
        }
    }

    for(int floor_down = 0; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        if(ORDERS_DOWN[floor_down] == 1 && check_queue_for_order(floor_down, HARDWARE_MOVEMENT_DOWN) == 0) {
            push_back_queue(floor_down, HARDWARE_MOVEMENT_DOWN);
        }
    }

}

void push_back_queue(int floor, HardwareMovement dir) {
    Order new_order = {.target_floor = floor, .dir = dir};
     for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == -1) {
            QUEUE[order] = new_order;
        }
    }
}

int check_queue_for_order(int floor, HardwareMovement dir) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == floor && QUEUE[order].dir == dir) {
            return 1;
        }
    }
    return 0;
}

void set_cab_orders(){
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++){
        ORDERS_CAB[floor] = hardware_read_order(floor, HARDWARE_ORDER_INSIDE);
    }
}


void clear_cab_orders(int current_floor){
    if(current_floor > HARDWARE_NUMBER_OF_FLOORS || current_floor < MIN_FLOOR) {
        printf("current_floor out of bounds in clear_cab_order()!");
    }

    for(int floor = MIN_FLOOR; floor <= HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(ORDERS_CAB[floor] == current_floor){
            ORDERS_CAB[floor] = 0;
        }
    }
}

void erase_queue(){
    //skal her slette hele køen
    //skal kun bli kalt dersom man trykker på stop
    for(int order = 0; order < QUEUE_SIZE; order++){
        QUEUE[order].dir = HARDWARE_ORDER_INSIDE;
        QUEUE[order].target_floor = -1;

    }
}

int queue_is_empty() {
    //We assume that the first element is always updated and thus always correct
    if(QUEUE[0].target_floor == -1){
        return 1;
    }
    return 0;
}

void update_target_floor(Order* p_current_order, int current_floor) {
    for(int floor; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(ORDERS_CAB[floor] == 1 && floor != current_floor) {
            p_current_order->target_floor = floor;
            ORDERS_CAB[floor] = 0;
            return;
        }
    }
    // If we make it to this point, all orders are 0
    // We can therefore set the order to be handled!
    p_current_order->target_floor = -1;
}


int check_order_match(int current_floor, HardwareMovement last_dir) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        Order current_order = QUEUE[order];
        // Only handle Orders if we have an order AT THIS FLOOR
        // We also need to check if its direction is the same as the last_dir of the elevator
        if(current_order.target_floor == current_floor && current_order.dir == last_dir) {
            return 1;
        }

        for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
            // Only handle cab orders if we have a cab order AT THE CURRENT FLOOR
            if(ORDERS_CAB[floor] == 1 && floor == current_floor) {
                return 1;
            }
        }
    }
    return 0;
}

//Pherhaps just delete this! Isn't used anywhere in the code, and has zero
//use in the code, since we already have defined the orders
Order initialize_new_order(){
    Order new_order = {.target_floor = -1, .dir = HARDWARE_ORDER_INSIDE};
    return new_order;
}

void push_back_queue(int floor, HardwareMovement dir) {
    Order new_order = {.target_floor = floor, .dir = dir};
     for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == INVALID_ORDER) {
            QUEUE[order] = new_order;
        }
    }
}
