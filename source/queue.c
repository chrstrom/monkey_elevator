#include "queue.h"

void update_queue(Order* p_queue) {
    //slette første element i køen
}

void set_cab_orders(Order* p_current_order){
    //Funksjonen skal 
    
}

//Just invalidating the values of the order
void clear_cab_orders(Order* p_current_order, int current_floor){
    for(int fl = MIN_FLOOR; fl <= MAX_FLOOR; fl++) {
        if(p_current_order->floor_to[fl] == current_floor){
            p_current_order->floor_to[fl] = 0;
        }
    }
}

void erase_queue(Order* p_queue){

}

