#ifndef SUDOKUPROJECT_BSTACK_H
#define SUDOKUPROJECT_BSTACK_H

#include "Board.h"

typedef struct BstructElement{
    int row;
    int col;
    struct BstructElement *prev;

}SavedCell;

typedef struct Bstack{
    int size;
    SavedCell *top;

}Stack;

/*
 * initialize a SavedCell stack element.
*/
SavedCell *newSavedCell(int i, int j, SavedCell *prev);

void initStack(Stack *stack);

/*
 * free a SavedCell stack element.
*/
void destroySavedCell(SavedCell *stack);

/*
 * init and push new SavedCell element to the stack.
*/
void push(Stack *stack , int i, int j);

/*
 * pop and extract values and free the top SavedCell of the stack.
*/
void pop(Stack *stack, int *i , int *j);

/*
 * returns 1 if Stack is empty else return 0.
*/
int isEmpty(Stack *stack );

#endif
