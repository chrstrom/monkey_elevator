/**
 * @file
 * @brief main linker point of elevator program
 */

#include "utility.h"
#include "queue.h"

// Antar:

    // At cab-knappene kun kan trykkes inn med "Folk" i heisen
    //   




int elevator_init() {
    hardware_command_door_open(0);
    
    if(!at_floor()) {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}

int main(){
    task_t tasks[QUEUE_SIZE];

    int floor_orders_up[4] = {0, 0, 0};     // Holder gjeldende opp-ordre, i rekkefølgen knappene ble trykt inn. Oppdateres med add_floor_orders() i queue
    int floor_orders_down[4] = {0, 0, 0};   // Holder gjeldende ned-ordre, i rekkefølgen knappene ble trykt inn. Oppdateres med add_floor_orders() i queue
    int cab_orders[5] = {0, 0, 0, 0};       // Holder gjeldende cab ordre, i rekkefølgen knappene ble trykt inn. Oppdateres med add_cab_orders() i queue

    int current_floor = -1; //invalid floor to set the elevator's intitial floor-value
    int door_open;

    task_t current_task = {0, 0, CMD_NO_ACTION};
    time_t last_time = time(NULL);
    HardwareMovement current_movement;


    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    error = elevator_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize software\n");
        exit(1);
    }




    while(1){
        if(hardware_read_stop_signal()) {
            elevator_stop();
            erase_queue();
            if(at_floor()){//add a function for this later
                //open doors
            }
            last_time = time(NULL);
            queue_push_front(0, 0, CMD_TIMER_START, tasks);
            continue;
        }

        if(hardware_read_obstruction_signal() && door_open) {
            if(at_floor()){//add a function for this later
            //basic check. Door should never be open between floors
                //open doors
            }
            last_time = time(NULL);
            queue_push_front(0, 0, CMD_TIMER_START, tasks);
            continue;
        }
    
        add_floor_orders(floor_orders_up, floor_orders_down);
        add_cab_orders(cab_orders);
        update_current_task(tasks);

        update_floor_button_lights(floor_orders_up, floor_orders_down);
        update_cab_lights(cab_orders);
        update_floor_lights(current_floor);

        // update next_action in each case
        switch(current_task.next_action) {

            case CMD_ELEVATOR_UP: {

                break;
            }

            case CMD_ELEVATOR_DOWN: {

                break;
            }

            case CMD_ELEVATOR_STOP: { 

                break;
            }

            case CMD_OPEN_DOOR: {

                break;
            }

            case CMD_TIMER_START: {
                if((long int)(last_time - time(NULL)) >= NORMAL_WAIT_TIME) {
                    //should be valid for stop and obstruction
                    //then do the first element in the queue


                }
                break;
            }
        }
    }
}
