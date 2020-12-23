#include "Interpreter.h"

 void Interpret(char* input, Command* command){
    char* token=NULL;
    float threshold;

    token = strtok(input, WHITS_SPACE);

	if (token == NULL){
        command->type=C_INVALID;
        free(token);
        return;
    }
	else if(strcmp(token,"solve")==0){
        command->type=C_SOLVE;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no file path entered!\n");
            free(token);
            return;
        }
        else{
            strcpy(command->path,token);
            token = strtok(NULL, WHITS_SPACE);
            if (ExtraParamsErr(command, token)){
                command->type=C_INVALID;
                printf("Error: bad file path entered!\n");
                free(token);
                return;
            }
        }
        free(token);
        return;
    }
	else if(strcmp(token,"edit")==0){
        command->type=C_EDIT;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            strcpy(command->path,"");
            free(token);
            return;
        }
        else{
            strcpy(command->path,token);
            token = strtok(NULL, WHITS_SPACE);
            if (ExtraParamsErr(command, token)){
                command->type=C_INVALID;
                printf("Error: bad file path entered!\n");
                free(token);
                return;
            }
        }
        free(token);
        return;
    }
	else if(strcmp(token,"mark_errors")==0){
        command->type=C_MARK_ERRORS;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_MARK_ERRORS;
            free(token);
            return;
        }
        else{
            if (!strToInt(token,&command->params[0])){
                command->type=C_INVALID;
                printf("Error: mark_errors illegal parameter!\n");
                free(token);
                return;
            }
            token = strtok(NULL, WHITS_SPACE);
            if (ExtraParamsErr(command, token)){
                command->type=C_INVALID;
                printf("Error: bad file path entered!\n");
                free(token);
                return;
            }
        }
        free(token);
        return;
    }
	else if(strcmp(token,"print_board")==0){
        command->type=C_PRINT_BOARD;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for board print command:'print_board' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"set")==0){
        command->type=C_SET;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no row value entered!\n");
            free(token);
            return;
        }
        if (!strToInt(token,&command->params[0])){
            command->type=C_INVALID;
            printf("Error: set illegal parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no column value entered!\n");
            free(token);
            return;
        }
        if (!strToInt(token,&command->params[1])){
            command->type=C_INVALID;
            printf("Error: set illegal parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no value entered!\n");
            free(token);
            return;
        }
        if (!strToInt(token,&command->params[2])){
            command->type=C_INVALID;
            printf("Error: set illegal parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: too many parameters entered!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"validate")==0){
        command->type=C_VALIDATE;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for validate command:'validate' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"guess")==0){

        command->type=C_GUESS;

        token = strtok(NULL, WHITS_SPACE);

        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no value entered!\n");
            free(token);
            return;
        }
        else {
            threshold = (float) atof(token);

            token = strtok(NULL, WHITS_SPACE);

            if (ExtraParamsErr(command, token)) {
                command->type=C_INVALID;
                printf("Error: too many parameters entered!\n");
                return;
            }

            if (threshold <= 0 || threshold > 1) {
                command->type = C_INVALID;
                printf("Error: illegal guess threshold value!\n");
                free(token);
                return;
            }


            command->threshold = threshold;
            free(token);
        }
    }
    else if(strcmp(token,"generate")==0){
            command->type=C_GENERATE;
            token = strtok(NULL, WHITS_SPACE);
            if (token==NULL){
                command->type=C_INVALID;
                printf("Error: no empty cells value entered!\n");
                free(token);
                return;
            }
            if (!strToInt(token,&command->params[0])){
                command->type=C_INVALID;
                printf("Error: generate illegal parameter!\n");
                free(token);
                return;
            }
            token = strtok(NULL, WHITS_SPACE);
            if (token==NULL){
                command->type=C_INVALID;
                printf("Error: no chosen cells value entered!\n");
                free(token);
                return;
            }
            if (!strToInt(token,&command->params[1])){
                command->type=C_INVALID;
                printf("Error: generate illegal parameter!\n");
                free(token);
                return;
            }
            token = strtok(NULL, WHITS_SPACE);
            if (ExtraParamsErr(command, token)){
                printf("Error: too many parameters entered!\n");
                free(token);
                return;
            }
            free(token);
            return;
    }
    else if(strcmp(token,"undo")==0){
        command->type=C_UNDO;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for undo command:'undo' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"redo")==0){
        command->type=C_REDO;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for redo command:'redo' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"save")==0){
        command->type=C_SAVE;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no file path entered!\n");
            free(token);
            return;
        }
        else{
            strcpy(command->path,token);
            token = strtok(NULL, WHITS_SPACE);
            if (ExtraParamsErr(command, token)){
                command->type=C_INVALID;
                printf("Error: bad file path entered!\n");
                free(token);
                return;
            }
        }
        free(token);
        return;
    }
    else if(strcmp(token,"hint")==0){
        command->type=C_HINT;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no row value entered!\n");
            return;
        }
        if (!strToInt(token,&command->params[0])){
            command->type=C_INVALID;
            printf("Error: hint illegal first parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no column value entered!\n");
            free(token);
            return;
        }
        if (!strToInt(token,&command->params[1])){
            printf("Error: hint illegal first parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: too many parameters entered!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"guess_hint")==0){
        command->type=C_GUESS_HINT;
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no row value entered!\n");
            free(token);
            return;
        }
        if (!strToInt(token,&command->params[0])){
            command->type=C_INVALID;
            printf("Error: guess_hint illegal first parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (token==NULL){
            command->type=C_INVALID;
            printf("Error: no column value entered!\n");
            free(token);
            return;
        }
        if (!strToInt(token,&command->params[1])){
            command->type=C_INVALID;
            printf("Error: guess_hint illegal first parameter!\n");
            free(token);
            return;
        }
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: too many parameters entered!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"num_solutions")==0){
        command->type=C_NUM_SOLUTIONS;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for num_solutions command:'num_solutions' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"autofill")==0){
        command->type=C_AUTOFILL;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for autofill command:'autofill' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"reset")==0){
        command->type=C_RESET;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for reset command:'reset' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else if(strcmp(token,"exit")==0){
        command->type=C_EXIT;
        token = strtok(NULL, WHITS_SPACE);
        if (ExtraParamsErr(command, token)){
            command->type=C_INVALID;
            printf("Error: for exit command:'exit' only!\n");
            free(token);
            return;
        }
        free(token);
        return;
    }
    else{
        command->type=C_INVALID;
        printf("Error: invalid command entered!\n");
    }
}


int strToInt(char* input, int *num){
    int i, negative;
    i=0;
    negative=0;
    *num = 0;
    if(input[i]=='-'){
        i++;
        negative=1;
    }
    while(input[i]!='\0'){
        *num =(*num)*10+(input[i]-'0');
        if(input[i] < '0' || input[i] > '9'){
            return 0;
        }
        i++;
    }
    if(negative==1){
        *num = (*num)*(-1);
    }
    return 1;
}

int ExtraParamsErr(Command* command, char* lastParam){
    if (lastParam!=NULL){
        command->type=C_INVALID;
        return 1;
    }
    return 0;
}