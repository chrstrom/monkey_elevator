#ifndef UTILITY_H
#define UTILITY_H

// returns the floor the elevator is at, or 0 if the elevator is between floors
int at_floor();

// pass på å push-back
void add_floor_orders(int* p_up, int* p_down);
void add_cab_orders(int* p_cab);

#endif