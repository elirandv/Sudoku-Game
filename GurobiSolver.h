#ifndef SUDOKUPROJECT_GUROBISOLVER_H
#define SUDOKUPROJECT_GUROBISOLVER_H

#include "Board.h"
#include "Gurobi.h"

typedef enum {
    G_VALIDATE,
    G_GENERATE,
    G_HINT,
    G_GUESS,
    G_GUESS_HINT
}GType;

/*
 * adds each cell Constraints to the gurobi module.
 */
void addCellsConstraint(GRBmodel* model, Board* board, int* ind, double* val);

/*
 * adds each row Constraints to the gurobi module.
 */
void addRowConstraint(GRBmodel* model, Board* board, int* ind, double* val);

/*
 * adds each col Constraints to the gurobi module.
 */
void addColConstraint(GRBmodel *model, Board* board, int* ind, double* val);

/*
 * adds each block Constraints to the gurobi module.
 */
void addBlockConstraint(GRBmodel* model, Board* board, int* ind, double* val);

/*
 * gets only sovable board.
 * for each command GTyoe (G_VALIDATE;G_GUESS;G_GENERATE;G_HINT;G_GUESS_HINT;):
 *      calculate the command solution via gurobi module.
 */
int gurobiSolve(Board* board, GType type , float  threshold, int x, int y);

void updateSalvedBoard(Board* board, double* sol);

void getGuessHintVals(Board* board, double* sol, int x, int y);

int updateGuessSolution(Board* board, double* sol,float threshold);

int* findAboveThreshValuesArrCell(Board* board, double* sol ,int *arrSize, int i, int j, float threshold);

void printIntArr(int *arr,int size);

void cleanGurArr(int *arr, double *arr2, int size);

void PrintSol(int size, double* sol);

#endif







