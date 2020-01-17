#include "queue_elevator.h"

/*
@function: get current floor
@abstract: set the current floor to floor_state.

@Additional comments: 
Must be changed, when we know how to read/fetch the current floor/position
Have to decide what to do when the elevator is in motion 
*/
void current_floor(){
    floor_state = 1;
    //floor_state = get_current_floor();
}

/*
@function: produce queue to the elevator
@abstract: return a Matrix with the current queue, which will all be zero
during setup of the elevator 

#Additional comments:
Can most make one order from one floor at a time.
Must generate orders and edit the matrix afterwards
*/
matrix_s* generate_initial_queue(){
    // We first allocate memory space for the double pointer
    // that will be the return value. 
    matrix_s* queue = malloc(QUEUE_COLS * NUMBER_FLOORS * sizeof(int) + sizeof(matrix_s));
    queue->cols = QUEUE_COLS;
    queue->rows = NUMBER_FLOORS;
    for(int i = 0; i < NUMBER_FLOORS*QUEUE_COLS; i++) {
        queue->data[i] = 0;
    }
    //If we want to prioritize the queue, it is clever to include the priority-variable
    for(int fl = 0; fl < NUMBER_FLOORS; fl++){
        queue->data[fl * QUEUE_COLS + PRIORITY] = fl + 1;
    }
    return queue;
}

/*
@function: produce empty matrix
@abstract: return an empty matrix

#Additional comments:
*/
matrix_s* create_empty_matrix(int rows, int cols) {
     // Allocate memory for a matrix of size rows x cols
    matrix_s* m = malloc(rows * cols * sizeof(int) + sizeof(matrix_s));
     // Set member variables of the matrix
    m->rows = rows;
    m->cols = cols;
    for(int i = 0; i < rows*cols; i++) {
        m->data[i] = 0;
    }
    return m;
}

/*
Disbanded code:
Went from a int** to matrix_s

 for (int i  = 0; i < NUMBER_FLOORS; i++) {
        queue[i] = (int*) calloc(QUEUE_COLS , sizeof(int));
        queue[i][PRIORITY] = i + 1; //shows the number of the floor in the matrix
    }
*/

/*
@function: print out the queue
@abstract: 

#Additional comments:

Disbanded code:
void print_queue(int** queue){
    for(int fl = 0; fl < NUMBER_FLOORS; fl++){
        for(int col = 0; col < QUEUE_COLS; col++){
         printf("%d ", queue[fl][col]);
            printf("\t");
        }
        printf("\n");
    }
}
*/
void print_queue(const matrix_s* queue) {
    for(int i = 0; i < queue->rows; i++) {
        for(int j = 0; j < queue->cols; j++) {
            printf("%d ", queue->data[i*queue->cols + j]);  // [i*m + j] is the equivalent of element [i][j], think "base + offset"
            printf(" ");
        }
        printf("\n");
    }
}

/*
@function: set the queue to the next direction
@abstract: will tell the elevator to go to a spesific floor, and also knows 
in what direction the elevator will leave afterwards. This makes it possible to 
optimize the elevator, making sure it is flexible in the order of floor-stops

#Additional comments:
Will try to delete the first in queue, however will not to that if that is not fulfilled
Here it is required to check the number of elements in the queue
and later set a spesific direction to a spesific floor. Have to take care of the problem if the 
queue is full. Throw error?

#Known problems:
"Direction is undefined"
----------------------Not finished - unsure how to move on (AKA Not sure how to use the FSM to solve this) ----------------
*/
void change_queue_direction(matrix_s* queue, int* newFloor, Direction* newDir){

    switch (newDir)
    {
    case Direction::up:
        delete_first();
        queue->data[]
        break;
    case Direction::down:
        break;
    default:
        break;
    }
}

/*
@function: set the destination of a spesific order
@abstract: Will be invoked when we get the next order during a stop. The function will 
first check if the elevator has stopped at the correct/set the next floor it will stop on. 

#Additional comments:
Should only be called when the elevator has reached the correct address

#Known problems:
*/
void change_queue_floor(matrix_s* queue, int* goToFloor){
    matrix_s* first_pri_matrix = get_current_priority(queue);
    //check if we arrived at the correct floor. If yes, set the next floor
    if(elevator_state == stop && first_pri_matrix->data[CALL_FLOOR] == floor_state){
        queue->data[TO_FLOOR] = *(goToFloor);
    }
    else{
        //The function should only be envoked when at the correct floor
        //Should throw an error?
    }
}

/*
@function: get the next priority
@abstract: check if the first priority is achieved (i.e. the elevator has stopped on the wanted floor)
if yes, then move the columns in the matrix to the left, and set the rightmost column to 0.
Therefore making it possible to recieve new orders

#Additional comments:
Will try to delete the first in queue, however will not to that if that is not fulfilled.

#Improvments:
Should try to use the number of orders that we have at each time. This will require us to 
keep updating that variable, and thus reducing the runtime of this exact function. However, it
will have a greater risk of error.

#Known problems:
"Direction is undefined"

#Deleted code:
if(elevator_state == stop && queue[PRIORITY][CALL_FLOOR] == current_floor){
        //The elevator has arrived to the destination, and can "reduce" the Matrix
        for(int fl = 0; fl < NUMBER_FLOORS; fl++){
            for(int col = 0; col < QUEUE_COLS; col++){
                if(col != QUEUE_COLS - 1){
                    //moving the columns one step to the left in the matrix
                    queue[fl][col] = queue[fl][col + 1];
                }
                else{
                    //filling the "new" column with 0. Can now get new data
                    queue[fl][col] = 0;
                }
            }
        }
    }
*/
void delete_first(matrix_s* queue){
    //first check if the destination is correct
    matrix_s* current_priority_matrix = get_current_priority(queue);
    current_floor();
    if(num_orders > 0 && elevator_state == stop && current_priority_matrix->data[TO_FLOOR] == floor_state){
        //If true, we have arrived at this destination, and we can delete the first entry in the queue
        //Changing the queue-order
        for(int fl = 0; fl < NUMBER_FLOORS - 1; fl++){
            for(int col = 0; col < QUEUE_COLS; col++){
                queue->data[fl * col + col] = queue->data[(fl + 1) * col + col];
            }
        }
    }
}

/*
@function: swap between orders with different priorities
@abstract: 

#Additional comments:

Discarded changes:
for(int col = 0; col < QUEUE_COLS; col++){
        temp = queue[highPri][col];
        queue[highPri][col] = queue[lowPri][col];
        queue[lowPri][col] = temp;
    }
*/
void swap_order(matrix_s* queue, int lowPri, int highPri){
    matrix_s* temp_matrix = generate_initial_queue();
    int temp;
    for(int col = 1; col < QUEUE_COLS; col++){
        temp = queue->data[(lowPri - 1) * col + col];
        queue->data[(lowPri - 1) * col + col] = queue->data[(highPri - 1) * col + col];
        queue->data[(highPri - 1) * col + col] = temp;
    }
}

/*
@function: iterate over the queue, check each priority and then swap them
@abstract: each call to the queue has it's own priority. If it is in the same direction 
as the elevator and/or is close to the elevators position, it will have a high priority
in the queue. This function will try to iterate through the orders and 

#Additional comments:
Will try to delete the first in queue, however will not to that if that is not fulfilled

#Known problems:
"Queue_Swap is undefined"
*/
void change_order_queue(matrix_s* queue){
    if(swap_state == Allow){
        //Need to find a good algorithm to sort this
        //We need womething to have good effieciency (right??) 
        //Base this on FSM, where the direction of the elevator decides the next stop/
        //queue order
    }
}

/*
@function: Check if the queue is full
@abstract: check the last priority. If all entries are 0, then the queue has space.

#Additional comments:

#Known problems:
*/
int check_space_queue(matrix_s* queue){
    //we know that col == 0 will show the priority of the order, and thus
    //not be 0
    for(int col = 1; col < QUEUE_COLS; col++){
        if(queue->data[(NUMBER_FLOORS - 1) * col] == 1){
            return 0;
        }
    }
    return 1;
}

/*
@function: Get the current top priority in the queue
@abstract: return a matrix of the queue's top priority. 

#Additional comments:

#Known problems:
*/
matrix_s* get_current_priority(matrix_s* queue){
    int top_pri = 1;
    //create a temporary matrix_s*
    matrix_s* temp = create_empty_matrix(top_pri, QUEUE_COLS);
    for(int col = 0; col < QUEUE_COLS; col++){
        //fill the matrix with the top data
        temp->data[col] = queue->data[col];
    }
    return temp;
}