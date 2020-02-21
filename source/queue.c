#include "queue.h"
#include "elevator_io.h"
#include <stdio.h>

void set_single_order(int idx, int target_floor, HardwareOrder order_type) {
    QUEUE[idx].target_floor = target_floor;
    QUEUE[idx].order_type = order_type;
}

void init_queue() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        set_single_order(i, INVALID_ORDER, HARDWARE_ORDER_NOT_INIT);
    }
}

void update_queue(){
    if(get_current_floor() == BETWEEN_FLOORS){
        return;
    }

    // If there are any orders with target_floor = INVALID_ORDER, restructure the queue
    refactor_queue();
    
    // QUEUE[0].target_floor should be INVALID_ORDER, if we had a target at the current_floor since we clear the orders before calling this function
    if(QUEUE[0].target_floor == INVALID_ORDER){ 
        for (int ord = 0; ord < QUEUE_SIZE; ord++){
            if (ord < QUEUE_SIZE - 1){
                set_single_order(ord, QUEUE[ord + 1].target_floor, QUEUE[ord + 1].order_type);
            }
            else if (ord == QUEUE_SIZE - 1){
                set_single_order(ord, INVALID_ORDER, HARDWARE_ORDER_NOT_INIT);
            }
        }
    }
}

void erase_queue(int* p_orders_up, int* p_orders_down, int* p_orders_cab){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        clear_orders_at_floor(p_orders_cab, p_orders_up, p_orders_down, floor);
    }
}


void refactor_queue(){
    for(int order = 0; order < QUEUE_SIZE; order++){
        if(QUEUE[order].target_floor == INVALID_ORDER){
            for(int hole = order; hole < QUEUE_SIZE; hole++){
                if(QUEUE[hole].target_floor != INVALID_ORDER){
                    set_single_order(order, QUEUE[hole].target_floor, QUEUE[hole].order_type);
                    set_single_order(hole, INVALID_ORDER, HARDWARE_ORDER_NOT_INIT);
                    break;
                }
            }
        }
    }
}

void push_back_queue(int target_floor, HardwareOrder order_type) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(check_order_match(target_floor, order_type) == 1) {
            return; // Return if we have an order with the same parameters in the queue already
        }
    }

    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == INVALID_ORDER) {
            set_single_order(order, target_floor, order_type);
            break;
        }
    }
}


int check_queue_empty() {
    refactor_queue();
    return QUEUE[0].target_floor == INVALID_ORDER;
}


int check_order_match(int current_floor, HardwareOrder order_type) {
    if(QUEUE[0].target_floor == current_floor) {
        return 1;
    }

    for(int order = 1; order < QUEUE_SIZE; order++) {
        Order current_order = QUEUE[order];
        if(current_order.target_floor == current_floor && (current_order.order_type == order_type || current_order.order_type == HARDWARE_ORDER_INSIDE)) {
            return 1;
        }
    }

    return 0;
}

void clear_orders_at_floor(int* p_orders_cab, int* p_orders_up, int* p_orders_down, int current_floor) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        int order_floor = QUEUE[order].target_floor;
        if(order_floor == current_floor) {
            p_orders_cab[order_floor] = 0;
            p_orders_up[order_floor] = 0;
            p_orders_down[order_floor] = 0;
            set_single_order(order, INVALID_ORDER, HARDWARE_ORDER_NOT_INIT);
        }
    }

    update_queue();
}
