#include "timer.h"

void start_timer(time_t* p_timer){
    *(p_timer) = time(NULL);
}

int check_timer(time_t* p_timer){
    unsigned int time_diff = (unsigned int)(time(NULL) - *(p_timer));
    //Might be a bug here! Comparing unsigned int and int
    if(time_diff >= NORMAL_WAIT_TIME){
        return 1;
    }
    return 0;
}


// void start_timer_new() {
//     LAST_TIME = time(NULL);
// }

// int check_timer_new(int time_req) {
//     unsigned int time_diff = (unsigned int)(time(NULL)) - LAST_TIME;
//     return time_diff >= time_req;
// }

