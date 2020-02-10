#include "lights.h"

void update_floor_lights(int* p_up, int* p_down) {
    
}

void update_cab_lights(int* p_cab) {

}

void update_floor_button_lights(int floor) {
    if(floor < 0 || floor > 3){
        return;
    }
    hardware_command_floor_indicator_on(floor);
}

void set_stop_light(int stop_light) {
    
}

void set_door_open_light(int door_open){
    
}