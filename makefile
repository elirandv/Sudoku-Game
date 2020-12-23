CC = gcc
OBJS = main.o ProgramFlow.o Interpreter.o MainAux.o GameFuncs.o BTsolver.o Bstack.o File.o Board.o Block.o Cell.o MovesList.o GurobiSolver.o

EXEC = sudoku-console
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c ProgramFlow.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
ProgramFlow.o: ProgramFlow.c ProgramFlow.h MainAux.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
MainAux.o: MainAux.c MainAux.h GameFuncs.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
GameFuncs.o: GameFuncs.c GameFuncs.h File.h MovesList.h Interpreter.h BTsolver.h GurobiSolver.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Interpreter.o: Interpreter.c Interpreter.h CmdType.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
File.o: File.c File.h Board.h GameFuncs.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Board.o: Board.c Board.h Block.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Block.o: Block.c Block.h Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Cell.o: Cell.c Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
MovesList.o: MovesList.c MovesList.h Board.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
BTsolver.o: BTsolver.c BTsolver.h Bstack.h Board.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Bstack.o: Bstack.c Bstack.h Board.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
GurobiSolver.o: GurobiSolver.c GurobiSolver.h Gurobi.h Board.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f *.o $(EXEC)
