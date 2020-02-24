#include "elevator_io.h"
#include "globals.h"
#include "queue.h"


int update_valid_floor(int valid_floor) {
    return (get_current_floor() == BETWEEN_FLOORS ? valid_floor : get_current_floor());
}


void set_floor_indicator_light(int floor) {
    if(floor != BETWEEN_FLOORS) {
        hardware_command_floor_indicator_on(floor);
    }
}


int get_current_floor() {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
       if(hardware_read_floor_sensor(floor)) {
           return floor;
       }
    }
    return BETWEEN_FLOORS;
}


void update_cab_buttons(int* p_orders_cab) {
    for(int floor = MIN_FLOOR; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if(p_orders_cab[floor] == 0 && hardware_read_order(floor, HARDWARE_ORDER_INSIDE)) {
            queue_push_back(floor, HARDWARE_ORDER_INSIDE);
            p_orders_cab[floor] = 1;
        }
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, p_orders_cab[floor]);
    }
}


void update_floor_buttons(int* p_orders_up, int* p_orders_down) {
    // The last floor does not have an up-button: Start at 0.
    for(int floor_up = MIN_FLOOR; floor_up < HARDWARE_NUMBER_OF_FLOORS - 1; floor_up++) {
        if(p_orders_up[floor_up] == 0 && hardware_read_order(floor_up, HARDWARE_ORDER_UP) == 1){
            queue_push_back(floor_up, HARDWARE_ORDER_UP);
            p_orders_up[floor_up] = 1;
        }
        hardware_command_order_light(floor_up, HARDWARE_ORDER_UP, p_orders_up[floor_up]);
    }

    // The first floor does not have a down-button: Start at 1.
    for(int floor_down = MIN_FLOOR + 1; floor_down < HARDWARE_NUMBER_OF_FLOORS; floor_down++) {
        if(p_orders_down[floor_down] == 0 && hardware_read_order(floor_down, HARDWARE_ORDER_DOWN) == 1){
            queue_push_back(floor_down, HARDWARE_ORDER_DOWN);
            p_orders_down[floor_down] = 1;
        }
        hardware_command_order_light(floor_down, HARDWARE_ORDER_DOWN, p_orders_down[floor_down]);
    }
}