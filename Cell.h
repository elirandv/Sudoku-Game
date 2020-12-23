#ifndef SUDOKUPROJECT_CELL_H
#define SUDOKUPROJECT_CELL_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/**
 * Cell module:
 *
 * Cell Structure represents a single cell in board.
 * Cell's 2 bool (int) and one int fields:
 * value - holds current's cell value.
 * fixed - cell has fixed answer.
 * valid - marks if cell value updated during validation process.
 * error - marks if there is a cell in same row or column or block with same value
 */
typedef struct struct_cell{
    int value;
    int fixed;
    int valid;
    int error;
}Cell;

/*
 * Creates new cell and init the cell's fields to 0, returns a pointer to it.
 */
Cell* newCell();

/*
 * Frees cell's memory .
 */
void deleteCell(Cell* cell);

/*
 * Sets cell value.
 */
void setValueCell(Cell *cell, int value);

/*
 * Sets cell value & fixed to 1.
 */
void setFixedCellValue(Cell* cell, int value);

/*
 * Sets cell value & valid to 1.
 */
void setValidValue(Cell *cell, int value);

/*
 * Sets valid to 0.
 */
void unValidCell (Cell *cell);

/*
 * Returns cell value field.
 */
int getValueCell(Cell *cell);

/*
 * Returns cell fixed field.
 */
int isFixedCell(Cell *cell);

/*
 * Returns cell valid field.
 */
int isCellValid(Cell *cell);

/*
 * Set cell as erroneous.
 */
void setCellErroneous(Cell *cell, int isError);

/*
 * Returns whether cell is erroneous.
 */
int isCellErroneous(Cell *cell);

#endif

