#include "Block.h"

Block* newBlock(int n, int m){
    int i,j;
    Block* block = (Block*) malloc(sizeof(Block));
    if(block==NULL){
        printf("Error: newBlock has failed\n");
        exit(0);
    }
    block->chooseCellInBlock = (Cell***) malloc( n * sizeof(Cell**));
    if(block->chooseCellInBlock==NULL){
        printf("Error: newBlock has failed\n");
        exit(0);
    }
    for(i=0; i<n; i++){
        block->chooseCellInBlock[i] = (Cell**) malloc(m*sizeof(Cell*));
        if(block->chooseCellInBlock[i]==NULL){
            printf("Error: newBlock has failed\n");
            exit(0);
        }
        for(j=0; j<m; j++){
            block->chooseCellInBlock[i][j] = newCell();
        }
    }
    return block;
}

int getCellValueFromBlock(int i, int j, Block *block){
    return getValueCell(block->chooseCellInBlock[i][j]);
}

void deleteBlock(Block* block,int n, int m){
    int i;
    (void) m;
    if(!block){
        return;
    }
    for(i=0; i<n; i++){
        free(block->chooseCellInBlock[i]);
    }
    free(block->chooseCellInBlock);
    free(block);
}