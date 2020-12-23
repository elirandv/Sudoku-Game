#ifndef SUDOKUPROJECT_BOARD_H
#define SUDOKUPROJECT_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "Block.h"

#define DEFULT_COLS 3
#define DEFULT_ROWS 3


/**
* Board module:
*
* Board Structure represents a full size soduko board.
* Board fields:
* bCol - block cols number.
* bRow - block rows number.
* chooseBlockInBoard - matrix of Blocks (of size:BLOCK_COLUMN_NUMBER*BLOCK_ROW_NUMBER).
* numberOfEmptyCells - the number of empty cells left in the board.
* foundSolution - points to the last found solution.
* fixErrMatrix - indicator matrix for fix/error cells.
* errorFlag - number of error on the board.
*/
typedef struct sodukoBoards {
    int bCol;
    int bRow;
    Block ***chooseBlockInBoard;
    int numberOfEmptyCells;
    int **foundSolution;
    int errorFlag;
} Board;

/*
 * Creates new board and init the chooseBlockInBoard to matrix of new Blocks
 * (of size:BLOCK_COLUMN_NUMBER*BLOCK_ROW_NUMBER)
 * Returns pointer to it.
 */
Board *newBoard(int n, int m);

/*
 * Free's movesList and board's memory.
 */
void destroyBoard(Board *board);

/*
 * one dimention size.
 */
int size(Board *board);

/*
 * Creates new defult board (9*9).
 * Returns pointer to it.
 */
Board *newDefultBoard();

/*
 * Returns pointer to the block in which cell i,j is at.
 */
Block *getCellsBlock(int i, int j, Board *board);

/*
 * Returns pointer to cell i,j in board.
 */
Cell *getCell(int i, int j, Board *board);

/*
 * Update iNext and jNext values to the next cell index after iCurrent and jCurrent.
 * Going right->left, up->down (Deterministic step).
 * Returns 1 if there's a next cell, else returns 0.
 */
int getNextCellIndexes(int iCurrent, int jCurrent, int *iNext, int *jNext ,int bRow,  int bCol);


/*
 * Update iNext and jNext values to the next empty cell index after index of iCurrent and jCurrent.
 * Going right->left, up->down (Deterministic step)
 * Returns 1 if there's a next cell, else returns 0.
 */
int getNextEmptyCell(int iCurrent, int jCurrent, int *iNext, int *jNext, Board *board);

/*
 * Returns the board to its original state before validation.
 * Walks over the board and un-validate all validated cells.
 */
void unValidBoard(Board *board);

/*
 * Copies inBoard cell's values to outBoard.
 */
void copyBoardValues(Board *inBoard, Board *outBoard);
void copyChooseBlockInBoard(Board *inBoard, Board *outBoard);
void copyFoundSolution(int **inFoundSolution, int **outFoundSolution, int size );

/*
 * Decreases numberOfEmptyCells by 1.
 */
void decNumberOfEmptyCells(Board *board);

/*
 * Increases numberOfEmptyCells by 1.
 */
void incNumberOfEmptyCells(Board *board);

/*
 * get Number Of Empty Cells.
 */
int getNumberOfEmptyCells(Board *board);

/*
 * Finds and fags each erroneous call in the board.
 */
int findErrorsInBoard(Board *board);

/*
 * Checks if value is different from all cell's neighbors.
 * Returns 1 if it's different, else 0;
 */
int checkCellNoErroneous(int i, int j, int value, Board *board, int boardOrSol);

/*
 * Checks if value is different from all values in row i in board.
 * Returns 1 if it's different, else 0;
 */
int ValidRow(int i, int j ,int value, Board *board, int boardOrSol);

/*
 * Checks if value is different from all values in column j in board.
 * Returns 1 if it's different, else 0;
 */
int ValidColumn(int i ,int j, int value, Board *board, int boardOrSol);

/*
 * Checks if value is different from all values in cell's i,j block .
 * Returns 1 if it's different, else 0;
 */
int ValidBlock(int i, int j, int value, Board *board, int boardOrSol);

/*
 * updates cell value. and all suitable changes at the board struct.
 */
void updateValueCell(Board *board, Cell *cell, int value);

/*
 * updates Fixed cell value. and all suitable changes at the board struct.
 */
void updateValueFixedCell(Board *board, Cell *cell, int value);

/*
* check how many "obvious" values have to cell[i][j].
* if he has one- return this value.
*/
int cellHasSingleLegalVal(int i,int j,Board *board);

/*
 * Checks if value is different from all values in cell's i,j block .
 * Returns 1 if it's different, else 0;
 */
int findLegalCellValues(int i, int j, int validVal[] ,Board *solvedBoard);

/*
 * Checks if value is different from all values in cell's i,j block .
 * Returns 1 if it's different, else 0;
 */
int* findLegalValuesArrCell(Board *board ,int *legalValsSize, int *row, int *column);

/*
 * randomly chooses empty cell in the board.
 * NumOfEmptyCells!=0!!
 */
Cell* randomEmptyCell(Board *board, int *row, int *column);

void PrintFoundSolution(Board* board);

#endif
