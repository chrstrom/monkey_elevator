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

#define MAX_FLOOR 3
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
 * Normal defines. Mostly true/false
 */
#define DOOR_CLOSED 0
#define DOOR_OPEN 1

#define LIGHT_OFF 0
#define LIGHT_ON 1

/** 
 * Enum of the different commands the elevator must handle. 
*/
typedef enum{
    CMD_STOP,
    CMD_TIMER_START,
    CMD_OPEN_DOOR,
    CMD_SET_FLOOR_LIGHT,
    CMD_SET_CAB_LIGHT,
    CMD_ELEVATOR_UP,
    CMD_ELEVATOR_DOWN,
    CMD_ELEVATOR_STOP,
    CMD_NO_ACTION
} CmdType_t;


#endif