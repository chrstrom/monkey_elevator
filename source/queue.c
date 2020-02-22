#include "queue.h"
#include "elevator_io.h"
#include <stdio.h>


void set_single_order(int idx, int target_floor, HardwareOrder order_type) {
    QUEUE[idx].target_floor = target_floor;
    QUEUE[idx].order_type = order_type;
}


int check_queue_empty() {
    refactor_queue();
    return QUEUE[0].target_floor == FLOOR_NOT_INIT;
}


void init_queue() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        set_single_order(i, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
    }
}


void erase_queue(int* p_orders_up, int* p_orders_down, int* p_orders_cab){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        clear_orders_at_floor(p_orders_up, p_orders_down, p_orders_cab, floor);
    }
}


void clear_orders_at_floor(int* p_orders_up, int* p_orders_down, int* p_orders_cab, int current_floor) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == current_floor) {
            set_single_order(order, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
        }
    }

    p_orders_cab[current_floor] = 0;
    p_orders_up[current_floor] = 0;
    p_orders_down[current_floor] = 0;

    update_queue();
}


void refactor_queue(){
    for(int order = 0; order < QUEUE_SIZE; order++){
        if(QUEUE[order].target_floor == FLOOR_NOT_INIT){
            for(int hole = order; hole < QUEUE_SIZE; hole++){
                if(QUEUE[hole].target_floor != FLOOR_NOT_INIT){
                    set_single_order(order, QUEUE[hole].target_floor, QUEUE[hole].order_type);
                    set_single_order(hole, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
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
        if(QUEUE[order].target_floor == FLOOR_NOT_INIT) {
            set_single_order(order, target_floor, order_type);
            break;
        }
    }
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


void clear_orders_at_floor(int* p_orders_up, int* p_orders_down, int* p_orders_cab, int current_floor) {
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(QUEUE[order].target_floor == current_floor) {
            set_single_order(order, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
        }
    }

    p_orders_cab[current_floor] = 0;
    p_orders_up[current_floor] = 0;
    p_orders_down[current_floor] = 0;

    update_queue();
}
