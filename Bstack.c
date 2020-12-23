#include "Bstack.h"

SavedCell *newSavedCell(int i, int j, SavedCell *prev){
    SavedCell *newCell = (SavedCell*) malloc(sizeof(SavedCell));
    if (newCell == NULL){
        printf("Error: newSavedCell malloc has failed\n");
        exit(0);
    }
    newCell->row=i;
    newCell->col=j;
    newCell->prev=prev;
    return newCell;
}

void initStack(Stack *stack) {
    stack->size = 0;
    stack->top = NULL;
}

void destroySavedCell(SavedCell *cell){
    if (!cell){
        return;
    }
    free(cell);
}

void push(Stack *stack , int i, int j){
    SavedCell *newCell = newSavedCell(i,j,stack->top);
    stack->top=newCell;
    stack->size++;
}

void pop(Stack *stack, int *i , int *j){
    SavedCell *popCell = stack->top;
    *i=popCell->row;
    *j=popCell->col;
    stack->top=popCell->prev;
    stack->size--;
    destroySavedCell(popCell);
}

int isEmpty(Stack *stack ){
    if (stack->size==0){
        return 1;
    }
    return 0;
}