#ifndef SUDOKUPROJECT_BTSOLVER_H
#define SUDOKUPROJECT_BTSOLVER_H

#include "Bstack.h"
#include "Board.h"

/*
 * Deletes the i-th element in the array by
 * shifting left all values in indexes [i+1,len-1].
 * Returns the new array length: len-1.
 */
int movePartToLeft(int array[], int length, int i);


/*
 * Solves the board using deterministic recursion method, starts from a giving indexes going from from left to right, then top to bottom.
 * For each empty cell, loops over all possible values.
 * In each loop, sets the cell to a valid value, set the valid member to 1 and continues to the next empty cell.
 * If no valid value is found, empties the cell, set the valid member to 0 and backtrack to the previous validated cell.
 * Returns 1 if solution is found, else 0.
 */
int detBTSolveBoard(Board *copyBoard);

#endif
