//
//  main.c
//  Laboratorio 10 - Es 02
//
//  Created by Davide Arcolini on 09/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define DD 11   //Max difficulty allowed for a diagonal
#define DP 33   //Max difficulty allowed for a program
#define MAX_PROG_DIAGS 3
#define MAX_DIAG_ELEMENTS 5
#define MAX_STRING_NAME 100
#define MAX_FILENAME_LENGTH 30

/* --- DATA --- */

//Definition of an element
typedef struct {
    char name[MAX_STRING_NAME]; //Name of the element
    int type;   //Type of the element. 2 if for forward movement, 1 is for backward movement and 0 is for transition movement
    int input;  //Input direction of the element. 1 is for front input direction, 0 is for back input direction
    int output; //Output direction of the element. 1 is for front output direction, 0 is for back output direction
    int start;   //Priority access. 1 is for an element that has to have an element before, 0 is for an element that does not need any elements before
    int end;    //Priority exit. 1 is for an element that has to be the last one, 0 is for an element that cn be followed by other elements
    float value;    //Value of the element
    int difficulty; //Difficulty of the element
} info;

//Set of elements
typedef struct {
    info *elements; //Array of elements
    int N_elements; //Total number of elements
} data;

//Definition of a diagonal
typedef struct {
    info elements[MAX_DIAG_ELEMENTS];   //A diagonal is composed by MAX_DIAG_ELEMENTS elements
    int diag_value_tot; //A diagonal has a total value given by the sum of all elements' value
} diag_t;

//Definition of a program
typedef struct {
    diag_t diags[MAX_PROG_DIAGS];   //A program is composed by MAX_PROG_DIAGS diagonals
    int prog_value_tot; //A program has a total value given by the sum of all diagonals' value
} prog_t;

prog_t best_program;

/* --- FUNCTIONS --- */

//Input and output
data file_READ (char *filename);
void data_SHOW (data set);

//Solution main
void solution_COMPUTE (data set);
void solution_SHOW (prog_t program);

//Recursive functions
int diags_COMPUTE (data set, diag_t *diags);
int diags_COMPUTE_R (data set, diag_t *diags, diag_t *diag_TMP, int recursiveLevel, int count);

int main(int argc, const char *argv[]) {
    
    //CHECKING TERMINAL PARAMETERS
    if (argc != 1) {
        fprintf(stderr, "ERROR: insert only program name!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING FILE NAME
    char filename[MAX_FILENAME_LENGTH] = "elementi.txt";
    //fprintf(stdout, "Insert FILE name: ");
    //fscanf(stdin, "%s", filename);
    
    //STORING DATA FROM FILE
    data set = file_READ(filename);
    
    //COMPUTING BEST PROGRAM SOLUTION
    solution_COMPUTE(set);
    
    //SHOW BEST PROGRAM
    solution_SHOW(best_program);
    
    //FREE
    
    
    exit(EXIT_SUCCESS);
    
    
}

//Input and output
data file_READ (char *filename) {
    
    data set;
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s", filename);
        exit(EXIT_FAILURE);
    }
    
    //DYNAMIC ALLOCATION OF THE ARRAY STORING ELEMENTS
    fscanf(fp, "%d", &set.N_elements);
    set.elements = (info*) malloc(set.N_elements * sizeof(info));
    if (set.elements == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READING DATA FROM FILE
    for (int i = 0; i < set.N_elements; i++)
        fscanf(fp, "%s %d %d %d %d %d %f %d", set.elements[i].name, &set.elements[i].type, &set.elements[i].input, &set.elements[i].output, &set.elements[i].start, &set.elements[i].end, &set.elements[i].value, &set.elements[i].difficulty);
    
    //CLOSING FILE
    fclose(fp);
    
    return set;
    
}
void data_SHOW (data set) {
    for (int i = 0; i < set.N_elements; i++)
        fprintf(stdout, "%s %d %d %d %d %d %f %d\n", set.elements[i].name, set.elements[i].type, set.elements[i].input, set.elements[i].output, set.elements[i].start, set.elements[i].end, set.elements[i].value, set.elements[i].difficulty);
}

//Solution main
void solution_COMPUTE (data set) {
    
    //DYNAMIC ALLOCATION FOR ARRAY OF DIAGONALS
    diag_t *diags = NULL;
    diags = (diag_t*) malloc(1 * sizeof(diag_t));
    if (diags == NULL) {
        fprintf(stderr, "ERROR: some errors occured while trying to stare data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //CREATING ALL POSSIBLE DIAGONALS AND COUNTING THEM
    int N_diags = diags_COMPUTE(set, diags);
    
    
}
void solution_SHOW (prog_t program) {
    fprintf(stdout, "This is the best program the athlete can compose:\n\n");
    fprintf(stdout, "TOTAL VALUE OF THE PROGRAM: %d\n", program.prog_value_tot);
    fprintf(stdout, "DIAGONALS: \n\n");
    for (int i = 0; i < MAX_PROG_DIAGS; i++) {
        fprintf(stdout, "\t[DIAGONAL %d - VALUE: %d]\n", i+1, program.diags[i].diag_value_tot);
        for (int j = 0; j < MAX_DIAG_ELEMENTS; j++) {
            fprintf(stdout, "\t\t[ELEMENT %d]: %s %d %d %d %d %d %f %d\n", j+1, program.diags[i].elements[j].name, program.diags[i].elements[j].type, program.diags[i].elements[j].input, program.diags[i].elements[j].output, program.diags[i].elements[j].start, program.diags[i].elements[j].end, program.diags[i].elements[j].value, program.diags[i].elements[j].difficulty);
        }
    }
}

//Recursive functions
int diags_COMPUTE (data set, diag_t *diags) {
    diag_t diag_TMP;
    
    //COMPUTING DIAGONALS SET
    return diags_COMPUTE_R(set, diags, &diag_TMP, 0, 0);
}
int diags_COMPUTE_R (data set, diag_t *diags, diag_t *diag_TMP, int recursiveLevel, int count) {
    
    //TERMINAL CONDITION
    if (recursiveLevel > MAX_DIAG_ELEMENTS)
        return count;
    
    //INSERT OF AN ELEMENT IN DIAG_TMP
    for (int i = 0; i < set.N_elements; i++) {
        element_COPY(diag_TMP->elements[recursiveLevel], set.elements[i]);
    }
}
