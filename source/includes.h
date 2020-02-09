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

#define MAX_FLOOR 4
#define MIN_FLOOR 1

#define NORMAL_WAIT_TIME 3


typedef enum{
    CMD_STOP,
    CMD_OPEN_DOOR,
    CMD_READ_OBST,
    CMD_READ_CURRENT_FLOOR,
    CMD_READ_FLOOR_BUTTON,
    CMD_SET_FLOOR_LIGHT,
    CMD_SET_CAB_LIGHT
} CmdType_t;





#endif