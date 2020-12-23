
#ifndef SUDOKUPROJECT_INTERPRETER_H
#define SUDOKUPROJECT_INTERPRETER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "CmdType.h"


#define WHITS_SPACE " \n\t\r"

void destroyCommand(Command *command);

/*
 *  Interprets the received string to Command type
*/
void Interpret(char* input, Command* command);

/*
 *  returns int value for a given string.
*/
int strToInt(char* str, int *num);

/*
 *  returns 0 if no Extra Parameters detected, otherwise returns 1.
*/
int ExtraParamsErr(Command* command, char* str);

#endif
