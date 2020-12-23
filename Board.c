#include "Board.h"

Board *newBoard(int bRow,  int bCol) {
    int i, j;
    Board *board = (Board*) malloc(sizeof(Board));
    if (board == NULL){
        printf("Error: newBoard malloc has failed\n");
        exit(0);
    }
    board->chooseBlockInBoard = (Block***) malloc( bCol*sizeof(Block**));
    if(board->chooseBlockInBoard==NULL){
        printf("Error: newBoard malloc has failed\n");
        exit(0);
    }
    for(i=0; i<bCol; i++){
        board->chooseBlockInBoard[i] = (Block**) malloc( bRow*sizeof(Block*));
        if(board->chooseBlockInBoard[i]==NULL){
            printf("Error: newBoard malloc has failed\n");
            exit(0);
        }
        for(j=0; j<bRow; j++){
            board->chooseBlockInBoard[i][j] = newBlock(bRow, bCol);
        }
    }
    board->bCol=bCol;
    board->bRow=bRow;
    board->numberOfEmptyCells=bCol*bCol*bRow*bRow;
    board->errorFlag=0;

    board->foundSolution = (int**) malloc( bCol*bRow*sizeof(int*));
    if(board->foundSolution==NULL){
        printf("Error: newBoard malloc has failed\n");
        exit(0);
    }
    for(j=0; j<bCol*bRow; j++){
        board->foundSolution[j] = (int*) calloc((size_t ) bCol*bRow,sizeof(int));
        if(board->foundSolution[j]==NULL){
            printf("Error: newBoard malloc has failed\n");
            exit(0);
        }
        for(i=0; i<bCol*bRow; i++){
            board->foundSolution[j][i] = 0;
        }
    }
    return board;
}

void destroyBoard(Board *board) {
    int i, j;
    if (!board) {
        return;
    }

    for( i= 0 ; i< (board->bCol*board->bRow);   i++){
        for( j= 0 ; j< (board->bCol*board->bRow);   j++){
            deleteCell(getCell(i,j,board));
        }
    }


    for (i = 0; i < board->bCol; i++) {
        for (j = 0; j < board->bRow; j++) {
            deleteBlock(board->chooseBlockInBoard[i][j], board->bRow, board->bCol);
        }
        free(board->chooseBlockInBoard[i]);
    }

    free(board->chooseBlockInBoard);
    if (board->foundSolution != NULL) {
        for (j = 0; j < board->bRow * board->bCol; j++) {
            free(board->foundSolution[j]);
        }
            free(board->foundSolution);
    }
   free(board);
}

int size(Board *board){
    return board->bCol * board->bRow;
}

Board *newDefultBoard(){
    return newBoard(DEFULT_ROWS ,DEFULT_COLS);
}

Block *getCellsBlock(int i, int j, Board *board) {
    return board->chooseBlockInBoard[i / board->bRow][j / board->bCol];
}

Cell *getCell(int i, int j, Board *board) {
    return getCellsBlock(i, j, board)->chooseCellInBlock[i % board->bRow][j % board->bCol];
}

int getNextCellIndexes(int iCurrent, int jCurrent, int *iNext, int *jNext,int bRow,  int bCol) {
    if (jCurrent == (bRow*bCol - 1)){
        *jNext = 0;
        *iNext = iCurrent + 1;
    } else {
        *jNext = jCurrent + 1;
        *iNext = iCurrent;
    }
    if (*iNext >= bRow*bCol){
        return 0;
    }
    return 1;
}

int getNextEmptyCell(int iCurrent, int jCurrent, int *iNext, int *jNext, Board *board) {
    Cell *cell;
    if (!getNextCellIndexes(iCurrent, jCurrent, iNext, jNext, board->bRow, board->bCol)){
        return 0;
    }
    do {
        cell = getCell(*iNext, *jNext, board);
        if (getValueCell(cell) == 0){
            return 1;
        }
    } while (getNextCellIndexes(*iNext, *jNext, iNext, jNext, board->bRow, board->bCol));
    return 0;
}

void unValidBoard(Board *board) {
    int i, j;
    Cell *cell = NULL;
    for (i = 0; i <  (board->bRow*board->bCol); i++) {
        for (j = 0; j < (board->bRow*board->bCol); j++) {
            cell = getCell(i, j, board);
            if (isCellValid(cell)){
                unValidCell(cell);
            }
        }
    }
}

void copyBoardValues(Board *inBoard, Board *outBoard) {
    outBoard->bRow= inBoard->bRow;
    outBoard->bCol= inBoard->bCol;
    outBoard->numberOfEmptyCells= inBoard->numberOfEmptyCells;
    outBoard->errorFlag= inBoard->errorFlag;
    copyChooseBlockInBoard(inBoard,outBoard);
    copyFoundSolution(inBoard->foundSolution, outBoard->foundSolution,inBoard->bRow*inBoard->bCol);
}

void copyChooseBlockInBoard(Board *inBoard, Board *outBoard) {
    int i, j;
    Cell *inCell = NULL, *outCell = NULL;
    for (i = 0; i < (inBoard->bRow*inBoard->bCol); i++) {
        for (j = 0; j < (inBoard->bRow*inBoard->bCol); j++) {
            inCell = getCell(i, j, inBoard);
            outCell = getCell(i, j, outBoard);
            outCell->error=isCellErroneous(inCell);
            setValueCell(outCell, getValueCell(inCell));
        }
    }
}

void copyFoundSolution(int **inFoundSolution, int **outFoundSolution, int size ) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            outFoundSolution[i][j] = inFoundSolution[i][j] ;
        }
    }
}

void decNumberOfEmptyCells(Board *board){
    board->numberOfEmptyCells--;
}

void incNumberOfEmptyCells(Board *board){
    board->numberOfEmptyCells++;
}

int getNumberOfEmptyCells(Board *board){
    return (board->numberOfEmptyCells);
}

int findErrorsInBoard(Board *board){
    Cell *cell = NULL;
    int i,j,size;
    board->errorFlag=0;
    size=board->bCol*board->bRow;

    for (j=0; j<size; j++){
        for (i=0; i<size; i++){
            cell = getCell(i,j,board);
            if (isFixedCell( cell) || getValueCell(cell)==0){
                cell->error =0;
                continue;
            }
            else if ( 1==checkCellNoErroneous(i,j,getValueCell(cell),board,0)){
                if(cell->error==1){
                    cell->error =0;
                }
            }
            else{
                board->errorFlag+=1;
                if(cell->error==0){
                    cell->error=1;
                }
            }
        }
    }
    return board->errorFlag;
}


int checkCellNoErroneous(int i, int j, int value, Board *board, int boardOrSol) {
    if (ValidRow(i, j, value, board, boardOrSol) && ValidColumn(i, j, value, board, boardOrSol)
        && ValidBlock(i, j, value, board, boardOrSol)){
        return 1;}
    else {
        return 0;
    }
}

int ValidRow(int i, int j ,int value, Board *board, int boardOrSol) {
    int k;
    for (k = 0; k < (board->bRow*board->bCol); k++) {
        if(boardOrSol==0) {
            if (j != k && value == getValueCell(getCell(i, k, board)))
                return 0;
        }
        else if (boardOrSol==1){
            if (j != k && value==board->foundSolution[i][k]){
                return 0;
            }
        }
    }
    return 1;
}

int ValidColumn(int i ,int j, int value, Board *board, int boardOrSol){
    int k;
    for (k = 0; k < (board->bRow*board->bCol); k++) {
        if(boardOrSol==0) {
            if (i != k && value == getValueCell(getCell(k, j, board)))
                return 0;
        }
        else if (boardOrSol==1){
            if (i !=k && value==board->foundSolution[k][j]){
                return 0;
            }
        }
    }
    return 1;
}

int ValidBlock(int i, int j, int value, Board *board, int boardOrSol){
    int k,l, cellR = i%board->bRow, cellC = j%board->bCol;
    Block *block = getCellsBlock(i,j,board);

    for (k = 0; k < board->bRow; k++) {
        for (l = 0; l < board->bCol; l++) {
            if(boardOrSol==0) {
                if  ((cellR != k && cellC != l) && value == getCellValueFromBlock(k, l, block)) {
                    return 0;
                }
            }
            else if (boardOrSol==1){
                if  ((cellR !=k && cellC != l) && value == board->foundSolution[i-cellR+k][j-cellC+l]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void updateValueCell(Board *board, Cell *cell, int value){
    if (getValueCell(cell)==0 && value!=0){
        board->numberOfEmptyCells--;
    }
    else if (getValueCell(cell)!=0 && value==0){
        board->numberOfEmptyCells++;
    }
    setValueCell(cell,value);
}

void updateValueFixedCell(Board *board, Cell *cell, int value){
    if (getValueCell(cell)==0 && value!=0){
        board->numberOfEmptyCells--;
    }
    else if (getValueCell(cell)!=0 && value==0){
        board->numberOfEmptyCells++;
    }
    setFixedCellValue(cell,value);
}

int cellHasSingleLegalVal(int row,int col,Board *board){
    int value, oneVal=0, counter=0;
    int maxVal=board->bRow*board->bCol;

    for(value = 1 ; value < (maxVal+1) ; value++){
        if(checkCellNoErroneous(row, col, value, board,0)){
            counter++;
            oneVal=value;
        }
    }
    if (counter==1){
        return oneVal;
    }
    else if(counter>1){
        return 0;
    }
    return 0;
}

int findLegalCellValues(int i, int j, int legalVal[] ,Board *solvedBoard){
    int value, valuesCnt = 0;
    for(value=1; value<=solvedBoard->bCol*solvedBoard->bRow; value++){
        if(checkCellNoErroneous(i, j, value, solvedBoard,0)){
            legalVal[valuesCnt] = value;
            valuesCnt++;
        }
    }
    return valuesCnt;
}

int* findLegalValuesArrCell(Board *board ,int *legalValsSize, int *row, int *column){
    int value, valuesCnt = 0;
    int size = board->bRow*board->bCol;
    int* legalVals = (int*) malloc( size*sizeof(int));
    for(value=1; value<=size; value++){
        if(checkCellNoErroneous(*row, *column, value, board,0)){
            legalVals[valuesCnt] = value;
            valuesCnt++;
        }
    }
    *legalValsSize=valuesCnt;
    return legalVals;
}

Cell* randomEmptyCell(Board *board, int *row, int *col){
    Cell* currCell;
    int i, iC=0, jC=0,iN=0, jN=0, random=0;

    currCell = getCell(0, 0, board);
    if (getValueCell(currCell) != 0) {
        if (!getNextEmptyCell(iC, jC, &iN, &jN, board)) {
/*            printf("Error To delete!!!!!: Generate - there are no empty cells!\n");*/
            return 0;
        }
        iC = iN;
        jC = jN;
    }
    random = rand() % board->numberOfEmptyCells;
    for (i = 0; i < random; i++) {
        getNextEmptyCell(iC, jC, &iN, &jN, board);
        iC = iN;
        jC = jN;
    }
    *row=iC;
    *col=jC;
    return getCell(iC, jC, board);
}

void PrintFoundSolution(Board* board){
    int i,j;
    printf("foundSolution:\n");
    for (i = 0; i < board->bCol*board->bRow; i++) {
        printf("[");
        for (j=0; j< board->bCol*board->bRow; j++){
            printf(" %d,", board->foundSolution[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}