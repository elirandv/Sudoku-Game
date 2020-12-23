#include "MovesList.h"

ChangeNode* newChangeNode(int row, int col, int oldValue, int newValue){
    ChangeNode *newChangeNode = (ChangeNode*)malloc(sizeof(ChangeNode));
    if (newChangeNode == NULL){
        printf("Error: newChangeNode has failed\n");
        free(newChangeNode);
        exit(0);
    }
    newChangeNode->next=NULL;
    newChangeNode->row=row;
    newChangeNode->col=col;
    newChangeNode->oldValue=oldValue;
    newChangeNode->newValue=newValue;
    return newChangeNode;
}

Node* newNode(Action action ,Node* lastMove){
    Node *newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL){
        printf("Error: newNode has failed\n");
        free(newNode);
        exit(0);
    }
    newNode->prev=lastMove;
    newNode->next=NULL;
    newNode->changeListHead=NULL;
    newNode->action=action;
    return newNode;
}

MovesList* newMovesList(Board *board){
    MovesList *movesList = (MovesList*) malloc(sizeof(MovesList));
    if (movesList == NULL){
        printf("Error: movesList has failed\n");
        free(movesList);
        exit(0);
    }

    movesList->fatherBord = newBoard(board->bRow,board->bCol);
    copyBoardValues(board , movesList->fatherBord);
    movesList->MoveListHead=newNode(HEAD,NULL);
    movesList->lastMove=movesList->MoveListHead;
    movesList->chosenMove=movesList->MoveListHead;
    return movesList;
}

void destroyMovesList(MovesList *movesList) {
    if (!movesList) {
        return;
    }
    destroyBoard(movesList->fatherBord);
    while (movesList->chosenMove->action!=HEAD) {
        movesList->chosenMove=movesList->chosenMove->prev;
    }
    deleteAllActionNodesAfterChosenMove(movesList);
    deleteLastNode(movesList->chosenMove);
    free(movesList->lastMove);
    free(movesList);
}

void deleteAllActionNodesAfterChosenMove(MovesList *movesList){
    while (movesList->lastMove!=movesList->chosenMove){
        movesList->lastMove=movesList->lastMove->prev;
        deleteLastNode(movesList->lastMove->next);
        movesList->lastMove->next=NULL;
    }
}

void deleteLastNode(Node* lastNode){
    if (!lastNode) {
        return;
    }
    deleteChangeList(lastNode->changeListHead);
/*    free(lastNode->prev);
    free(lastNode->next);
    free(lastNode);*/
}

void deleteChangeList(ChangeNode *changeListHead){
    ChangeNode *temp;
    if (!changeListHead) {
        return;
    }
    while (changeListHead!=NULL){
        temp=changeListHead->next;
        free(changeListHead);
        changeListHead=temp;
    }
/*    free(temp);*/
}

ChangeNode* delChangeNode(ChangeNode* changeListHead, int row, int col){
    ChangeNode *temp, *prev;
    if (!changeListHead) {
        return changeListHead;
    }
    temp=changeListHead;
    if (temp->row==row && temp->col==col) {
        changeListHead = temp->next;
        free(temp);
        return changeListHead;
    }
    prev=temp;
    temp=temp->next;
    while (temp!=NULL){
        if (temp->row==row && temp->col==col) {
            prev->next = temp->next;
            free(temp);
            return changeListHead;
        }
        temp=temp->next;
    }
/*    free(temp);*/
    return changeListHead;
}

void addChangeNode(ChangeNode *changeListHead, int row, int col, int oldValue, int newValue){
    ChangeNode* newCnode = newChangeNode(row,col,oldValue,newValue);
    newCnode->next=changeListHead->next;
    changeListHead->next=newCnode;
/*    free(newCnode);*/
}

void addActionNode(MovesList *movesList, Action action, ChangeNode *changeListHead){
    Node *newActioNode = newNode(action, movesList->lastMove);
    newActioNode->changeListHead=changeListHead;
    movesList->lastMove->next=newActioNode;
    newActioNode->prev=movesList->lastMove;
    movesList->lastMove=newActioNode;
    movesList->chosenMove=newActioNode;
/*    free(newActioNode);*/
}

void updateBoard(Board *board, ChangeNode *changeListHead, UndoOrRedo ur){
    ChangeNode* changePointer=changeListHead;
    while (changePointer!=NULL) {
        if (ur == UNDO) {
            updateValueCell(board, getCell(changePointer->row, changePointer->col, board), changePointer->oldValue);
        }
        else {
            updateValueCell(board, getCell(changePointer->row, changePointer->col, board), changePointer->newValue);
        }
        changePointer=changePointer->next;
    }
/*    free(changePointer);*/
    findErrorsInBoard(board);
}

void printChanges(ChangeNode *changeListHead, UndoOrRedo ur){
    ChangeNode* changePointer=changeListHead;
    while (changePointer!=NULL) {
        if (ur == UNDO) {
            printf("cell[%d][%d] value has changed from: %d to %d.\n", changePointer->row, changePointer->col, changePointer->newValue, changePointer->oldValue);
        }
        else {
            printf("cell[%d][%d] value has changed from: %d to %d.\n", changePointer->row, changePointer->col, changePointer->oldValue, changePointer->newValue);
        }
        changePointer=changePointer->next;
    }
/*    free(changePointer);*/
}