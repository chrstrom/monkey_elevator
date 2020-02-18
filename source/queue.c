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
    int current_floor = at_floor();
    if(current_floor == -1){
        return;
    }

    // If there are any orders with target_floor = INVALID_ORDER, restructurate the queue
    delete_holes_in_queue();
    
    // QUEUE[0].target_floor should be INVALID_ORDER, if we had a target at the current_floor
    // since we clear the orders before calling this function
    if(QUEUE[0].target_floor == INVALID_ORDER){ 
        for (int ord = 0; ord < QUEUE_SIZE; ord++){
            if (ord < QUEUE_SIZE - 1){
                QUEUE[ord].target_floor = QUEUE[ord + 1].target_floor;
                QUEUE[ord].order_type = QUEUE[ord + 1].order_type;
            }
            else if (ord == QUEUE_SIZE - 1){
                QUEUE[ord].target_floor = INVALID_ORDER;
                QUEUE[ord].order_type = HARDWARE_ORDER_NOT_INIT;
            }
        }
    }
}

void erase_queue(elevator_data_t* p_data){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        clear_orders_at_floor(p_data, floor);
    }
}

void erase_single_order(int floor, HardwareOrder order_type){
    for(int ord = 0; ord < QUEUE_SIZE; ord++){
        if(QUEUE[ord].target_floor == floor && QUEUE[ord].order_type == order_type){
            QUEUE[ord].target_floor = INVALID_ORDER;
            QUEUE[ord].order_type = HARDWARE_ORDER_NOT_INIT;
        }
    }
}

void delete_holes_in_queue(){
    for(int ord = 0; ord < QUEUE_SIZE; ord++){
        if(QUEUE[ord].target_floor == INVALID_ORDER){
            for(int inv = ord; inv < QUEUE_SIZE; inv++){
                if(QUEUE[inv].target_floor != INVALID_ORDER){
                    QUEUE[ord].target_floor = QUEUE[inv].target_floor;
                    QUEUE[ord].order_type = QUEUE[inv].order_type;
                    QUEUE[inv].target_floor = INVALID_ORDER;
                    QUEUE[inv].order_type = HARDWARE_ORDER_NOT_INIT;
                }
            }
        }
    }
}

void push_back_queue(int target_floor, HardwareOrder order_type) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(check_queue_for_order(target_floor, order_type) == 1) {
            return;
        }
    }

    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == INVALID_ORDER) {
            QUEUE[order].target_floor = target_floor;
            QUEUE[order].order_type = order_type;
            break;
        }
    }
}

void push_front_queue(int floor, HardwareOrder order_type){
    // Add a new order to the front of the queue
    // We can always assume that the QUEUE will have space, due to simple math (not shown here)
    
    // Rightshifting of the queue
    for(int ord = QUEUE_SIZE - 1; ord > 0; ord--){
        QUEUE[ord].order_type = QUEUE[ord - 1].order_type;
        QUEUE[ord].target_floor = QUEUE[ord - 1].target_floor;
    }
    QUEUE[0].target_floor = floor;
    QUEUE[0].order_type = order_type;
}

int queue_is_empty() {
    //We assume that the first element is always updated and thus always correct
    if(QUEUE[0].target_floor == INVALID_ORDER){
        return 1;
    }
    return 0;
}


int check_queue_for_order(int floor, HardwareOrder order_type) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == floor && QUEUE[order].order_type == order_type) {
            return 1;
        }
    }
    return 0;
}

int check_order_match(elevator_data_t* p_data) {
    int current_floor = at_floor();
    if(QUEUE[0].target_floor == current_floor) {
        return 1;
    }

    for(int order = 1; order < QUEUE_SIZE; order++) {
        Order current_order = QUEUE[order];
        // Only handle Orders if we have an order AT THIS FLOOR
        // We also need to check if its direction is the same as the last_dir of the elevator
        if(current_order.target_floor == current_floor && current_order.order_type == p_data->last_dir) {
            return 1;
        }

        if(current_order.target_floor == current_floor && current_order.order_type == HARDWARE_ORDER_INSIDE) {
            return 1;
        }
    }

    return 0;
}

void clear_orders_at_floor(elevator_data_t* p_data, int current_floor) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        int order_floor = QUEUE[order].target_floor;
        if(order_floor == current_floor) {
            p_data->ORDERS_CAB[order_floor] = 0;
            p_data->ORDERS_UP[order_floor] = 0;
            p_data->ORDERS_DOWN[order_floor] = 0;
            QUEUE[order].target_floor = INVALID_ORDER;
            QUEUE[order].order_type = HARDWARE_ORDER_NOT_INIT;
        }
    }

    set_cab_button_lights(p_data);
    set_floor_button_lights(p_data);
    update_queue();
}


void set_cab_orders(elevator_data_t* p_data){
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++){
        p_data->ORDERS_CAB[floor] = hardware_read_order(floor, HARDWARE_ORDER_INSIDE);
    }
}

void clear_cab_orders(elevator_data_t* p_data, int current_floor){
    if(current_floor > HARDWARE_NUMBER_OF_FLOORS || current_floor < MIN_FLOOR) {
        printf("current_floor out of bounds in clear_cab_order()!");
    }

    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(p_data->ORDERS_CAB[floor] == current_floor){
            p_data->ORDERS_CAB[floor] = 0;
        }
    }
}


