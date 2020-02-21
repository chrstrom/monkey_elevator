#include <stdio.h>
#include "timer.h"
#include <sys/time.h>

//It is something buggy about the timer. By the time of writing, we cannot identify the bug in the code, but it exists!

void start_timer(){
    DOOR_TIMER = time(NULL);
    //trying with clock instead of time
    //DOOR_TIMER = clock();

    //trying with gettimeofday insted
    //gettimeofday(&DOOR_TIMER, NULL);
}

int check_timer(){
    double normal_wait = 3.0;
    return (difftime(time(NULL), DOOR_TIMER) >= normal_wait);
    //return ((double)((clock() - DOOR_TIMER)/CLOCKS_PER_SEC) >= normal_wait);

    // unsigned int UINT_NORMAL_WAIT_TIME = 3;
    // time_t new_time = time(NULL);
    // return ((unsigned int)(time(NULL) - DOOR_TIMER) > UINT_NORMAL_WAIT_TIME);
}
