#include "queue.h"
#include "elevator_io.h"
#include <stdio.h>


int queue_empty() {
    queue_refactor();
    return QUEUE[0].target_floor == FLOOR_NOT_INIT;
}


void queue_init() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        queue_set_order(i, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
    }
}


void queue_set_order(int idx, int target_floor, HardwareOrder order_type) {
    QUEUE[idx].target_floor = target_floor;
    QUEUE[idx].order_type = order_type;
}


void queue_erase(int* p_orders_up, int* p_orders_down, int* p_orders_cab){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        queue_clear_order_at_floor(p_orders_up, p_orders_down, p_orders_cab, floor);
    }
}


void queue_push_back(int target_floor, HardwareOrder order_type) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(queue_check_order_match(target_floor, order_type) == 1) {
            return; // Return if we have an order with the same parameters in the queue already
        }
    }

    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == FLOOR_NOT_INIT) {
            queue_set_order(order, target_floor, order_type);
            break;
        }
    }
}


void queue_clear_order_at_floor(int* p_orders_up, int* p_orders_down, int* p_orders_cab, int current_floor) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == current_floor) {
            queue_set_order(order, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
        }
    }

    p_orders_cab[current_floor] = 0;
    p_orders_up[current_floor] = 0;
    p_orders_down[current_floor] = 0;

    queue_update();
}


void queue_refactor(){
    for(int order = 0; order < QUEUE_SIZE; order++){
        if(QUEUE[order].target_floor == FLOOR_NOT_INIT){
            for(int hole = order; hole < QUEUE_SIZE; hole++){
                if(QUEUE[hole].target_floor != FLOOR_NOT_INIT){
                    queue_set_order(order, QUEUE[hole].target_floor, QUEUE[hole].order_type);
                    queue_set_order(hole, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
                    break;
                }
            }
        }
    }
}


int queue_check_order_match(int current_floor, HardwareOrder order_type) {
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


void queue_update(){
    if(get_current_floor() == BETWEEN_FLOORS){
        return;
    }

    // If there are any orders with target_floor = FLOOR_NOT_INIT, restructure the queue
    queue_refactor();
    
    // QUEUE[0].target_floor should be FLOOR_NOT_INIT, if we had a target at the current_floor since we clear the orders before calling this function
    if(QUEUE[0].target_floor == FLOOR_NOT_INIT){ 
        for (int ord = 0; ord < QUEUE_SIZE; ord++){
            if (ord < QUEUE_SIZE - 1){
                queue_set_order(ord, QUEUE[ord + 1].target_floor, QUEUE[ord + 1].order_type);
            }
            else if (ord == QUEUE_SIZE - 1){
                queue_set_order(ord, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
            }
        }
    }
}