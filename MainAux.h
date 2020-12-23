#ifndef SUDOKUPROJECT_MAINAUX_H
#define SUDOKUPROJECT_MAINAUX_H

#include <stdio.h>
#include <stdlib.h>
#include "GameFuncs.h"

#define CHAR_SPACE 4

/*
 * Prints a single cell value in board format, for a given cell row and column.
 */
void printCell(Cell *cell, int inEdit);

/*
 * Prints a single row of blocks in board format, for a given row number.
 * Uses the function: printCellValue.
 */
void printRowOfBlocks(Board* board, int row, int inEdit);

/*
 * Prints the board to the user.
 * This command is only available in Edit and Solve modes.
 * other prints for programmer.
 */
void PrintBoard(Game* game);
void PrintTempBoard(Board* board);

/*
 * check if in solve mode and the the board is full, print suitable message.
 */
void checkIfSolved(Game* game);

void printTitle();

#endif
