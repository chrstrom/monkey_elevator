#include "timer.h"

void start_timer(time_t* p_timer){
    *(p_timer) = time(0);
}

int check_timer(time_t* p_timer){
    unsigned int time_diff = (unsigned int)(time(0) - *(p_timer));
    //Might be a bug here! Comparing unsigned int and int
    if(time_diff >= NORMAL_WAIT_TIME){
        return 1;
    }
    return 0;
}


