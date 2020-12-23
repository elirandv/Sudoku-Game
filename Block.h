#ifndef SUDOKUPROJECT_BLOCK_H
#define SUDOKUPROJECT_BLOCK_H

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "Cell.h"

/**
 * Block module:
 *
 * Block Structure represents a single block in the board.
 * Block field:
 * chooseCellInBlock - matrix of cells (size:BLOCK_ROW_NUMBER*BLOCK_COLUMN_NUMBER).
 */
typedef struct boardBlocks{
    Cell ***chooseCellInBlock;
} Block;

/*
 * Creates new block and init the chooseCellInBlock to matrix of new cells
 * (of size:n*m) n cols, m rows.
 * Returns pointer to it.
 */
Block* newBlock(int n, int m);

/*
 * Returns the value of cell i,j in the given block.
 */
int getCellValueFromBlock(int i, int j, Block *block);

/*
 * Frees block's memory.
 */
void deleteBlock(Block* block,int n, int m);


#endif
