#include "GurobiSolver.h"

void addCellsConstraint(GRBmodel* model, Board* board, int* ind, double* val){
    int i,j,v,error;
    int size=board->bCol*board->bRow;
	Cell* cell = NULL;
    int* indOne;
    double* valOne;

    indOne = (int*) calloc(1, sizeof(int));
    if (indOne == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }
    valOne = (double*) calloc(1, sizeof(double));
    if (valOne == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
			cell = getCell(i, j, board);
            cleanGurArr(ind, val, size);
            for (v = 0; v < size; v++) {
                if (getValueCell(cell) == v + 1) {
                    indOne[0] = i * size * size + j * size + v;
                    valOne[0] = 1.0;
                    error = GRBaddconstr(model, 1, indOne, valOne, GRB_EQUAL, 1.0, NULL);
                    if (error) {
                        printf("ERROR: Cells Constraint in GRBaddconstr()\n cell[%d][%d] ind:",i,j);
                        printIntArr(ind,size);
                        return;
                    }
                }
                ind[v] = i * size * size + j * size + v;
                val[v] = 1.0;
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR: Cells Constraint in GRBaddconstr()\n cell[%d][%d] ind:",i,j);
                printIntArr(ind,size);
                return;
            }
        }
    }
    free(indOne);
    free(valOne);
}

void addRowConstraint(GRBmodel* model, Board* board, int* ind, double* val){
    int i,j,v,error;
    int size=board->bCol*board->bRow;

    for (i = 0; i < size; i++) {
        for (v = 0; v < size; v++) {
            cleanGurArr(ind, val, size);
            for (j = 0; j < size; j++) {
                ind[j] = i*size*size + j*size + v;
                val[j] = 1.0;
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR: Rows Constraint in GRBaddconstr()\n");
                return;
            }
        }
    }
}

void addColConstraint(GRBmodel* model, Board* board, int* ind, double* val){
    int i,j,v,error;
    int size=board->bCol*board->bRow;

    for (j = 0; j < size; j++) {
        for (v = 0; v < size; v++) {
            cleanGurArr(ind, val, size);
            for (i = 0; i < size; i++) {
                ind[i] = i*size*size + j*size + v;
                val[i] = 1.0;
            }
            error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR: Columns Constraint in GRBaddconstr()\n");
                return;
            }
        }
    }
}

void addBlockConstraint(GRBmodel* model, Board* board, int* ind, double* val){
    int m,n,i,j,v, count=0, error;
    int size=board->bCol*board->bRow;

    for (v = 0; v < size; v++) {
        for(m=0; m<board->bCol; m++){
            for(n=0; n<board->bRow; n++){
                cleanGurArr(ind, val, size);
                count=0;
                for (i=0; i< board->bRow; i++) {
                    for (j = 0; j < board->bCol; j++) {
                        ind[count] = (((m * board->bRow)+i) * size*size) +  (((n*board->bCol)+j) *size ) + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR: blocks Constraint in GRBaddconstr()\n");
                    return;
                }
            }
        }
    }
}

int gurobiSolve(Board* board, GType type , float  threshold, int x, int y){

    int size = (board->bCol * board->bRow);
    int D3size = size*size*size;
    int i,error;
    int solution=0;

    double* sol;
    int* ind;
    double* val;
    double* obj;
    char* vtype;
    int optimstatus;
    double objval;

    GRBenv *env = NULL;
    GRBmodel *model = NULL;

    sol = (double*) calloc(D3size, sizeof(double));
    if (sol == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }
    ind = (int*) calloc(size, sizeof(int));
    if (ind == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }
    val = (double*) calloc(size, sizeof(double));
    if (val == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }
    obj = (double*) calloc(D3size, sizeof(double));
    if (obj == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }
    vtype = (char*) calloc(D3size, sizeof(char));
    if (vtype == NULL) {
        printf("Error: gurobi malloc has failed\n");
        exit(0);
    }

    error = GRBloadenv(&env, "gurobiSolution.log");
    if (error) {
        printf("Error: GRBloadenv() has failed\n");
    }

    error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (error) {
        printf("Error: GRBsetintparam() has failed\n");
    }

    error = GRBnewmodel(env, &model, "gurobiSolution", 0, NULL, NULL, NULL, NULL, NULL);
    if (error) {
        printf("Error: GRBnewmodel() has failed\n");
    }

    for (i = 0; i < D3size; i++) {
                                                     /* ///////////////////////////////////////////////////is this the target function?*/
        obj[i] = 0;
    }

    switch (type) {
        case G_VALIDATE:
            for (i = 0; i < D3size; i++) {
                vtype[i] = GRB_BINARY;
            }
            break;
        case G_GENERATE:
            for (i = 0; i < D3size; i++) {
                vtype[i] = GRB_BINARY;
            }
            break;
        case G_HINT:
            for (i = 0; i < D3size; i++) {
                vtype[i] = GRB_BINARY;
            }
            break;
        case G_GUESS:
            for (i = 0; i < D3size; i++) {
                vtype[i] = GRB_CONTINUOUS;
            }
            break;
        case G_GUESS_HINT:
            for (i = 0; i < D3size; i++) {
                vtype[i] = GRB_CONTINUOUS;
            }
            break;
        default:
            printf("Error: Bad gurobi type for gurobiSolve()\n");
            return 0;
    }

    error = GRBaddvars(model, D3size, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
                                                                         /* ///////////alon - can we omit it for ILP?*/


    if (error) {
        printf("Error: GRBaddvars() has failed\n");
    }

    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
                                                                     /* /////////////////////////////// should it be always max?*/

    if (error) {
        printf("Error: GRBsetintattr() has failed\n");
    }

    error = GRBupdatemodel(model);
                                               /* //////////////////////////////////////////////////////////////////////// what is this?*/

    if (error) {
        printf("Error: GRBupdatemodel() has failed\n");
    }

    addCellsConstraint(model, board, ind, val);

    addRowConstraint( model, board, ind, val);

    addColConstraint( model, board, ind, val);

    addBlockConstraint( model,  board,  ind,  val);

    error = GRBoptimize(model);
    if (error) {
        printf("Error: GRBoptimize() has failed\n");
    }

    error = GRBwrite(model, "gurobiSolution.lp");
    if (error) {
        printf("Error: GRBwrite() has failed\n");
    }

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("Error: GRBgetintattr() has failed\n");
    }

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
/*    if (error) {
        printf("Error: GRBgetdblattr() has failed\n");
    }*/

    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, D3size, sol);
/*    if (error) {
        printf("Error: GRBgetdblattrarray() has failed\n");
    }*/

    if (optimstatus == GRB_OPTIMAL){
        switch (type) {
            case G_VALIDATE:
                updateSalvedBoard(board, sol);
                solution = 1;
                break;
            case G_GENERATE:
                updateSalvedBoard(board, sol);
                solution = 1;
                break;
            case G_HINT:
                updateSalvedBoard(board, sol);
                solution = board->foundSolution[x][y];
                break;
            case G_GUESS:
                solution = updateGuessSolution(board, sol, threshold);
                break;
            case G_GUESS_HINT:
                getGuessHintVals(board, sol, x, y);
                solution = 1;
                break;
        }
    }
    else {
        solution = 0;
    }

/*    PrintSol(size, sol);
    PrintFoundSolution(board);*/

    free(sol);
    free(ind);
    free(val);
    free(obj);
    free(vtype);

    GRBfreemodel(model);

    GRBfreeenv(env);

    return solution;
}

void updateSalvedBoard(Board* board, double* sol) {
    int i, j, v;
    int size = (board->bCol * board->bRow);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            for (v = 0; v < size; v++) {
                if (sol[(size * size) * i + size * j + v] > 0) {
                    board->foundSolution[i][j] = v + 1;
                }
            }
        }
    }
}

void getGuessHintVals(Board* board, double* sol, int x, int y) {
    int v;
    int size = (board->bCol * board->bRow);

    printf("Cell[%d][%d]:\n", x , y );
    for (v = 0; v < size; v++) {
        if (sol[(size*size)*x+size*y+v] > 0) {
            printf("Value:%d , score:%.2f\n", v + 1, sol[(size*size)*x+size*y+v]);
        }
    }
}

int updateGuessSolution(Board* board, double* sol, float threshold){
    int i, j, random, arrSize;
    int *randArr = NULL;
    int size = (board->bCol * board->bRow);
    Cell *currCell = NULL;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board->foundSolution[i][j]=0;
        }
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            currCell = getCell(i, j, board);
            if (getValueCell(currCell) == 0) {
                arrSize=size;
                randArr = findAboveThreshValuesArrCell(board, sol, &arrSize, i, j, threshold);
				if (arrSize!=0) {
                    random = rand() % arrSize;
                    if ( 1==checkCellNoErroneous(i,j,randArr[random],board,1)) {
                        board->foundSolution[i][j] = randArr[random];
                    }
				}
            } else{
                board->foundSolution[i][j]=getValueCell(currCell);
            }
        }
    }
    free(randArr);
    return 1;
}

int* findAboveThreshValuesArrCell(Board* board, double* sol ,int *arrSize, int i, int j, float threshold){
    int v,freg, start=0, counter=0;
	float score, aboveThreshSum=0.0;
    int size = (board->bCol * board->bRow);
    int* aboveThreshArrVals = NULL;
    int* randArr  = NULL;
    float* aboveThreshArrScores  = NULL;

    aboveThreshArrVals = (int*) malloc( size*sizeof(int));
    randArr = (int*) malloc(100*sizeof(int));
    aboveThreshArrScores = (float*) malloc( size*sizeof(float));
    if (!aboveThreshArrVals || !randArr || !aboveThreshArrScores){
        free(aboveThreshArrVals);
        free(aboveThreshArrScores);
        free(randArr);
        printf("Error: findAboveThreshValuesArrCell has failed\n");
        exit(0);
    }
    for(v=0; v<size; v++){
        score = sol[(size * size) * i + size * j + v];
        if(score >= threshold && checkCellNoErroneous(i, j, v+1, board,0)){
            aboveThreshArrVals[counter] = v+1;
            aboveThreshArrScores[counter] = score;
            counter++;
            aboveThreshSum+=score;
        }
    }


    for(v=0; v<counter; v++){
        freg = (int) 100*(aboveThreshArrScores[v]/ (float) aboveThreshSum);
        for (i=0; i<freg; i++){
            randArr[start+i]= aboveThreshArrVals[v];
        }
        start+=freg;
    }

    *arrSize=start;
    free(aboveThreshArrVals);
    free(aboveThreshArrScores);
    return randArr;
}

void printIntArr(int *arr,int size){
    int i;

    printf("[ ");
    for (i = 0; i < size-1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d]\n\n",arr[size-1]);
}

void cleanGurArr(int* arr, double* arr2, int size){
    int i;
    for (i = 0; i < size; i++) {
        arr[i]=0;
        arr2[i]=0.0;
    }
}

void PrintSol(int size, double* sol){
    int i;

    printf("7 sol:\n[");
    for (i = 0; i < size*size*size; i++) {
        if (i % size * size == 0) {
            printf("\n");
        }
        printf(" %.3f,", sol[i]);
    }
    printf("]\n");
}