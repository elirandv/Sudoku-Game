#include "ProgramFlow.h"

int PlaySudoku(){
    char* input;
    Game* game = initGame();
    Command *command=NULL;

    input = (char*)malloc(MAX_STRING*sizeof(char));
    if (input == NULL) {
        printf("Error: input malloc has failed\n");
        printf("Exiting...\n");
        destroyGame(game);
        exit(0);
    }

    command = (Command*)malloc(sizeof(Command));
    if (command == NULL) {
        printf("Error: command malloc has failed\n");
        printf("Exiting...\n");
        destroyGame(game);
        free(input);
        exit(0);
    }

    command->path = (char*)malloc(MAX_STRING*sizeof(char));
    if (command->path == NULL) {
        printf("Error: command->path malloc has failed\n");
        printf("Exiting...\n");
        destroyGame(game);
        free(input);
        free(command);
        exit(0);
    }

    printTitle();

    printf("Please enter a command: ");

    while (1){
        if(fgets(input,MAX_STRING,stdin)==NULL){
            printf("Exiting Game...\n");
            free(input);
            if (command) {
                if (command->path) free(command->path);
                free(command);
            }
            destroyGame(game);
            return -1;
        }

        if(strcmp("\n",input)==0){
            printf("Please enter a command: ");
            continue;
        }

        if (strlen(input) > MAX_COMMAND_LENGTH){
            printf("Error: the command is too long\n");
            printf("Please enter a command: ");
            continue;
        }


        Interpret(input, command);

        ExecuteCommand(game, command);

        if(game->gamemode == ENDGAME){
            if (input) free(input);
            if (command) {
                if (command->path) free(command->path);
                free(command);
            }
            destroyGame(game);
            return -1;
        }
        printf("Please enter a command: ");
    }
}

void ExecuteCommand(Game *game, Command* command){
    switch (command->type)
    {
        case C_SOLVE:
            solve(game,command->path);
            break;
        case C_EDIT:
            edit(game,command->path);
            break;
        case C_MARK_ERRORS:
            markErrors(game,command->params[0]);
            break;
        case C_PRINT_BOARD:
            PrintBoard(game);
            break;
        case C_SET:
            set(game,command->params[0],command->params[1],command->params[2]);
            break;
        case C_VALIDATE:
            validate(game);
            break;
        case C_GUESS:
            guess(game, command->threshold);
            break;
        case C_GENERATE:
            generate(game ,command->params[0],command->params[1]);
            break;
        case C_UNDO:
            undo(game);
            break;
        case C_REDO:
            redo(game);
            break;
        case C_SAVE:
            save(game,command->path);
            break;
        case C_HINT:
            hint(game,command->params[0],command->params[1]);
            break;
        case C_GUESS_HINT:
            guessHint(game,command->params[0],command->params[1]);
            break;
        case C_NUM_SOLUTIONS:
            numSolutions(game);
            break;
        case C_AUTOFILL:
            autoFill(game);
            break;
        case C_RESET:
            reset(game);
            break;
        case C_EXIT:
            Exit(game);
            break;
        case C_INVALID:
            break;
        default:
            printf("Error: Bad command type for ExecuteCommand()");
            break;
    }
}
