#ifndef SUDOKUPROJECT_MOVESLIST_H
#define SUDOKUPROJECT_MOVESLIST_H

#include "Board.h"

/*
 * select undo or redo while updating the board.
 */
typedef enum {
    UNDO,
    REDO
}UndoOrRedo;

/*
 * relevant actions for moves List.
 */
typedef enum {
    HEAD,
    SET,
    GENERATE,
    GUESS,
    AUTOFILL
}Action;

/*
 * Changes list node that happened during one action.
 */
typedef struct Changes_Node{
    struct Changes_Node* next;
    int row;
    int col;
    int oldValue;
    int newValue;
}ChangeNode;

/*
 * Moves list node per each action, holds:
 * - prev action node (if prev==NULL -> this is the first change in the board).
 * - next action node (if prev==NULL -> this is the last change in the board).
 * - changeListHead - head to the current action changes list.
 * - Action type.
 */
typedef struct actionNode{
    struct actionNode* prev;
    struct actionNode* next;
    ChangeNode* changeListHead;
    Action action;
}Node;

/*
 * Move list, there is only one exist per each game, holds:
 * - fatherboard - first board we initialize the game with.
 * - lastMove - pointer to the last change/action we did on board.
 * - chosenMove - pointer to the chosen change/action at the moment (after using one undo or more command).
 */
typedef struct GameMovesList{
    Board* fatherBord;
    Node* lastMove;
    Node* chosenMove;
    Node* MoveListHead;
}MovesList;

/*
 * Build ChangeNode.
 */
ChangeNode* newChangeNode(int row, int col, int oldValue, int newValue);

/*
 * Build actionNode.
 */
Node* newNode(Action action ,Node* lastMove);

/*
 * Build movesList.
 */
MovesList* newMovesList(Board* board);

/*
 * destroy movesList.
 */
void destroyMovesList(MovesList *movesList);

/*
 * when making action after one or more 'undo' this function is called to clear all the irrelevant action nodes in the movesList:
 * all nudes after chosenNode.
 */
void deleteAllActionNodesAfterChosenMove(MovesList* movesList);

/*
 * delete Change list.
 */
void deleteChangeList(ChangeNode *changeListHead);

/*
 * delete Last Node.
 */
void deleteLastNode(Node *lastNode);

/*
 * add new change node to the change list.
 */
ChangeNode* delChangeNode(ChangeNode* changeListHead, int row, int col);

/*
 * add new change node to the change list.
 */
void addChangeNode(ChangeNode* changeListHead, int row, int col, int oldValue, int newValue);

/*
 * add new action node to the movesList.
 */
void addActionNode(MovesList* movesList, Action action, ChangeNode* changeListHead);

/*
 * update the board after relevant action Redo or Undo.
 */
void updateBoard(Board *board, ChangeNode *changeListHead, UndoOrRedo ur);

/*
 * print all changes list for each action.
 */
void printChanges(ChangeNode *changeListHead, UndoOrRedo ur);

#endif