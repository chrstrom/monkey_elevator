#include "queue.h"
#include "elevator_io.h"
#include <stdio.h>

void init_queue() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        QUEUE[i].target_floor = -2;
        QUEUE[i].order_type =  HARDWARE_ORDER_NOT_INIT;
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
    Order new_order = {.target_floor = INVALID_ORDER, .order_type = HARDWARE_ORDER_NOT_INIT};
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
            next_order = new_order;
        }
        current_order = QUEUE[i + 1];
    }
}

void add_order_to_queue(int target_floor, HardwareOrder order_type) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(check_queue_for_order(target_floor, order_type) == 1) {
            return;
        }
    }

    Order new_order = {.target_floor = target_floor, .order_type = order_type};
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == INVALID_ORDER) {
            QUEUE[order] = new_order;
            break;
        }
    }
}

int check_queue_for_order(int floor, HardwareOrder order_type) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == floor && QUEUE[order].order_type == order_type) {
            return 1;
        }
    }
    return 0;
}

void set_cab_orders(elevator_data_t* data){
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++){
        data->ORDERS_CAB[floor] = hardware_read_order(floor, HARDWARE_ORDER_INSIDE);
    }
}


void clear_cab_orders(elevator_data_t* data, int current_floor){
    if(current_floor > HARDWARE_NUMBER_OF_FLOORS || current_floor < MIN_FLOOR) {
        printf("current_floor out of bounds in clear_cab_order()!");
    }

    for(int floor = MIN_FLOOR; floor <= HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(data->ORDERS_CAB[floor] == current_floor){
            data->ORDERS_CAB[floor] = 0;
        }
    }
}

void clear_orders_at_floor(elevator_data_t* data, int current_floor) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        int order_floor = QUEUE[order].target_floor;
        if(order_floor == current_floor) {
            data->ORDERS_CAB[order_floor] = 0;
            data->ORDERS_UP[order_floor] = 0;
            data->ORDERS_DOWN[order_floor] = 0;
            QUEUE[order].target_floor = INVALID_ORDER;
            QUEUE[order].order_type = HARDWARE_ORDER_NOT_INIT;
        }
    }

    set_cab_button_lights(data);
    set_floor_button_lights(data);
}

void erase_queue(elevator_data_t* data){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        clear_orders_at_floor(data, floor);
    }
}

int queue_is_empty() {
    //We assume that the first element is always updated and thus always correct
    if(QUEUE[0].target_floor == INVALID_ORDER){
        return 1;
    }
    return 0;
}


int check_order_match(elevator_data_t* data, int current_floor, HardwareMovement last_dir) {
    if(QUEUE[0].target_floor == current_floor) {
        return 1;
    }

    for(int order = 1; order < QUEUE_SIZE; order++) {
        Order current_order = QUEUE[order];
        // Only handle Orders if we have an order AT THIS FLOOR
        // We also need to check if its direction is the same as the last_dir of the elevator
        if(current_order.target_floor == current_floor && current_order.order_type == last_dir) {
            return 1;
        }

        if(current_order.target_floor == current_floor && current_order.order_type == HARDWARE_ORDER_INSIDE) {
            return 1;
        }
    }

    return 0;
}


// void push_back_queue(int floor, HardwareMovement dir) {
//     Order new_order = {.target_floor = floor, .dir = dir};
//      for(int order = 0; order < QUEUE_SIZE; order++) {
//         if(QUEUE[order].target_floor == INVALID_ORDER) {
//             QUEUE[order] = new_order;
//             break;
//         }
//     }
// }
