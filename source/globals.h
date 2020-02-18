/**
* @file
* @brief Important includes and definitions that all files require
*/
#ifndef GLOBALS_H
#define GLOBALS_H


#define QUEUE_SIZE 16               /** Size of the queue. It is not necessary to have it at 16, but it is a power of 2, and it is more than good enough for an elevator with 4 floors*/

#define ORDER_SIZE 5                /** Size of the orders. One to spare for each floor */

#define MIN_FLOOR 0                 /**The bottom floor that the elevator runs to. Note that this is 0-indexed */

#define NORMAL_WAIT_TIME 3          /** Normal wait-time for the elevator, for obstruction and stop timers */

#define DOOR_CLOSE 0                /** Macro for door closed/close door */
#define DOOR_OPEN 1                 /** Macro for door open/open door */

#define LIGHT_OFF 0                 /** Macro for light off */
#define LIGHT_ON 1                  /** Macro for light on */

#define NOT_CHECK_DOOR_TIME 0       /** Macro to not check the timer */
#define CHECK_DOOR_TIME 1           /** Macro for check timer */

#endif //GLOBALS_H