/**
* @file
* @brief IO library for elevator, spanning set/get -functions for buttons and lights
*/

#ifndef ELEVATOR_IO_H
#define ELEVATOR_IO_H

#include "includes.h"
#include "queue.h"


// Mye overlap med utdelt kode!!!!! Mest sannsynlig å fjerne set-funksjoner, men å beholde update/get

/**
 * @brief Find what floor the elevator is at
 * 
 * @return 0 if the elevator is between floors, and an int corresponding to the current floor
 * if the elevator is at a floor
 */
int at_floor() {

}


/**
 * @brief Poll all floor buttons
 * 
 * @return An @c Order object with the first floor button that was detected to be pressed
 * as its @c floor_at value.
 * 
*/
Order poll_floor_buttons() {

}

/**
 * 
 */
void update_cab_buttons(Order* current_order) {

}



#endif

