#include "Cell.h"

Cell* newCell(){
    Cell* cell = (Cell*) malloc(sizeof(Cell));
    if(cell==NULL){
        printf("Error: newCell has failed\n");
        exit(0);
    }
    cell->value=0;
    cell->fixed=0;
    cell->valid=0;
    cell->error=0;

    return cell;
}

void deleteCell(Cell* cell){
    if(!cell){
        return;
    }
   free(cell);
}

void setValueCell(Cell *cell, int value){
    cell->value=value;
}

void setFixedCellValue(Cell* cell, int value){
    setValueCell(cell, value);
    cell->fixed=1;
}

void setValidValue(Cell *cell, int value){
    cell->valid = 1;
    cell->value = value;
}

int getValueCell(Cell *cell){
    return cell->value;
}

int isFixedCell(Cell *cell){
    return cell->fixed;
}

int isCellValid(Cell *cell){
    return cell->valid;
}

void unValidCell(Cell *cell){
    cell->value = 0;
    cell->valid = 0;
    cell->error=0;
}

void setCellErroneous(Cell *cell, int isError) {
    cell->error = isError;
}

int isCellErroneous(Cell *cell) {
    return cell->error;
}

