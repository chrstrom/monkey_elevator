#include <stdio.h>
#include <sys/time.h>

#include "timer.h"

void start_timer(){
    DOOR_TIMER = time(NULL);
}

int check_timer(double time_req){
    return (difftime(time(NULL), DOOR_TIMER) >= time_req);
}
