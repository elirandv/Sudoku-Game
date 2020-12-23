#include "BTsolver.h"

int movePartToLeft(int array[], int length, int i){
    for(; i<length-1; i++){
        array[i] = array[i+1];
    }
    return length-1;
}

int detBTSolveBoard(Board *copyBoard) {
    int icurr, jcurr, iNext, jNext, existNext, maxValue, counter;
    Cell *currCell;
    Stack stack;

    counter=0;
    icurr=0;
    jcurr=0;
    maxValue = copyBoard->bCol*copyBoard->bRow;
    initStack(&stack);

    currCell = getCell(icurr,jcurr, copyBoard);
    if(getValueCell(currCell)!=0){
        getNextEmptyCell(icurr ,jcurr, &icurr, &jcurr, copyBoard);
        currCell = getCell(icurr,jcurr, copyBoard);
    }
    push(&stack,icurr, jcurr);
    while (!isEmpty(&stack)) {
        pop(&stack,&icurr,&jcurr);
        currCell = getCell(icurr,jcurr,copyBoard);
        if (getValueCell(currCell)>=maxValue){
            updateValueCell(copyBoard,currCell,0);
            continue;
        }
        else{
            updateValueCell(copyBoard,currCell,1+getValueCell(currCell));
        }
        if (!checkCellNoErroneous(icurr,jcurr,getValueCell(currCell),copyBoard,0)){
            push(&stack,icurr,jcurr);
            continue;
        }
        existNext = getNextEmptyCell(icurr, jcurr, &iNext, &jNext, copyBoard);
        if (!existNext){
            counter++;
            updateValueCell(copyBoard,currCell,0);
            continue;
        }
        else{
            push(&stack,icurr,jcurr);
            icurr=iNext;
            jcurr=jNext;
            push(&stack,icurr,jcurr);
        }
    }
    return counter;
}
