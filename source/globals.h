/**
* @file
* @brief Important includes and definitions that all files require
*/
#ifndef GLOBALS_H
#define GLOBALS_H


#define QUEUE_SIZE 16       /** Size of the QUEUE. Mathematical impossible to have more than 10 orders for this elevator, but 16 is a power of 2 :), and it is better to be on the safe side. */

#define MIN_FLOOR 0                 /**The bottom floor that the elevator runs to. Note that this is 0-indexed */

#define DOOR_TIME_REQ 3.0   /** Macro for the standard amount of time, in seconds, that the door should stay open*/
#define DOOR_CLOSE 0        /** Macro for door closed/close door */
#define DOOR_OPEN 1         /** Macro for door open/open door */

#define LIGHT_OFF 0                 /** Macro for light off */
#define LIGHT_ON 1                  /** Macro for light on */

#define BETWEEN_FLOORS -1   /** Macro for the elevator being between floors */
#define FLOOR_NOT_INIT -2   /** Macro for invalid order */


#endif //GLOBALS_H