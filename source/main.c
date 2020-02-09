/**
 * @file
 * @brief main linker point of elevator program
 */

#include "data_checking.h"
#include "door_and_light_cont.h"
#include "motor_cont.h"
#include "timer_elevator.h"

int main(){

    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    while(1){
        // Check all external order buttons, stop, and obstruction

        // Switch for the command for the first element in the queue
    }
}
