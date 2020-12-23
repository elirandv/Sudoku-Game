#ifndef SUDOKUPROJECT_FILE_H
#define SUDOKUPROJECT_FILE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "Board.h"
#include "GameFuncs.h"

/*
 * Returns pointer to a new board created from file.
 * doesnt maintain errorFlag and error for each cell.
 */
Board *loadBoardFromFile(char* path, int inEdit);

/*
 * saves the given board to a file.
 */
void saveBoardToFile(char* path, Board* board, int inEdit);

#endif