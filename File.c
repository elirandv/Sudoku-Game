#include "File.h"


Board *loadBoardFromFile(char* path, int gameInEditMode){
    Board* board = NULL;
    FILE* filePointer = NULL;
    int bRow, bCol, i, j, num;

     if (access(path, F_OK) != -1) {
        filePointer = fopen(path, "r");
        if (!filePointer) {
            printf("Error: file does not open!\n");
            free(filePointer);
            free(board);
            return NULL;
        }
        if (fscanf(filePointer, "%d %d", &bRow, &bCol)  != 2 || bRow*bCol>99) {
            printf("Error: The requested file is in bad format\n");
            if (fclose(filePointer) == EOF) {
                printf("Error: failure closing the file\n");
                free(board);
                exit(0);
            }
            return NULL;
        }

        board = newBoard(bRow, bCol);

        for(i=0; i<bRow*bCol; i++){
            for(j=0; j<bRow*bCol; j++){
                if (fscanf(filePointer,"%d",&num)!=1){
                    printf("Error: The requested file is in bad format\n");
                    fclose(filePointer);
                    destroyBoard(board);
                    exit(0);
                }
                if(num<0 || num> bRow*bCol){
                    printf("Error: The requested file is in bad format - value out of range!\n");
                    fclose(filePointer);
                    destroyBoard(board);
                    exit(0);
                }
                if(fgetc(filePointer) == '.' && (gameInEditMode==0)){
                    updateValueFixedCell(board,getCell(i, j, board), num);

                }
                else{
                    updateValueCell(board,getCell(i, j, board), num);
                }
            }
        }
        if (fclose(filePointer) == EOF) {
            printf("Error: failure closing the file\n");
            destroyBoard(board);
            exit(0);
        }
        printf("Board was successfully loaded!\n");
        return board;
    }
    printf("Error: file does not exists or path is wrong!\n");
    free(board);
    free(filePointer);
    return NULL;
}

void saveBoardToFile(char* path, Board* board, int gameInEditMode){
    FILE *filePointer=NULL;
    int bRow = board->bRow,bCol = board->bCol, i,j;
    Cell *cell = NULL;
    filePointer = fopen(path, "w");
    if(filePointer==NULL){
        printf("Error: file open has failed\n");
        return;
    }

    fprintf(filePointer, "%d %d\n",bRow,bCol); /* put number of block columns and rows in file */
    for (i=0; i<bRow*bCol; i++){
        for (j=0; j<bRow*bCol; j++){
            cell = getCell(i,j,board);
            if (isFixedCell(cell) || (gameInEditMode && getValueCell(cell)!=0)){
                if(j == bRow*bCol-1){
                    fprintf(filePointer, "%d.",getValueCell(cell));
                }
                else
                    fprintf(filePointer, "%d. ",getValueCell(cell));
            }
            else {
                if(j == bRow*bCol-1){
                    fprintf(filePointer, "%d",getValueCell(cell));
                }
                else
                    fprintf(filePointer, "%d ", getValueCell(cell));
            }
        }
        fprintf(filePointer,"\n");
    }

    if (fclose(filePointer) == EOF) {
        printf("Error: failure closing the file\n");
        exit(0);
    }

    printf("Board was successfully saved!\n");
}