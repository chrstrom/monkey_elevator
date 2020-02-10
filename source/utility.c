#include "utility.h"

int at_floor() {
    for(int floor = 0; floor < MAX_FLOOR; floor++){
        if(hardware_read_floor_sensor(floor)){
            return floor;
        }
    }
    return -1;
    // returns the floor the elevator is at, or -1 if the elevator is between floors
}

void add_floor_orders(int* p_up, int* p_down) {

}

void add_cab_orders(int* p_cab) {

}
