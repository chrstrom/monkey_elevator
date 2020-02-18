#include <stdio.h>
#include "timer.h"

//It is something buggy about the timer. By the time of writing, we cannot identify the bug in the code, but it exists!

void start_timer(){
    DOOR_TIMER = time(NULL);
}

int check_timer(int time_req){
    return ((unsigned int)(time(NULL) - DOOR_TIMER) > time_req);
}
