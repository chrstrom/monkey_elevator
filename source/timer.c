#include <stdio.h>
#include <sys/time.h>

#include "timer.h"


void timer_start(){
    DOOR_TIMER = time(NULL);
}

int timer_check(double time_req){
    return (difftime(time(NULL), DOOR_TIMER) >= time_req);
}