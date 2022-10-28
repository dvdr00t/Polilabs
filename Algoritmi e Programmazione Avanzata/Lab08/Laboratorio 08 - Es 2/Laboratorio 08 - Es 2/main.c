//
//  main.c
//  Laboratorio 08 - Es 2
//
//  Created by Davide Arcolini on 11/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 50

/* ----  DATA  ---- */

//TUBE STRUCT FOR A TILE
typedef struct {
    char color;
    int value;
} tube_t;

//TILE STRUCT WITH TWO TUBES
typedef struct {
    tube_t tube_horz;
    tube_t tube_vert;
} tiles_t;

//BOARD STRUCT WITH INFORMATIONS (ROWS AND COLUMNS)
typedef struct {
    tiles_t **board;
    int rows;
    int columns;
} board_t;

int N_tiles;    //Number of tiles available
int bestValue = 0;  //Best value computed on every iteration of recursive function. It is initialize to zero in order to be sure that the first value computed will be greater than it




/* ----  FUNCTIONS  ---- */

//SOLUTION FUNCTIONS

//Main recursive functions, it fills the board with every tiles available trying the two different positions (with rotation and without rotation). It creates a powerset of solution (TMP_board) that will be avaluated by a function in order to knoe the value of the board. If it is greater than the best value, it will upload the bestBoard
void computeBestBoard_R (board_t *bestBoard, board_t board, board_t *TMP_board, tiles_t *tiles, int *chosen, int recursionLevel);
void computeBestBoard (board_t *bestBoard, board_t board, tiles_t *tiles, int *chosen); //Wrapper function
int computeBoardValue (board_t *board); //Computes the value of a particular board

//INPUT FUNCTIONS
tiles_t* readTILES (void);  //Input function that stores data (tiles) in memory from FILE
board_t readBOARD (tiles_t *tiles, int *chosen);    //Input function that stores data (starting board) in memory

//OUTPUT FUNCTIONS
void showBOARD (board_t board); //Show a board composition

//COPY FUNCTIONS
void copyBOARD (board_t *board_DEST, board_t board);  //Copies a board in another one
void copyTILE (tiles_t *tile_DEST, tiles_t *tile_SRC, int rotation);  //Copies the SOURCE tile in the DESTINATION tile

int main(int argc, const char *argv[]) {
    
    //CHECKING TERMINAL PARAMETERS
    if (argc != 1){
        fprintf(stderr, "ERROR: insert only program name!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //VARIABLES USED
    tiles_t *tiles = NULL;  //Set of tiles
    board_t board;  //Board
    board_t bestBoard;  //Solution of the problem
    int *chosen = NULL; //Marks if we have taken a tile or not, 0 is still avaiable, 1 is taken

    //READING TILES, STORING THEM IN MEMORY AND UPLOADING TOTAL NUMBER OF TILES
    tiles = readTILES();
    
    //Dynamic allocation of space in memory for chosen array
    chosen = (int*) calloc(N_tiles, sizeof(int));
    if (chosen == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to allocate space in memory!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING STARTING BOARD AND STORE IT IN MEMORY
    board = readBOARD(tiles, chosen);
    printf("Board layout: [Horizontal.Color  Horizontal.Value  Vertical.Color  Vertical.Value]\n\n");
    printf("This is the starting board:\n\n");
    showBOARD(board);
    printf("\n\n");
    
    //Dynamic allocation for bestBoard
    bestBoard.board = (tiles_t **) malloc(board.rows * sizeof(tiles_t*));
    if (bestBoard.board == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to allocate space in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < board.rows; i++){
        bestBoard.board[i] = (tiles_t*) malloc(board.columns * sizeof(tiles_t));
        if (bestBoard.board[i] == NULL){
            fprintf(stderr, "ERROR: some errors occurred while trying to allocate space in memory.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //COMPUTING BEST BOARD
    computeBestBoard(&bestBoard, board, tiles, chosen);
    
    //SHOWING BEST BOARD
    printf("This is the best board with a best value %d:\n\n", bestValue);
    showBOARD(bestBoard);
    printf("\n\n");
    
    //DEALLOCATING SPACE IN MEMORY
    free(bestBoard.board);
    free(board.board);
    
    exit(EXIT_SUCCESS);
}


/* ----  FUNCTIONS  ---- */

//SOLUTION FUNCTIONS
int computeBoardValue (board_t *board) {
    int i, j;
    int value_tmp;
    int value_tot = 0;
    
    //COMPUTING VALUES THROUGH THE ROWS
    for (i = 0; i < (*board).rows; i++) {
        j = 0;
        value_tmp = 0;
        while (j+1 < (*board).columns && (*board).board[i][j].tube_horz.color == (*board).board[i][j+1].tube_horz.color) {
            value_tmp = value_tmp + (*board).board[i][j].tube_horz.value;
            ++j;
        }
        
        if (j == (*board).columns - 1){
            value_tmp = value_tmp + (*board).board[i][j].tube_horz.value;
            value_tot = value_tot + value_tmp;
        }
    }
    
    //COMPUTING VALUES THROUGH THE COLUMNS
    for (j = 0; j < (*board).columns; j++) {
        i = 0;
        value_tmp = 0;
        while (i+1 < (*board).rows && (*board).board[i][j].tube_vert.color == (*board).board[i+1][j].tube_vert.color) {
            value_tmp = value_tmp + (*board).board[i][j].tube_vert.value;
            ++i;
        }
        
        if (i == (*board).rows - 1){
            value_tmp = value_tmp + (*board).board[i][j].tube_vert.value;
            value_tot = value_tot + value_tmp;
        }
    }
    
    return value_tot;
    
}
void computeBestBoard (board_t *bestBoard, board_t board, tiles_t *tiles, int *chosen) {
    int i;
    
    //Dynamic allocation for TMP_solution that will be filled by the recursive function
    board_t TMP_board;
    TMP_board.board = (tiles_t **) malloc(board.rows * sizeof(tiles_t*));
    if (TMP_board.board == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to allocate space in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < board.rows; i++){
        TMP_board.board[i] = (tiles_t*) malloc(board.columns * sizeof(tiles_t));
        if (TMP_board.board[i] == NULL){
            fprintf(stderr, "ERROR: some errors occurred while trying to allocate space in memory.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    TMP_board.rows = board.rows;
    TMP_board.columns = board.columns;
    
    //COMPUTING SOLUTION
    computeBestBoard_R(bestBoard, board, &TMP_board, tiles, chosen, 0);
    
}
void computeBestBoard_R (board_t *bestBoard, board_t board, board_t *TMP_board, tiles_t *tiles, int *chosen, int recursionLevel){
    int i, j, k;
    
    //TERMINAL CONDITION (we have visited every slot in the board)
    if (recursionLevel >= board.rows * board.columns) {
        
        //COMPUTING VALUE OF BOARD
        int value = computeBoardValue(TMP_board);
        
        //UPLOADING SOLUTION
        if (value > bestValue){
            copyBOARD (bestBoard, *TMP_board);
            bestValue = value;
        }
        return;
    }
    
    //COMPUTING INDICES ON EVERY ITERATION
    i = recursionLevel / board.columns;
    j = recursionLevel % board.columns;
    
    //CHECKING THE SLOT ON THE BOARD WITH RECURSION INDICES
    //IF IT IS AN EMPTY SLOT
    if (board.board[i][j].tube_horz.color == 'X') {
        
        //VISITING ALL TILES IN ORDER TO CHOOSE THE FIRST ONE AVAILABLE
        for (k = 0; k < N_tiles; k++){
            
            //IF THE TILE IS AVAILABLE
            if (chosen[k] == 0) {
                
                //TAKING THE TILE...
                chosen[k] = 1;
                
                //...WITH NO ROTATION
                copyTILE(&TMP_board->board[i][j], &tiles[k], 0);
                computeBestBoard_R(bestBoard, board, TMP_board, tiles, chosen, recursionLevel+1);
                
                //...WITH ROTATION
                copyTILE(&TMP_board->board[i][j], &tiles[k], 1);
                computeBestBoard_R(bestBoard, board, TMP_board, tiles, chosen, recursionLevel+1);
                
                //BACKTRACKING
                chosen[k] = 0;
            }
        }
    }
    //IF THE SLOT IS ALREADY SET
    else {
        copyTILE(&TMP_board->board[i][j], &board.board[i][j], 0);
        computeBestBoard_R(bestBoard, board, TMP_board, tiles, chosen, recursionLevel+1);
    }
    
    //RETURN
    return;
}

//INPUT FUNCTIONS
tiles_t* readTILES (void) {
    
    //FILE OPENING AND CHECKING
    FILE *fp;
    char filename[MAX_FILENAME_LENGTH] = "tiles.txt";
    //printf("Insert tiles' file name: ");
    //scanf("%s", filename);
    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s", filename);
        exit(EXIT_FAILURE);
    }
    
    //READING DATA
    tiles_t *tiles = NULL;
    
    //Reading number of tiles
    fscanf(fp, "%d\n", &N_tiles);
    
    //Dynamic allocation of space in memory for tiles array of struct
    tiles = (tiles_t*) malloc(N_tiles * sizeof(tiles_t));
    if (tiles == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory from FILE.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Reading data from file and storing them in the array of struct
    int i = 0;
    while (fscanf(fp, "%c %d %c %d\n", &tiles[i].tube_horz.color, &tiles[i].tube_horz.value, &tiles[i].tube_vert.color, &tiles[i].tube_vert.value) != EOF){
        i++;
    }
    
    //CLOSING FILE
    fclose(fp);
    
    return tiles;
}
board_t readBOARD (tiles_t *tiles, int *chosen) {
    
    //FILE OPENING AND CHECKING
    FILE *fp;
    char filename[MAX_FILENAME_LENGTH] =  "board.txt";
    //printf("Insert board's file name: ");
    //scanf("%s", filename);
    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s", filename);
        exit(EXIT_FAILURE);
    }
    
    //READING DATA
    board_t board;
    int i, j;
    int rotation;
    int index;
    
    //Reading dimensions and dynamic allocation of space in memory
    fscanf(fp, "%d %d\n", &board.rows, &board.columns);
    board.board = (tiles_t **) malloc(board.rows * sizeof(tiles_t *));
    if (board.board == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data from FILE");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < board.rows; i++) {
        board.board[i] = (tiles_t *) malloc(board.columns * sizeof(tiles_t));
        if (board.board[i] == NULL){
            fprintf(stderr, "ERROR: some errors occurred while trying to store data from FILE");
            exit(EXIT_FAILURE);
        }
    }
    
    //Inserting tiles already existing in the board
    for (i = 0; i < board.rows; i++){
        for (j = 0; j < board.columns; j++){
            
            fscanf(fp, "%d/%d ", &index, &rotation);
            
            //Empty tile slot
            if (index == -1) {
                board.board[i][j].tube_horz.color = 'X';
                board.board[i][j].tube_horz.value = -1;
                board.board[i][j].tube_vert.color = 'X';
                board.board[i][j].tube_vert.value = -1;
            }
            
            //Tile slot
            else {
                
                //Marking this tile as chosen from the array of tiles. It cannot be used in future
                chosen[index] = 1;
                
                if (rotation == 0)
                    copyTILE(&board.board[i][j], &tiles[index], rotation);
                else
                    copyTILE(&board.board[i][j], &tiles[index], rotation);
            }
        }
    }
    
    
    //CLOSING FILE
    fclose(fp);
    
    return board;
}

//OUTPUT FUNCTIONS
void showBOARD (board_t board){
    int i, j;
    
    for (i = 0; i < board.rows; i++){
        for (j = 0; j < board.columns; j++){
            printf("[%c %d %c %d]  ", board.board[i][j].tube_horz.color, board.board[i][j].tube_horz.value, board.board[i][j].tube_vert.color, board.board[i][j].tube_vert.value);
        }
        printf("\n");
    }
}

//COPY FUNCTIONS
void copyBOARD (board_t *board_DEST, board_t board) {
    int i, j;
    
    //COPYING BOARD TILES (COLORS AND VALUES)
    for (i = 0; i < board.rows; i++)
        for (j = 0; j < board.columns; j++)
            copyTILE(&board_DEST->board[i][j], &board.board[i][j], 0);
    
    //COPYING BOARD DIMENSIONS
    board_DEST->rows = board.rows;
    board_DEST->columns = board.columns;
    
}
void copyTILE (tiles_t *tile_DEST, tiles_t *tile_SRC, int rotation) {
    
    if (rotation) {
        tile_DEST->tube_horz.color = tile_SRC->tube_vert.color;
        tile_DEST->tube_horz.value = tile_SRC->tube_vert.value;
        tile_DEST->tube_vert.color = tile_SRC->tube_horz.color;
        tile_DEST->tube_vert.value = tile_SRC->tube_horz.value;
    }
    else {
        tile_DEST->tube_horz.color = tile_SRC->tube_horz.color;
        tile_DEST->tube_horz.value = tile_SRC->tube_horz.value;
        tile_DEST->tube_vert.color = tile_SRC->tube_vert.color;
        tile_DEST->tube_vert.value = tile_SRC->tube_vert.value;
    }
}
