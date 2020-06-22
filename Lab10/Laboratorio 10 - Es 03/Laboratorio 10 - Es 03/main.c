//
//  main.c
//  Laboratorio 10 - Es 03
//
//  Created by Davide Arcolini on 23/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limits.h"


#define DD 11   //Max difficulty allowed for a diagonal
#define DP 33   //Max difficulty allowed for a program
#define BONUS 1.5   //Bonus value
#define BONUS_DIFFICULTY 8  //Minimum difficulty of an element to be inserted in order to achieve the bonus
#define MAX_PROG_DIAGS 3
#define MAX_DIAG_ELEMENTS 5
#define MAX_STRING_NAME 100
#define MAX_FILENAME_LENGTH 30

/* --- DATA --- */

//DEFINITION OF AN ELEMENT
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

//SET OF ELEMENTS
typedef struct {
    info *elements; //Array of elements
    int N_elements; //Total number of elements
} data;

//DEFINITION OF A DIAGONAL
typedef struct {
    info elements[MAX_DIAG_ELEMENTS];   //A diagonal is composed by MAX_DIAG_ELEMENTS elements
    int N_elements;     //Number of elements in the diagonal
    float diag_value_tot; //A diagonal has a total value given by the sum of all elements' value
} diag_t;

//DEFINITION OF A PROGRAM
typedef struct {
    diag_t diags[MAX_PROG_DIAGS];   //A program is composed by MAX_PROG_DIAGS diagonals
    float prog_value_tot; //A program has a total value given by the sum of all diagonals' value
} prog_t;

//SPECIFIC VALUE FOR AN ELEMENT
typedef struct check_s {
    float specific_value;
    int type_FORWARD;
    int type_BACKWARD;
    int type_TRANS;
} check_t;

//GLOBAL VARIABLES STORING INFORMATION ABOUT LAST ELEMENT INSERTED
int FORWARD = 0;
int BACKWARD = 0;
int TRANS = 0;

//GLOABL VARIABLES STORING INFORMATION ABOUT BONUS
int BONUS_ACHIEVED = 0;
int BONUS_DIAG = -1;


/* --- FUNCTIONS --- */

//INPUT AND OUTPUT
data file_READ (char *filename);
void data_SHOW (data set);
void elements_COPY (info *element_DST, info element_SRC);

//CHECK IF AN ELEMENT IS FRONT OR BACK
int frontElement_CHECK (info element);

//COMPUTE THE TOTAL DIFFICULTY VALUE OF A DIAGONAL
int difficulty_COMPUTE (diag_t diag);

//INSERT BEST ELEMENT IN THE SOLUTION
int element_COMPUTE (data set, prog_t *solution, int row, int column, int difficulty_DIAG, int difficulty_TOT);

//COMPUTE SPECIFIC VALUES
check_t specific_FRONT_EVALUATE (info element);
check_t specific_TRANS_EVALUATE (info element_PREV, info element_NEXT);


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
    
    //SHOWING DATA
    data_SHOW(set);
    
    //SET OF SOLUTION
    prog_t solution;
    solution.prog_value_tot = 0;
    
    //VARIABLES USED
    int difficulty_MIN = INT_MAX;
    int difficulty_INDEX = 0;
    int difficulty_TOT = 0;
    int difficulty_DIAG = 0;
    int difficulty_CURRENT = 0;
    int value_MAX = INT_MIN;
    

    
    
    
    /* --- SEARCH THE FRONT ELEMENT THAT MAXIMIZE THE VALUE AND MINIMIZE THE DIFFICULTY --- */
    
    
    for (int i = 0; i < set.N_elements; i++)
        if (frontElement_CHECK(set.elements[i]))
            
            //MAXIMIZE VALUE
            if (set.elements[i].value >= value_MAX)
                
                //MINIMIZE DIFFICULTY
                if (set.elements[i].difficulty <= difficulty_MIN) {
                
                    //UPLOADING VALUES
                    difficulty_MIN = set.elements[i].difficulty;
                    value_MAX = set.elements[i].value;
                    difficulty_INDEX = i;
                }
    difficulty_TOT = set.elements[difficulty_INDEX].difficulty * MAX_PROG_DIAGS;
    
    
    //INSERTING THE BEST FRONT ELEMENT IN EVERY STARTING POSITION OF DIAGONALS
    for (int i = 0; i < MAX_PROG_DIAGS; i++) {
        elements_COPY(&solution.diags[i].elements[0], set.elements[difficulty_INDEX]);
        solution.diags[i].N_elements = 1;
    }
    
    
    
    
    
    
    
    
    
    /* --- INSERT BEST ELEMENT IN EVERY POSITION OF A DIAGONAL UNTIL IT'S FULL OR GO BEYOND THE LIMIT --- */
    
    //VISITING EVERY DIAGONAL
    for (int i = MAX_PROG_DIAGS-1; i >= 0; i--) {
        
        //COMPUTING THE DIFFICULTY VALUE OF THE CURRENT DIAGONAL
        difficulty_DIAG = difficulty_COMPUTE(solution.diags[i]);
        
        //VISITING EVERY POSITION
        for (int j = 1; j < MAX_DIAG_ELEMENTS; j++) {
            
            //INSERTING ELEMENT
            difficulty_CURRENT = element_COMPUTE(set, &solution, i, j, difficulty_DIAG, difficulty_TOT);
            
            //UPLOADING LIMIT VALUES
            if (difficulty_CURRENT != -1) {
                difficulty_DIAG += difficulty_CURRENT;
                difficulty_TOT += difficulty_CURRENT;
            }
            else
                break;
        }
    }
    
    
    
    
    
    /* --- SHOWING RESULTS --- */
    
    for (int i = 0; i < MAX_PROG_DIAGS; i++) {
        fprintf(stdout, "DIAGONAL %d: \n", i+1);
        
        //SHOWING DIAGONAL
        for (int j = 0; j < solution.diags[i].N_elements; j++)
            fprintf(stdout, "\t%s\n", solution.diags[i].elements[j].name);
        
        //COMPUTING VALUE
        float value = 0.0;
        for (int k = 0; k < solution.diags[i].N_elements; k++)
            value += solution.diags[i].elements[k].value;
        
        //ADDING BONUS
        if (BONUS_ACHIEVED == 1 && BONUS_DIAG == i) {
            value = value * BONUS;
            fprintf(stdout, "\nVALUE (--- BONUS ---): %.2f\n\n", value);
        }
        else
            fprintf(stdout, "\nVALUE: %.2f\n\n", value);
    }
    
    
    
    /* --- FREE --- */
    free(set.elements);
    
    exit(EXIT_SUCCESS);
}


/* --- FUNCTIONS --- */

//NPUT AND OUTPUT
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
    
    fprintf(stdout, "THIS IS THE WHOLE SET OF ELEMENTS:\n");
    for (int i = 0; i < set.N_elements; i++)
        fprintf(stdout, "%s %d %d %d %d %d %.2f %d\n", set.elements[i].name, set.elements[i].type, set.elements[i].input, set.elements[i].output, set.elements[i].start, set.elements[i].end, set.elements[i].value, set.elements[i].difficulty);
    
    fprintf(stdout, "\n\n");
    
}
void elements_COPY (info *element_DST, info element_SRC) {
    
    strcpy(element_DST->name, element_SRC.name);
    element_DST->type = element_SRC.type;
    element_DST->input = element_SRC.input;
    element_DST->output = element_SRC.output;
    element_DST->start = element_SRC.start;
    element_DST->end = element_SRC.end;
    element_DST->value = element_SRC.value;
    element_DST->difficulty = element_SRC.difficulty;
    
}

//CHECK IF AN ELEMENT IS FRONT OR BACK
int frontElement_CHECK (info element) {
    return element.input;
}

//COMPUTE THE TOTAL DIFFICULTY VALUE OF A DIAGONAL
int difficulty_COMPUTE (diag_t diag) {
    int sum = 0;
    for (int i = 0; i < diag.N_elements; i++)
        sum += diag.elements[i].difficulty;
    
    return sum;
}

//COMPUTE
int element_COMPUTE (data set, prog_t *solution, int N_diag, int N_element, int difficulty_DIAG, int difficulty_TOT) {
    
    //CREATING BEST ELEMENT
    check_t best_element;
    
    //INITIALIZING BEST ELEMENT
    best_element.specific_value = -1.0;
    best_element.type_FORWARD = 0;
    best_element.type_BACKWARD = 0;
    best_element.type_TRANS = 0;
    
    //INDEX VALUE
    int index = -1;
    
    //EVALUATING EVERY ELEMENT
    for (int i = 0; i < set.N_elements; i++) {
            
        //IF WE CAN CHOOSE THAT ELEMENT
        if ((difficulty_DIAG + set.elements[i].difficulty <= DD) && (difficulty_TOT + set.elements[i].difficulty <= DP)) {
            
            //CHECKING IF THE ELEMENT WE WANT TO INSERT IS BETTER THAN THE PREVIOUS ONE
            check_t element_TMP = specific_FRONT_EVALUATE(set.elements[i]);
            if (element_TMP.specific_value > best_element.specific_value) {
                
                //CHECKING LAST ELEMENT COMPATIBILITY
                if (FORWARD == 0) {
                    if (element_TMP.type_FORWARD == 1) {
                        
                        //COPYING ELEMENTS VALUE
                        best_element.specific_value = element_TMP.specific_value;
                        best_element.type_FORWARD = element_TMP.type_FORWARD;
                        best_element.type_BACKWARD = element_TMP.type_BACKWARD;
                        best_element.type_TRANS = element_TMP.type_TRANS;
                        
                        //ASSIGNING INDEX
                        index = i;
                    }
                }
                else if (BACKWARD == 0) {
                    if (element_TMP.type_BACKWARD == 1) {
                        
                        //COPYING ELEMENTS VALUE
                        best_element.specific_value = element_TMP.specific_value;
                        best_element.type_FORWARD = element_TMP.type_FORWARD;
                        best_element.type_BACKWARD = element_TMP.type_BACKWARD;
                        best_element.type_TRANS = element_TMP.type_TRANS;
                        
                        //ASSIGNING INDEX
                        index = i;
                    }
                    
                }
                else if (TRANS == 0) {
                    element_TMP = specific_TRANS_EVALUATE((*solution).diags[N_diag].elements[N_element-1], set.elements[i]);
                    if (element_TMP.type_TRANS == 1) {
                        
                        //COPYING ELEMENTS VALUE
                        best_element.specific_value = element_TMP.specific_value;
                        best_element.type_FORWARD = element_TMP.type_FORWARD;
                        best_element.type_BACKWARD = element_TMP.type_BACKWARD;
                        best_element.type_TRANS = element_TMP.type_TRANS;
                        
                        //ASSIGNING INDEX
                        index = i;
                    }
                }
                else {
                    if (element_TMP.specific_value > best_element.specific_value) {
                        
                        //COPYING ELEMENTS VALUE
                        best_element.specific_value = element_TMP.specific_value;
                        best_element.type_FORWARD = element_TMP.type_FORWARD;
                        best_element.type_BACKWARD = element_TMP.type_BACKWARD;
                        best_element.type_TRANS = element_TMP.type_TRANS;
                        
                        //ASSIGNING INDEX
                        index = i;
                    }
                }
                
            }
            
        }
    }
    
    //UPLOADING GLOBAL VARIABLES
    if (best_element.type_FORWARD == 1)
        FORWARD = 1;
    else if (best_element.type_BACKWARD == 1)
        BACKWARD = 1;
    if (best_element.type_TRANS == 1)
        TRANS = 1;
    
    //ASSIGNING ELEMENT TO THE SOLUTION
    if (index != -1) {
        
        //VERYFING BONUS
        if (set.elements[index].difficulty >= 8) {
            BONUS_ACHIEVED = 1;
            BONUS_DIAG = N_diag;
        }
        
        elements_COPY(&(*solution).diags[N_diag].elements[N_element], set.elements[index]);
        (*solution).diags[N_diag].N_elements++;
        
        return set.elements[index].difficulty;
    }
    
    return -1;
}
check_t specific_FRONT_EVALUATE (info element) {
    
    //COMPUTING SPECIFIC VALUES FOR THE ELEMENT
    check_t element_TMP;
    element_TMP.specific_value = element.value / element.difficulty;
    element_TMP.type_FORWARD = 0;
    element_TMP.type_BACKWARD = 0;
    element_TMP.type_TRANS = 0;
    
    //ASSIGNING INFORMATION
    if (element.type == 2)
        element_TMP.type_FORWARD = 1;  //SETTING TRUE VALUE
    if (element.type == 1)
        element_TMP.type_BACKWARD = 1;  //SETTING TRUE VALUE
    
    return element_TMP;
}
check_t specific_TRANS_EVALUATE (info element_PREV, info element_NEXT) {
    
    //COMPUTING SPECIFIC VALUES FOR THE ELEMENT
    check_t element_TMP;
    element_TMP.specific_value = element_NEXT.value / element_NEXT.difficulty;
    element_TMP.type_FORWARD = 0;
    element_TMP.type_BACKWARD = 0;
    element_TMP.type_TRANS = 0;
    
    if (element_NEXT.type == 2)
        element_TMP.type_FORWARD = 1;  //SETTING TRUE VALUE
    if (element_NEXT.type == 1)
        element_TMP.type_BACKWARD = 1;  //SETTING TRUE VALUE
    if (element_PREV.output == element_NEXT.input)
        element_TMP.type_TRANS = 1; //SETTING TRUE VALUE
        
    return element_TMP;
}

