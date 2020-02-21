#include "queue.h"
#include "elevator_io.h"
#include <stdio.h>


void init_queue() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        set_single_order(i, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
    }
}

void update_queue(){
    if(get_current_floor() == BETWEEN_FLOORS){
        return;
    }

    // If there are any orders with target_floor = FLOOR_NOT_INIT, restructure the queue
    refactor_queue();
    
    // QUEUE[0].target_floor should be FLOOR_NOT_INIT, if we had a target at the current_floor since we clear the orders before calling this function
    if(QUEUE[0].target_floor == FLOOR_NOT_INIT){ 
        for (int ord = 0; ord < QUEUE_SIZE; ord++){
            if (ord < QUEUE_SIZE - 1){
                set_single_order(ord, QUEUE[ord + 1].target_floor, QUEUE[ord + 1].order_type);
            }
            else if (ord == QUEUE_SIZE - 1){
                set_single_order(ord, FLOOR_NOT_INIT, HARDWARE_ORDER_NOT_INIT);
            }
        }
    }
}

void erase_queue(int* p_orders_up, int* p_orders_down, int* p_orders_cab){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        clear_orders_at_floor(p_orders_up, p_orders_down, p_orders_cab, floor);
    }
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

void set_single_order(int idx, int target_floor, HardwareOrder order_type) {
    QUEUE[idx].target_floor = target_floor;
    QUEUE[idx].order_type = order_type;
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


int check_queue_empty() {
    refactor_queue();
    return QUEUE[0].target_floor == FLOOR_NOT_INIT;
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


//Trenger en sorteringsalgoritme slik at vi kan styre heisen til rett etasje
void sort_queue(HardwareMovement current_dir){
    int num_cab_orders = 0;
    int num_up_orders = 0;
    int num_down_orders = 0;
    //get state of the current queue
    Order cab_order[QUEUE_SIZE];
    Order up_order[QUEUE_SIZE];
    Order down_order[QUEUE_SIZE];
    for(int order = 0; order < QUEUE_SIZE; order++){
        if(QUEUE[order].order_type == HARDWARE_ORDER_UP){
            up_order[num_up_orders] = QUEUE[order];
            num_up_orders++;
        }
        if(QUEUE[order].order_type == HARDWARE_ORDER_DOWN){
            down_order[num_down_orders] = QUEUE[order];
            num_down_orders++;
        }
        if(QUEUE[order].order_type == HARDWARE_ORDER_INSIDE){
            cab_order[num_cab_orders] = QUEUE[order];
            num_cab_orders++;
        }
    }
    //prioriteten mellom de ordrene med samme type, baseres på etasje
    for(int ord_up = 0; ord_up < num_up_orders; ord_up++){
        for(int temp = ord_up; temp < num_up_orders; temp++){
            if(up_order[ord_up].target_floor > up_order[temp].target_floor){
                Order temp_order;
                temp_order.target_floor = up_order[temp].target_floor;
                temp_order.order_type = up_order[temp].order_type;
                up_order[ord_up] = up_order[temp];
                up_order[temp] = temp_order;
            }
        }
    }
    for(int ord_down = 0; ord_down < num_down_orders; ord_down++){
        for(int temp = ord_down; temp < num_down_orders; temp++){
            if(down_order[ord_down].target_floor < down_order[temp].target_floor){
                Order temp_order;
                temp_order.target_floor = down_order[temp].target_floor;
                temp_order.order_type = down_order[temp].order_type;
                down_order[ord_down] = down_order[temp];
                down_order[temp] = temp_order;
            }
        }
    }

    for(int num_cab = 0; num_cab < num_cab_orders; num_cab++){
        QUEUE[num_cab] = cab_order[num_cab];
    }
    if(current_dir == HARDWARE_MOVEMENT_DOWN){
        //prioritet gis til ordre ned
        for(int num_down = 0; num_down < num_down_orders; num_down++){
            QUEUE[num_down + num_cab_orders] = down_order[num_down];
        }
        for(int num_up = 0; num_up < num_up_orders; num_up++){
            QUEUE[num_up + num_cab_orders + num_down_orders] = up_order[num_up];
        }
    }
    if(current_dir == HARDWARE_MOVEMENT_UP){
        //prioritet gis til ordre opp
        for(int num_up = 0; num_up < num_up_orders; num_up++){
            QUEUE[num_up + num_cab_orders] = up_order[num_up];
        }
        for(int num_down = 0; num_down < num_down_orders; num_down++){
            QUEUE[num_down + num_cab_orders + num_up_orders] = down_order[num_down];
        }
    }
}