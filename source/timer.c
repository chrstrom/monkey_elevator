#include <stdio.h>
#include "timer.h"

//It is something buggy about the timer. By the time of writing, we cannot identify the bug in the code, but it exists!

// void reset_timer(){
//     door_timer = time(NULL);
// }

// int check_timer(){
//     return (time(NULL) - door_timer) >= 3;
// }

void start_timer(time_t* p_timer){
    *(p_timer) = time(NULL);
}

int check_timer(time_t* p_timer, unsigned int time_req){
    unsigned int time_diff = (unsigned int)(time(NULL) - *(p_timer));
    return time_diff >= time_req;
}
