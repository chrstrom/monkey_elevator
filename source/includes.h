/**
* @file
* @brief Important includes and definitions that all files require
*/
#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "driver/hardware.h"

/**
 * The top floor that the elevator runs to. Note that this is 0-indexed 
 */
#define MAX_FLOOR 3

/**
 * The bottom floor that the elevator runs to. Note that this is 0-indexed 
 */
#define MIN_FLOOR 0

/** 
 * Size of the queue. The max amount of concurrent elements in the queue will be at most 2 * ( MAX_FLOOR + 1 )  - 2
 * In this case it will be 6, but we set it to 8 because its a power of 2 (nice) :)
*/
#define QUEUE_SIZE 8

/** 
 * Size of the orders. One to spare for each floor
*/
#define ORDER_SIZE 5

/** 
 * Normal wait-time for the elevator.
 * Will be used everytime after obstruction and stop 
*/
#define NORMAL_WAIT_TIME 3

/**
 * Macro for closed door
 */
#define DOOR_CLOSED 0

/**
 * Macro for open door
 */
#define DOOR_OPEN 1

/**
 * Macro for light off 
 */
#define LIGHT_OFF 0
/**
 * Macro for light on
 */
#define LIGHT_ON 1


#endif