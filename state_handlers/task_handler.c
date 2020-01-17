#include "headers/task_handler.h"

// Potentially need to handle this in a different way
// As long as the number here correspond to the location in the vector
// that will be the result of the mask/conditional matrix operations
#define FLOOR_1_TASK_IDX 0
#define FLOOR_2_TASK_IDX 1
#define STOP_TASK_IDX 5
/*!
 * @function: handle_tasks
 * @abstract: handle the current tasks that potentially need to be executed
 * @param:  tasks   the matrix (or vector, really) that holds a 1/0 for task/not task
*/
// void task_handler(matrix_s* tasks) {
//     // STOP is the most important task to deal with; because of this, it should be
//     // dealt with first
//     // The else-ifs should be other events that we need to handle before others
//     // else will take care of the different floors, and start moving the elevator
//     if(tasks->data[STOP_TASK_IDX]) {
//         //stop_elevator();
//         return;
//     }
//     else {
//         // Handle floor switching:
//         // This is where the queue system is introduced, so this whole
//         // Area might get really messy

//     }
// }


// Converting data is done in two steps:
// 1) columnwise AND by matrix_AND_data
// 2) columwise COMP by matrix_COMP_matri

matrix_s* to_task_const(const matrix_s* data, const matrix_s* mask, const matrix_s* cond) {
    matrix_AND_data(mask, data);
    matrix_s* task = matrix_COMP_matrix_const(cond, data);

    return task;
}