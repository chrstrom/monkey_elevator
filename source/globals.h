/**
* @file
* @brief Important includes and definitions that all files require
*/
#ifndef GLOBALS_H
#define GLOBALS_H

/** 
 * Size of the QUEUE. The max amount of concurrent elements in the QUEUE will be at most 2 * (HARDWARE_NUMBER_OF_FLOORS + 1 )  - 2 + HARDWARE_NUMBER_OF_FLOORS+1
 * In this case it will be 6, but we set it to 8 because its a power of 2 (nice) :)
*/

#define QUEUE_SIZE 10
#define ORDER_SIZE 5        /** Size of the orders. One to spare for each floor */

#define MIN_FLOOR 0         /**The bottom floor that the elevator runs to. Note that this is 0-indexed */

#define NORMAL_WAIT_TIME 3  /** Normal wait-time for the elevator, for obstruction and stop timers */

#define DOOR_CLOSE 0        /** Macro for door closed/close door */
#define DOOR_OPEN 1         /** Macro for door open/open door */

#define LIGHT_OFF 0         /** Macro for light off */
#define LIGHT_ON 1          /** Macro for light on */

#define BETWEEN_FLOORS -1
#define INVALID_ORDER -2

#endif //GLOBALS_H