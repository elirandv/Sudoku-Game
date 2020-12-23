#include "GameFuncs.h"
#include "MainAux.h"

Game* initGame(){
    Game *game = (Game*) malloc(sizeof(Game));
    if (game == NULL) {
        printf("Error: game malloc has failed\n");
        exit(0);
    }
    game->board=NULL;
    game->markErrors=1;
    game->gamemode=INIT;
    game->movesList=NULL;
    return game;
}

void resetGame(Game *game) {
    game->board = NULL;
    game->markErrors = 1;
    game->gamemode = INIT;
    game->movesList=NULL;
}

void destroyGame(Game *game){
    if (!game) {
        return;
    }
    destroyBoard(game->board);
    destroyMovesList(game->movesList);
    free(game);
}

void solve(Game* game, char* path){
    if (game->board!=NULL) {
        destroyBoard(game->board);
        destroyMovesList(game->movesList);
        resetGame(game);
    }
    game->board=loadBoardFromFile(path, 0);
    if (game->board==NULL){
        resetGame(game);
        return;
    }
    findErrorsInBoard(game->board);
    game->markErrors=1;
    game->movesList = newMovesList(game->board);
    game->gamemode = SOLVE;
    PrintBoard(game);
}

void edit(Game *game, char* path){
    if (game->board!=NULL) {
        destroyBoard(game->board);
        destroyMovesList(game->movesList);
        resetGame(game);
    }
    if(strcmp(path,"")==0){
        game->board = newDefultBoard();
    }
    else{
        game->board=loadBoardFromFile(path, 1);
        if (game->board==NULL){
            resetGame(game);
            return;
        }
        findErrorsInBoard(game->board);
    }
    game->markErrors=1;
    game->movesList=newMovesList(game->board);
    game->gamemode=EDIT;
    PrintBoard(game);
}

void markErrors(Game *game, int x){
    if (game->gamemode!=SOLVE){
        printf("Error: mark_errors is only available in Solve mode!\n");
        return;
    }
    if (x==0 || x==1){
        game->markErrors=x;
        printf("Mark_errors is set to %d!\n",x);
    }
    else printf("Error: mark_errors is only set to 0 or 1!\n");
}

void set(Game *game, int x, int y, int value){
    int size, oldValue;
    Cell* cell=NULL;
    ChangeNode *changeNodeHead=NULL;

    if (game->gamemode==INIT){
        printf("Error: set is not available in Init mode!\n");
        return;
    }
    size=(game->board->bRow*game->board->bCol);
    if (x<0 || x>=size || y<0 || y>=size){
        printf("Error: illegal cell's index parameters, Allowed index range:[%d,%d]!\n", 0, size-1);
        return;
    }
    if (value<0 || value>size){
        printf("Error: illegal value parameter!\n");
        return;
    }
    cell = getCell(x,y,game->board);
    oldValue=getValueCell(cell);
    if (game->gamemode==EDIT){
        updateValueCell(game->board, cell,value);
    }
    else if (game->gamemode==SOLVE){
        if (isFixedCell(cell)){
            printf("Error: set fixed cell is not available in solve mode!\n");
            return;
        }
        updateValueCell(game->board, cell,value);
    }
    findErrorsInBoard(game->board);
    deleteAllActionNodesAfterChosenMove(game->movesList);
    changeNodeHead = newChangeNode(x, y, oldValue, value);
    addActionNode(game->movesList, SET, changeNodeHead);
    PrintBoard(game);
    checkIfSolved(game);
}

void validate(Game *game){
    if (game->gamemode==INIT){
        printf("Error: undo is not available in Init mode!\n");
        return;
    }
    if (game->board->errorFlag){
        printf("Error: erroneous boards may not be validate!\n");
        return;
    }
    if (gurobiSolve(game->board,G_VALIDATE,0,0,0)){
        printf("The board is solvable!\n");
    }
    else printf("The board is not solvable!\n");
}

void generate(Game *game, int x, int y){
    int boardSize, iter=0;

    if (game->gamemode!=EDIT){
        printf("Error: generate is only available in Edit mode!\n");
        return;
    }
    if (game->board->numberOfEmptyCells<x){
        printf("Error: There are less than %d empty cells!\n",x);
        return;
    }
    boardSize=(game->board->bRow)*(game->board->bRow)*(game->board->bCol)*(game->board->bCol);
    if (y>boardSize){
        printf("Error: There are less than %d cells in the board!\n",y);
        return;
    }
    while ((!generateGurobi(game, x, y))&&(iter<1000)){
        iter++;
    }
    if (iter==1000){
        printf("Error: Puzzle generator over 1000 iterations!\n");
        return;
    }
    PrintBoard(game);
}

int generateGurobi(Game *game, int x, int y) {
	int size, m = 0, value, randomX = 0, randomY = 0, randomValIndex = 0, legalValsSize, iC=0, jC=0;
	int *legalVals=NULL;
    Board *board=NULL, *tempBoard=NULL;
    Cell *currCell=NULL;
    ChangeNode *changeNodeHead=NULL;

    board=game->board;
    tempBoard = newBoard(board->bRow,board->bCol);
    copyBoardValues(board,tempBoard);

    while (m < x) {
        currCell = randomEmptyCell(tempBoard, &iC, &jC);
        legalVals = findLegalValuesArrCell(tempBoard, &legalValsSize, &iC, &jC);
        if (legalValsSize == 0) {
/*            printf("Error To delete!!!!!: Generate - one of the X randomly-chosen cells has no legal value available!\n");*/
            destroyBoard(tempBoard);
            free(legalVals);
            return 0;
        }
        randomValIndex = rand() % legalValsSize;
        updateValueCell(tempBoard, currCell, legalVals[randomValIndex]);
        if (changeNodeHead == NULL) {
            changeNodeHead = newChangeNode(iC, jC, 0, legalVals[randomValIndex]);
        } else {
            addChangeNode(changeNodeHead, iC, jC, 0, legalVals[randomValIndex]);
        }
        m++;
    }
    if (!gurobiSolve(tempBoard ,G_GENERATE,0,0,0)){
/*        printf("Error To delete!!!!!: Generate - unsolvable random board!\n");*/
        destroyBoard(tempBoard);
        free(legalVals);
        return 0;
    }

    size=board->bRow*board->bCol;
    m=(size*size)-y;

    while (tempBoard->numberOfEmptyCells<m){
        randomX = rand() % size;
        randomY = rand() % size;
        currCell=getCell(randomX, randomY, tempBoard);
        value = getValueCell(currCell);
        if(value!=0) {
            updateValueCell(tempBoard, currCell, 0);
            if (getValueCell(getCell(randomX, randomY, board)) != 0) {
                if (changeNodeHead == NULL) {
                    changeNodeHead = newChangeNode(randomX, randomY, value, 0);
                } else {
                    addChangeNode(changeNodeHead, randomX, randomY, value, 0);
                }
            }
            else changeNodeHead = delChangeNode(changeNodeHead, randomX, randomY);
        }
    }
    addActionNode(game->movesList, GENERATE, changeNodeHead);
    copyBoardValues(tempBoard,board);
    destroyBoard(tempBoard);
    free(legalVals);
    return 1;
}

void undo(Game *game){
    Node* currentMove=NULL;;
    if (game->gamemode==INIT){
        printf("Error: undo is not available in Init mode!\n");
        return;
    }
    currentMove=game->movesList->chosenMove;
    if (currentMove->action==HEAD){
        printf("Error: there are no moves to undo!\n");
        return;
    }
    if (currentMove->prev->action==HEAD){
        copyBoardValues(game->movesList->fatherBord , game->board);
    }
    else{
        updateBoard(game->board, currentMove->changeListHead, UNDO);
        printChanges(currentMove->changeListHead, UNDO);
    }
    game->movesList->chosenMove=currentMove->prev;
    PrintBoard(game);
}

void redo(Game *game){
    Node* currentMove=NULL;
    if (game->gamemode==INIT){
        printf("Error: undo is not available in Init mode!\n");
        return;
    }
    currentMove=game->movesList->chosenMove;
    if (currentMove==game->movesList->lastMove){
        printf("Error: there are no moves to redo!\n");
        return;
    }
    currentMove=currentMove->next;
    updateBoard(game->board, currentMove->changeListHead, REDO);
    printChanges(currentMove->changeListHead, REDO);
    PrintBoard(game);
    game->movesList->chosenMove=game->movesList->chosenMove->next;
}

void save(Game *game, char* path){
    if (game->gamemode==INIT){
        printf("Error: save is not available in Init mode!\n");
        return;
    }
    if (game->gamemode==EDIT){
        if (game->board->errorFlag){
            printf("Error: erroneous boards may not be saved!\n");
            return;
        }
        else{
            saveBoardToFile(path, game->board, 1);
        }
    }
    else{
        saveBoardToFile(path, game->board, 0);
    }
}

void hint(Game *game, int x, int y) {
    Cell *cell=NULL;
    int hint=0;

    if (game->gamemode != SOLVE) {
        printf("Error: hint is only available in Solve mode!\n");
        return;
    }
    if (game->board->errorFlag) {
        printf("Error: no hint possible for erroneous board!\n");
        return;
    }
    cell = getCell(x, y, game->board);
    if (isFixedCell(cell)){
        printf("Error: cell[%d][%d] is already fixed!\n", x, y);
        return;
    }
    if (getValueCell(cell) != 0){
        printf("Error: cell[%d][%d] is already filled!\n", x, y);
        return;
    }

    hint = gurobiSolve(game->board, G_HINT ,0, x, y);

    if (!hint){
        printf("Error: hint is illegal when the board is not solvable!\n");
        return;
    }
    printf("The hint value for cell[%d][%d] is: %d\n",x, y, hint);
}


void guess(Game *game, float threshold){
    if (game->gamemode!=SOLVE){
        printf("Error: guess is only available in Solve mode!\n");
        return;
    }
    if (game->board->errorFlag){
        printf("Error: erroneous boards may not be validate!\n");
        return;
    }
    gurobiSolve(game->board,G_GUESS,threshold,0,0);
    getGuessSolution(game);
    PrintBoard(game);
}

int getGuessSolution(Game *game){
    int i, j;
    Board* board=game->board;
    int size = (board->bCol * board->bRow);
    Cell *currCell=NULL;
    ChangeNode *changeNodeHead=NULL;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            currCell = getCell(i, j, board);
            if (getValueCell(currCell) == 0) {
                updateValueCell(board, currCell, board->foundSolution[i][j]);
                if (changeNodeHead == NULL) {
                    changeNodeHead = newChangeNode(i, j, 0, board->foundSolution[i][j]);
                } else {
                    addChangeNode(changeNodeHead, i, j, 0, board->foundSolution[i][j]);
                }
                findErrorsInBoard(board);
            }
        }
    }
    addActionNode(game->movesList, GUESS, changeNodeHead);
    return 1;
}


void guessHint(Game *game, int x, int y){
    Cell *cell=NULL;
    int guessHint=0;

    if (game->gamemode != SOLVE) {
        printf("Error: hint is only available in Solve mode!\n");
        return;
    }
    if (game->board->errorFlag) {
        printf("Error: no hint possible for erroneous board!\n");
        return;
    }
    cell = getCell(x, y, game->board);
    if (isFixedCell(cell)){
        printf("Error: cell[%d][%d] is already fixed!\n", x, y);
        return;
    }
    if (getValueCell(cell) != 0){
        printf("Error: cell[%d][%d] is already filled!\n", x, y);
        return;
    }
    guessHint=gurobiSolve(game->board, G_GUESS_HINT ,0, x, y);
    if (!guessHint){
        printf("Error: guess hint is illegal when the board is not solvable!\n");
        return;
    }
}

void numSolutions(Game *game){
    int num=0;
    Board *copyBoard;

    if (game->board->errorFlag!=0){
        printf("Error: the Board is Erroneous!\n");
        return;
    }
    if (game->board->numberOfEmptyCells==0){
        printf("The Board if Full thus has one solution\n");
        return;
    }
    copyBoard=newBoard(game->board->bRow, game->board->bCol);
    copyBoardValues(game->board,copyBoard);
    num = detBTSolveBoard(copyBoard);
    printf("There are %d solutions for the current board!\n", num);
    destroyBoard(copyBoard);
}

void autoFill(Game *game){
    int i,j,value;
    Board *board=NULL, *tempBoard=NULL;
    ChangeNode *changeNodeHead=NULL;
    if(game->gamemode != SOLVE){
        printf("Error: autofill is only available in Solve mode!\n");
        return;
    }
    if (game->board->errorFlag){
        printf("Error: erroneous boards may not be auto filled!\n");
        return;
    }
    board=game->board;
    tempBoard = newBoard(board->bRow,board->bCol);
    copyBoardValues(board,tempBoard);
    for(j = 0 ; j < (tempBoard->bRow*tempBoard->bCol) ; j++){
        for(i = 0 ; i < (tempBoard->bRow*tempBoard->bCol) ; i++){
            if (getValueCell(getCell(i,j,board))==0){
                value = cellHasSingleLegalVal(i,j,board);
                if (value){
                    updateValueCell(tempBoard, getCell(i,j,tempBoard),value);
                    if (changeNodeHead==NULL){
                        changeNodeHead = newChangeNode(i, j, 0, value);
                    }
                    else{
                        addChangeNode(changeNodeHead, i, j, 0,value);
                    }
                }
            }
        }
    }
    findErrorsInBoard(tempBoard);
    addActionNode(game->movesList, AUTOFILL, changeNodeHead);
    copyBoardValues(tempBoard,board);
    PrintBoard(game);
    checkIfSolved(game);
    destroyBoard(tempBoard);
}

void reset(Game *game){
    if (game->gamemode==INIT){
        printf("Error: reset is not available in Init mode!\n");
        return;
    }
    game->movesList->chosenMove=game->movesList->MoveListHead;
    copyBoardValues(game->movesList->fatherBord , game->board);
    PrintBoard(game);
}

void Exit(Game *game){
    game->gamemode = ENDGAME;
}
