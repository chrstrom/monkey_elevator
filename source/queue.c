#include "queue.h"
#include "elevator_io.h"
#include <stdio.h>

void init_queue() {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        QUEUE[i].target_floor = INVALID_ORDER;
        QUEUE[i].order_type =  HARDWARE_ORDER_NOT_INIT;
    }
}

void update_queue(){
    if(get_current_floor() == BETWEEN_FLOORS){
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

void erase_queue(int* p_orders_up, int* p_orders_down, int* p_orders_cab){
    for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        clear_orders_at_floor(p_orders_cab, p_orders_up, p_orders_down, floor);
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
                    break;
                }
            }
        }
    }
}

void push_back_queue(int target_floor, HardwareOrder order_type) {
    //her burde det være en logikk som legger ting i rett rekkefølge
    //altså dersom man har en ordre som skal ned i en høyere etasje, så kjører man først til den etasjen før man tar den lenger ned
    //også motsatt: dersom man får ordre fra en etasje lengre ned som skal opp, så kjører man først til den etasjen
    for(int order = 0; order < QUEUE_SIZE; order++) {
        if(check_order_match(target_floor, order_type) == 1) {
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

    //kall en sorteringsfunksjon som sorterer ordrene etter slik logikk som forespeilet øverst
}

// void push_front_queue(int floor, HardwareOrder order_type){
//     //OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOoppdateeeeeeeeeeeeeeeeeeeer
//     // for(int ord = QUEUE_SIZE - 1; ord > 0; ord--){
//     //     QUEUE[ord].order_type = QUEUE[ord - 1].order_type;
//     //     QUEUE[ord].target_floor = QUEUE[ord - 1].target_floor;
//     // }
//     sort_queue();
//     for(int cab_order = 0; cab_order < QUEUE_SIZE; cab_order++){
//         if(QUEUE[cab_order].order_type != HARDWARE_ORDER_INSIDE){
//             Order temp = QUEUE[cab_order];
//             QUEUE[cab_order].target_floor = INVALID_ORDER;
//             QUEUE[cab_order].order_type = HARDWARE_ORDER_NOT_INIT;
//             for(int floor_order = cab_order + 1; floor_order < QUEUE_SIZE; floor_order++){
//                 Order temp2 = QUEUE[floor_order];
//                 QUEUE[floor_order].target_floor = temp.target_floor;
//                 QUEUE[floor_order].order_type = temp.order_type;
//                 temp = temp2;
//             }
//             QUEUE[cab_order].target_floor = floor;
//             QUEUE[cab_order].order_type =  order_type;
//             break;
//         }
//     }
// }

int check_queue_empty() {
    delete_holes_in_queue();
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
            QUEUE[order].target_floor = INVALID_ORDER;
            QUEUE[order].order_type = HARDWARE_ORDER_NOT_INIT;
        }
    }

    //set_cab_button_lights(p_orders_cab);
    //set_floor_button_lights(p_orders_up, p_orders_down);
    update_queue();
}

// void sort_queue(){
//     //sorteringen skal sortere ordrene i heisen til først å gå etter ordre innenfra
//     //så skal den prioritere ordre utenfra

//     delete_holes_in_queue();

//     Order* temp_queue = QUEUE;
//      for(int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
//         QUEUE[floor].target_floor = INVALID_ORDER;
//         QUEUE[floor].order_type = HARDWARE_ORDER_NOT_INIT;
//     }

//     for(int order = 0; order < QUEUE_SIZE; order++) {
//         Order temp_order = temp_queue[order];
//         if(temp_order.order_type == HARDWARE_ORDER_INSIDE) {
//             push_front_queue(temp_order.target_floor, temp_order.order_type);
//         }
//         else {
//             push_back_queue(temp_order.target_floor, temp_order.order_type);
//         }
//     }

    // //sorting of the HARDWARE_ORDER_INSIDE/cab_buttons
    // int sorted_queue_number = 0;
    // for(int ord = 0; ord < QUEUE_SIZE; ord++){
    //     if(QUEUE[ord].order_type == HARDWARE_ORDER_INSIDE){
    //         if(QUEUE[sorted_queue_number].order_type != HARDWARE_ORDER_INSIDE){
    //             int temp_floor = QUEUE[sorted_queue_number].target_floor;
    //             HardwareOrder temp_order_type = QUEUE[sorted_queue_number].order_type;
    //             QUEUE[sorted_queue_number].target_floor = QUEUE[ord].target_floor;
    //             QUEUE[sorted_queue_number].order_type = QUEUE[ord].order_type;
    //             QUEUE[ord].target_floor = temp_floor;
    //             QUEUE[ord].order_type = temp_order_type;
    //             sorted_queue_number++;
    //         }
    //     }
    // }

    // //skal sortere køen etter rekkefølgen som heisen skal prioritere av eksterne ordre

    // //funksjonen vil endre target_floor til heisen

    // int possible_target_floor = INVALID_ORDER;
    // if (current_dir == HARDWARE_ORDER_UP){
    //     for (int ord = 0; ord < QUEUE_SIZE; ord++){
    //         if (QUEUE[ord].order_type == HARDWARE_ORDER_DOWN)
    //         {
    //             if(possible_target_floor == INVALID_ORDER){
    //                 possible_target_floor = QUEUE[ord].target_floor;
    //             }
    //             possible_target_floor = QUEUE[ord].target_floor;
    //         }
    //     }
    // }
// }
