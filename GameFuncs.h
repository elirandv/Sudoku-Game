#ifndef SUDOKUPROJECT_GAMEFUNCS_H
#define SUDOKUPROJECT_GAMEFUNCS_H

#include "File.h"
#include "MovesList.h"
#include "Interpreter.h"
#include "BTsolver.h"
#include "GurobiSolver.h"

/**
 * The different game modes.
 */
typedef enum SudokuGameMode{
    INIT,
    SOLVE,
    EDIT,
    ENDGAME
} GameMode;

/**
 * Game status structure, contains the fields:
 * board - a pointer to the game board.
 * mode - contains the game play mode.
 * markErrors - in solve mode the user can ask to see errors in the game board by setting this member to 1.
 * history - a pointer to linked list that saves the user previous moves.
 */
typedef struct GameStatusStruct{
    GameMode gamemode;
    Board *board;
    int markErrors;
    MovesList *movesList;
} Game;

/**
* Initialize game status.
*/
Game* initGame();

/**
* reset game, keeps pointer.
*/
void resetGame(Game *game);

/**
* Free's game's memory
*/
void destroyGame(Game *game);

/**
* Starts a puzzle in Edit mode, loaded from a file with the name "X", where X includes a full or relative path to the file.
* The parameter X is optional. If no parameter is supplied, the program should enter Edit mode with an empty 9x9 board. There is no command to create a board of different size (the user must edit a file for that, explained later).
* Recall that in Edit mode we always mark errors, regardless of the value of the "mark errors" parameter (which remains whatever value it contained previously).
*/
void solve(Game *game, char* path);

/**
* Starts a puzzle in Edit mode, loaded from a file with the name "X", where X includes a full or relative path to the file.
* The parameter X is optional. If no parameter is supplied, the program should enter Edit mode with an empty 9x9 board. There is no command to create a board of different size (the user must edit a file for that, explained later).
*/
void edit(Game *game, char* path);

/**
* Sets the "mark errors" setting to X, where X is either 0 or 1.
* This command is only available in Solve mode.
*/
void markErrors(Game *game, int x);

/**
* Sets the value of cell <X,Y> to Z.
* This command is only available in Edit and Solve modes.
*/
void set(Game *game, int x, int y, int value);

/**
* Validates the current board using ILP, ensuring it is solvable.
* This command is only available in Edit and Solve modes.
* If the board is erroneous, the program prints an error message and the command is not executed.
* The command prints whether the board is found to be solvable, or not.
*/
void validate(Game *game);

/**
generate X Y
a. Generates a puzzle by randomly filling X empty cells with legal values, running ILP to solve the board, and then clearing all but Y random cells.
b. This command is only available in Edit mode.
c. If the board does not contain X empty cells, then that is an error and the user should be notified.
 generate - shell function.
 generateGurobi - core function.
*/
void generate(Game *game, int x, int y);
int generateGurobi(Game *game, int x, int y);

/**
* Undo a previous move done by the user.
* This command is only available in Edit and Solve modes.
*/
void undo(Game *game);

/**
* Redo a move previously undone by the user.
* This command is only available in Edit and Solve modes.
*/
void redo(Game *game);

/**
* Saves the current game board to the specified file, where X includes a full or relative path to the file.
* This command is only available in Edit and Solve modes.
*/
void save(Game *game, char* path);

/**
* Give a hint to the user by showing the solution of a single cell X,Y.
* This command is only available in Solve mode.
* If the board is erroneous, cell <X,Y> is fixed, or cell <X,Y> already contains a value, it is an error (in that order).
* Run ILP to solve the board. If the board is unsolvable, it is an error. Otherwise, print to the user the value of cell <X,Y> found by the ILP solution.
*/
void hint(Game *game, int x, int y);

/**
* Guesses a solution to the current board using LP (not ILP!), with threshold X.
* This command is only available in Solve mode.
* The parameter X is a float and represents the threshold we use for the LP solution.
* If the board is erroneous, the program prints an error message and the command is not executed.
*/
void guess(Game *game, float threshold);

int getGuessSolution(Game *game);

/**
guess_hint X Y
* Show a guess to the user for a single cell X,Y.
* This command is only available in Solve mode.
* If the board is erroneous, cell <X,Y> is fixed, or cell <X,Y> already contains a value, it is an error (in that order).
*/
void guessHint(Game *game, int x, int y);

/**
* Print the number of solutions for the current board.
* This command is only available in Edit and Solve modes.
* If the board is erroneous it is an error.
*/
void numSolutions(Game *game);

/*
* Automatically fill "obvious" values – cells which contain a single legal value.
* This command is only available in Solve mode.
* If the board is erroneous it is an error.
*/
void autoFill(Game *game);


/*
* Undo all moves, reverting the board to its original loaded state.
* This command is only available in Edit and Solve modes.
*/
void reset(Game *game);

/*
* Terminates the program.
*/
void Exit(Game *game);


#endif
