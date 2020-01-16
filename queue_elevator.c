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
int** generate_initial_queue(){
    // We first allocate memory space for the double pointer
    // that will be the return value. 
    int** queue = (int**) calloc(NUMBER_FLOORS, sizeof(int*));
    for (int i  = 0; i < NUMBER_FLOORS; i++) {
        queue[i] = (int*) calloc(QUEUE_COLS , sizeof(int));
        queue[i][PRIORITY] = i + 1; //shows the number of the floor in the matrix
    }
    return queue;
}

/*
@function: print out the queue
@abstract: 

#Additional comments:
*/
void print_queue(int** queue){
    for(int fl = 0; fl < NUMBER_FLOORS; fl++){
        for(int col = 0; col < QUEUE_COLS; col++){
         printf("%d ", queue[fl][col]);
            printf("\t");
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

#Known problems:
"Direction is undefined"
Not finished - unsure how to move on
*/
void change_queue_direction(int** queue, int* newFloor, Direction* newDir){

    switch (newDir)
    {
    case Direction::up:
        delete_first();
        queue[*(newFloor)][];
        break;
    case Direction::down:
        break;
    default:
        break;
    }
}

void change_queue_floor(int** queue, int* goToFloor){

}

/*
@function: get the next priority
@abstract: check if the first priority is achieved (i.e. the elevator has stopped on the wanted floor)
if yes, then move the columns in the matrix to the left, and set the rightmost column to 0.
Therefore making it possible to recieve new orders

#Additional comments:
Will try to delete the first in queue, however will not to that if that is not fulfilled

#Known problems:
"Direction is undefined"
*/
void delete_first(int** queue){
    //first check if the destination is correct
    if(elevator_state == Direction::stop && queue[PRIORITY][CALL_FLOOR] == current_floor){
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
}

/*
@function: swap between orders with different priorities
@abstract: 

#Additional comments:
*/
void swap_order(int** queue, int lowPri, int highPri){
    int temp;
    for(int col = 0; col < QUEUE_COLS; col++){
        temp = queue[highPri][col];
        queue[highPri][col] = queue[lowPri][col];
        queue[lowPri][col] = temp;
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
void change_order_queue(int** queue){
    if(swap_state == Queue_Swap::Allow){
        //Need to find a good algorithm to sort this
        //We need womething to have good effieciency (right??) 
    }
}
