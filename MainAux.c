#include "MainAux.h"

void printCell(Cell* cell, int markErrorsFlag){
    if(isFixedCell(cell)){    /* fixed cell */
        printf(" %2d.", getValueCell(cell));
    }
    else{ /* non fixed cell */

        if(getValueCell(cell)==0){ /* 0 cell */
            printf("    ");
        }
        else if (markErrorsFlag && isCellErroneous(cell)==1){/* erroneous cell */
            printf(" %2d*", getValueCell(cell));
        }
        else{ /* non fixed non zero cell */
            printf(" %2d ", getValueCell(cell));
        }
    }
}

void printRowOfBlocks(Board* board, int row, int markErrorsFlag){
    int i,j,k;
    for(i=0; i<board->bRow; i++){
        printf("|");
        for(j=0; j<board->bRow; j++){ /* block number */
            for(k=0; k<board->bCol; k++){ /*cell's column index */
                printCell(board->chooseBlockInBoard[row][j]->chooseCellInBlock[i][k], markErrorsFlag);
            }
            printf("|");
        }
        printf("\n");
    }
}

void PrintBoard(Game* game){
    int i,j,markErrorsFlag=0;
    Board* board=game->board;

    if(game->gamemode==EDIT || game->markErrors==1){
        markErrorsFlag=1;
    }
    for(j=0;j<(1+CHAR_SPACE*(board->bCol*board->bRow)+board->bRow); j++){ /* print Separators */
        printf("-");
    }
    for(i=0; i<board->bCol; i++){ /*number of blocks in column */
        printf("\n");
        printRowOfBlocks(board, i, markErrorsFlag);
        for(j=0;j<(1+CHAR_SPACE*(board->bCol*board->bRow)+board->bRow); j++){ /* print Separators */
            printf("-");
        }
    }
    printf("\n");
/*    printf("PrintBoard: (DELETE) numOEmptyCells=%d\n",board->numberOfEmptyCells);*/
}

void PrintTempBoard(Board* board){
    int i,j,markErrorsFlag=1;
    for(j=0;j<(1+CHAR_SPACE*(board->bCol*board->bRow)+board->bRow); j++){ /* print Separators */
        printf("-");
    }
    for(i=0; i<board->bCol; i++){ /*number of blocks in column */
        printf("\n");
        printRowOfBlocks(board, i, markErrorsFlag);
        for(j=0;j<(1+CHAR_SPACE*(board->bCol*board->bRow)+board->bRow); j++){ /* print Separators */
            printf("-");
        }
    }
    printf("\n");
/*    printf("PrintBoard: (DELETE) numOEmptyCells=%d\n",board->numberOfEmptyCells);*/
}


void checkIfSolved(Game* game){
    if (game->board->numberOfEmptyCells==0 && game->gamemode==SOLVE){
        if (game->board->errorFlag){
            printf("Your solution is erroneous! you may undo or reset board\n");
        }
        else{
            printf("The soduko puzzle was successfully solved!! Game set to INIT mode\n");
            game->gamemode=INIT;
        }
    }
}
void printTitle() {
    printf("\n\n");
    printf("        *  *      *        *   *  *            *  *      *       *   *        *\n");
    printf("      *      *    *        *   *     *       *      *    *      *    *        *\n");
    printf("     *        *   *        *   *       *    *        *   *     *     *        *\n");
    printf("     *            *        *   *        *   *        *   *    *      *        *\n");
    printf("       *          *        *   *        *   *        *   *   *       *        *\n");
    printf("         *        *        *   *        *   *        *   *  *        *        *\n");
    printf("           *      *        *   *        *   *        *   * *         *        *\n");
    printf("             *    *        *   *        *   *        *   * *         *        *\n");
    printf("              *   *        *   *       *    *        *   *  *        *        *\n");
    printf("      *      *     *      *    *     *       *      *    *    *       *      * \n");
    printf("        *  *         *  *      *  *            *  *      *      *       *  *  \n\n");
    printf("                    Created by Meni Omaisy & Eliran Eitan     \n\n");
}
