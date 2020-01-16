#ifndef QUEUE_ELEVATOR
#define QUEUE_ELEVATOR

#include <stdlib.h>
#include <stdio.h>

/*
    The composition of the queue-matrix:
    {Priority, floor, Up(enum flag), Down(enum flag), goto_floor}
*/

//Must be changed when we know number of floors in "Sanntidshall"
//Number of rows decided with an enum
#define NUMBER_FLOORS 5 
#define QUEUE_COLS 5

//different defines, to ease the use of the matrix
#define PRIORITY 0
#define CALL_FLOOR 1
#define UP 2
#define DOWN 3
#define TO_FLOOR 4

//define if we allow the program to change the queue for maximum
//effiecency
enum Queue_Swap{
    Not_allow, Allow
};

//Allow the program to swap the priorities in the queue
static swap_state = Queue_Swap::Allow;

#define VEC_LEN(x) (sizeof(x) / sizeof(x)[0])

//The floor-number of the elevator. Will influence the queue of the elevator 
static int floor_state;

//Enum that shows the elevator's current direction
enum Direction{
    stop, down, up
};

//The current direction of the elevator. Will be updated during 
static Direction elevator_state = Direction::stop;

//Return the elevator's current-floor 
void current_floor();

//returns a Matrix of the first matrix. 
//must be generated on intitial startup
//Must be edited later!!
int** generate_initial_queue();

//print the queue for debugging
void print_queue();

//Change the queue. change_queue will be called first, when we only know the 
//direction the elevator will travel in. When the destination is known, 
//change_queue_floor will set the spesific floor the elevator must go to. It 
//will also update the queue
void change_queue_direction(int** queue, int* newFloor, Direction* newDir);
void change_queue_floor(int** queue, int* goToFloor);

//delete the current queue. It has served it's purpose...
//Will be called during change_queue_floor
void delete_first(int** queue);

//Change the order of current queue, depending on the priorities.
//Floors with a high prioity, is the result of the elevator's direction,
//where floors with the same direction (and closest to the elevator) will be 
//prioritized.
//change_order_queue will iterate through the queue and find orders to swap, and 
//then call swap_order
void swap_order(int** queue, int lowPri, int highPri);
void change_order_queue(int** queue);

#endif //QUEUE_ELEVATOR