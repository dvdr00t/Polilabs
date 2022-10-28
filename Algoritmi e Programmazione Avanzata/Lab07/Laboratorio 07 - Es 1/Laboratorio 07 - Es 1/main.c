//
//  main.c
//  Laboratorio 07 - Es 1
//
//  Created by Davide Arcolini on 30/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILENAME_LENGTH 30
#define N_JEWELS 4

//Struct which stores all data for every test
typedef struct {
    int amount[N_JEWELS];   //Amount of jewels avaiable. (.amount[0] is for the first jewel (zapphire), .amount[1] is for the second jewel (rubie) and so on...)
    int tot;    //Total number of jewels in the set
} data;

//The idea here is to notice that, given one for every jewels, we can terminate the necklace with every different jewels (retS, etsR, sreT or tsrE are all allowed). This leads us to the point in which we decide to reduce the amount of every jewels by the minimum amount of the set. So that, if we have a set like [10, 12, 11, 16] we can reduce it to a set like [0, 2, 1, 6], computing the necklace and add in the very begging of the neclace a string of jewels (retS, etsR, sreT or tsrE) according to the starting one in the necklace
int improvingAlgorithm (data *set);
int computeSET (data *set, int *choices, int k);    //Wrapper for recursive function. K is, for every iteration, the best lenght we can expect to get
int computeSET_R (int *amount, int *choices, int recursionLevel, int k, int pos);   //Recursive function used to compute best set of solution. It returns 1 if the solution we have found is long enough for what we are looking for, else 0
int checkSolution (int *choices, int actual, int previous); //Checks if the new jewel we want to insert in the solution set is allowed or not. If yes, it proceeds by adding that jewel to the solution, if not, it skips to the next one
void readSET(FILE *fp, data *set);  //Acquisition of data from FILE
int computeTotSET (data set);   //Computes the total amount of jewels
void printSET (data set, int min);   //Prints information about the particular set of the test
void checkFILE (FILE *fp, char *filename);
int main(int argc, const char * argv[]) {
    
    //Checking command line parameters
    if (argc != 2){
        fprintf(stderr, "ERROR: insert only program and FILE name.\n\n");
        exit(EXIT_FAILURE);
    }
    
    char filename[MAX_FILENAME_LENGTH];
    strcpy(filename, argv[1]);
    data set;   //Stores all data from FILE for every test
    int *choices = NULL;    //Stores in every position a number between 0 and N_JEWELS in order to indicate that, in that position, there will be a particular jewel due to the number stored. 0 for zapphire, 1 for rubie and so on...
    int test_number;    //Stores number of tests in the FILE
    int i = 0, j = 0;  //Counters
    char jewels[N_JEWELS] = {'S', 'R', 'T', 'E'};   //Used to print necklace composition, using choices[] as a selector for which jewel has to be printed
    int min;    //Used to improve speed of the algorithm
    
    //FILE opening
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    checkFILE(fp, filename);
    fscanf(fp, "%d", &test_number);
    
    
    //Running TESTS from FILE
    for (i = 0; i < test_number; i++){
        //Acquisition of data
        readSET(fp, &set);
        //Improving algorithm
        min = improvingAlgorithm(&set);
        //Printing information about the particular set
        printf("TEST #%d\n", i+1);
        printSET(set, min);
        
        //Dynamic allocation for solution array. We can't know in advance the maximum length of the necklace but we know it can be no longer than the total amount of jewels avaiable
        choices = (int*) calloc(set.tot, sizeof(int));
        if (choices == NULL){
            fprintf(stderr, "ERROR: some errors occured while trying to compute results for the test.\n\n");
            exit(EXIT_FAILURE);
        }
        
        //The idea here is to look for the best solution moving from the total amount of jewel avaible and decreasing the possible solution length of 1 for every iteration. So, if we can't find a solution "tot" long, we go looking for a "tot-1" long solution, and so on...
        for (j = set.tot; j > 0; j--)
            if (computeSET(&set, choices, j))   //Due to that, when we find a solution, we know it has to be the longest avaiable
                break;                          //So we stop looking and print the result
        
        //Printing the result (improved algorithm)
        printf("Max necklace length: %d\n", j + (N_JEWELS * min));
        printf("Necklace composition: ");
        
        //Printing part of the necklace cut by the improved algorithm. Depending on the first jewel in the necklace, we can have two different composition
        int k;  //Used to count from zero to the min value, because we have to add "min" string in the very beginning of the necklace
        if (choices[0] == 0 || choices[0] == 1)
            for (k = 0; k < min; k++)
                printf("%c%c%c%c", jewels[2], jewels[1], jewels[3], jewels[0]);
        if (choices[0] == 2 || choices[0] == 3)
            for (k = 0; k < min; k++)
                printf("%c%c%c%c", jewels[3], jewels[2], jewels[0], jewels[1]);
    
        //Printing the rest of the necklace
        for (k = 0; k < j; k++)
            printf("%c", jewels[choices[k]]);
        printf(".\n\n");
        
        //Deallocating space in memory for next test
        free(choices);
    }
    fclose(fp);
    
    exit(EXIT_SUCCESS);
}
int improvingAlgorithm(data *set){
    int min;    //Stores the minimum amount of jewels for the particular test
    int i;
    
    //Computing the minimum value
    min = set->amount[0];
    for (i = 1; i < N_JEWELS; i++)
        if (set->amount[i] < min)
            min = set->amount[i];
    
    //Decreasing the amount of jewels avaiable
    for (i = 0; i < N_JEWELS; i++)
        set->amount[i] = set->amount[i] - min;
    set->tot = set->tot - (N_JEWELS * min);
    
    //Saving min value by returning it to the main
    return min;
}

//Wrapper function used to copy original amounts of jewels for a particular test
int computeSET (data *set, int *choices, int k){
    int i;
    
    //Copy of the orginal amounts that will be modified by the recursive function
    int amount[N_JEWELS];
    for (i = 0; i < N_JEWELS; i++)
        amount[i] = set->amount[i];
    
    //Main function
    return computeSET_R(amount, choices, 0, k, -1);
    
}
int computeSET_R (int *amount, int *choices, int recursionLevel, int k, int pos){
    int i;
    
    //Terminal condition, we have found a solution long enough (if we were looking for a solution "tot" long, k is "tot". If we were looking for a solution "tot-1" long, k is "tot-1" and so on...)
    if (recursionLevel >= k)
        return 1;
    
    //Choosing for every position one of the N_JEWELS jewels.
    for (i = 0; i < N_JEWELS; i++){
        
        //If we did not run out of that jewel, we can take that
        if (amount[i] > 0){
            
            //If we are not in the first instance of recursion (so taking the first jewel) when all the choices are good, we check if the current jewel we have in mind to use is allowed by the problem or not
            if (pos != -1){
                if (checkSolution(choices, i, pos) == 0)
                    //If not, we move to the next jewel
                    continue;
            }
            
            //After having checked, we can now take that element
            amount[i]--;
            choices[recursionLevel] = i;    //Inserting jewel in the solution array
            
            //Recursively move to the next position in the necklace
            if (computeSET_R(amount, choices, recursionLevel+1, k, recursionLevel))
                return 1;   //Returning 1 if the solution is valid
            
            //Backtracking, we know have in mind to take a different jewel so we re-add the current one and move to the next one
            amount[i]++;
        }
    }
    
    return 0;   //Returning zero if the solution has not reached the length we were looking for
}
//Check if the current jewel we want to add in the necklace is allowed by the problem or not.
int checkSolution (int *choices, int actual, int previous){
    if (((choices[previous] == 0) || (choices[previous] == 2)) && (actual == 2 || actual == 3))
        return 0;
    if (((choices[previous] == 3) || (choices[previous] == 1)) && (actual == 1 || actual == 0))
        return 0;
    
    return 1;   //Solution is valid
}
//Acquisition of data
void readSET(FILE *fp, data *set){
    int i;
    for (i = 0; i < N_JEWELS; i++)
        fscanf(fp, "%d", &set->amount[i]);
    
    set->tot = computeTotSET( *set);
}
void checkFILE (FILE *fp, char *filename){
    if (fp == NULL){
        fprintf(stderr, "[ERROR]\nSome errors occurred while trying to open FILE: %s.\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
void printSET (data set, int min){
    printf("Sapphires: %d - ", set.amount[0] + min);
    printf("Rubies: %d - ", set.amount[1] + min);
    printf("Topaz: %d - ", set.amount[2] + min);
    printf("Emeralds: %d - ", set.amount[3] + min);
    
    //ADD here new jewels
    
    printf("TOT: %d\n", set.tot + (N_JEWELS * min));
    
}
int computeTotSET (data set){
    int i;
    int result = 0;
    for (i = 0; i < N_JEWELS; i++)
        result = result + set.amount[i];
    return result;
}
