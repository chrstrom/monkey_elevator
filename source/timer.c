#include <stdio.h>
#include "timer.h"

void start_timer(time_t* p_timer){
    *(p_timer) = time(NULL);
}

int check_timer(time_t* p_timer, unsigned int time_req){
    unsigned int time_diff = (unsigned int)(time(NULL) - *(p_timer));
    return time_diff >= time_req;
}
