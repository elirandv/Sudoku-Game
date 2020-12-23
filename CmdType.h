#ifndef SUDOKUPROJECT_CMDTYPE_H
#define SUDOKUPROJECT_CMDTYPE_H

#include <string.h>
#define MAX_COMMAND_LENGTH 256

typedef enum {
    C_SOLVE,
    C_EDIT,
    C_MARK_ERRORS,
    C_PRINT_BOARD,
    C_SET,
    C_VALIDATE,
    C_GUESS,
    C_GENERATE,
    C_UNDO,
    C_REDO,
    C_SAVE,
    C_HINT,
    C_GUESS_HINT,
    C_NUM_SOLUTIONS,
    C_AUTOFILL,
    C_RESET,
    C_EXIT,
    C_INVALID

} CmdType;

typedef struct Command_struct{
    CmdType type;
    char* path;
    float threshold;
    int params[3];
}Command;

#endif
