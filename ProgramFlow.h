#ifndef SUDOKUPROJECT_PROGRAMFLOW_H
#define SUDOKUPROJECT_PROGRAMFLOW_H

#include "MainAux.h"
#include <stdio.h>
#define MAX_STRING 1024

/*
 * main game function, manage the sudoku game program flow.
*/
int PlaySudoku();

/*
 * Execute Commands.
*/
void ExecuteCommand(Game *game, Command* command);

#endif
