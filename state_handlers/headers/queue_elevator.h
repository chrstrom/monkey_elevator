#ifndef QUEUE_ELEVATOR
#define QUEUE_ELEVATOR

#include <stdlib.h>
#include <stdio.h>

/*
    The composition of the queue-matrix:
    {Priority, floor, Up(enum flag), Down(enum flag), goto_floor}
*/

#define VEC_LEN(x) (sizeof(x) / sizeof(x)[0])

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

/*!
 * @struct       matrix_s
 * @abstract:    Contains the main properties of a matrix
 * @discussion:  data will contain rows*cols elements, and makes up the values
 * in the matrix. Note that the data is stored in a 1D array, so any subsequent
 * operations on one single element in a matrix where rows && cols > 1 (2D matrix)
 * must be done with caution to ensure the correct indexation.
 * Example: mat[i][j] ==> [i*mat->cols + j]
 *  Note also that by specfying rows = 1, you are practically making a 1-d vector.
 * @field:       rows    The amount of rows in the matrix
 * @field:       cols    The amount of columns in the matrix
 * @field:       data[]  The data contained by the matrix. Is rows*cols long.
*/
typedef struct {
    int rows;
    int cols;
    int data[];
} matrix_s;


/*!
 *@enum:        Bool
 *@abstract:    Show either true or false of a function
 *@discussion:  Easier to read a code. The intention is more obvious
*/
enum Bool {
    false, true
};

//Allow the program to swap the priorities in the queue
static enum Queue_Swap swap_state = Allow;

//The floor-number of the elevator. Will influence the queue of the elevator 
static int floor_state;

//The number of orders at a spesific time
static int num_orders;

//Enum that shows the elevator's current direction
enum Direction{
    stop, down, up
};

//The current direction of the elevator. Will be updated during 
static enum Direction elevator_state = stop;

//Return the elevator's current-floor 
void current_floor();

//returns a Matrix of the first matrix. 
//must be generated on intitial startup
//Must be edited later!!
matrix_s* generate_initial_queue();

//create an empty matrix
matrix_s* create_empty_matrix(int rows, int cols);

//print the queue for debugging
void print_queue();

//Change the queue. change_queue will be called first, when we only know the 
//direction the elevator will travel in. When the destination is known, 
//change_queue_floor will set the spesific floor the elevator must go to. It 
//will also update the queue
void change_queue_direction(matrix_s* queue, int* newFloor, enum Direction* newDir);
void change_queue_floor(matrix_s* queue, int* goToFloor);

//delete the current queue. It has served it's purpose...
//Will be called during change_queue_floor
void delete_first(matrix_s* queue);

//Change the order of current queue, depending on the priorities.
//Floors with a high prioity, is the result of the elevator's direction,
//where floors with the same direction (and closest to the elevator) will be 
//prioritized.
//change_order_queue will iterate through the queue and find orders to swap, and 
//then call swap_order
void swap_order(matrix_s* queue, int lowPri, int highPri);
void change_order_queue(matrix_s* queue);

//Check if the queue has space left. If yes, return 1. Else, return 0
int check_space_queue(matrix_s* queue);

//Get the matrix of the current priority
matrix_s* get_current_priority(matrix_s* queue);

#endif //QUEUE_ELEVATOR