//
//  main.c
//  Laboratorio 07 - Es 2
//
//  Created by Davide Arcolini on 03/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILENAME_LENGTH 30
#define N_JEWELS 4

//Struct which stores information about every jewel
typedef struct {
    int amount;
    int value;
} item;

//Struct which stores all data for every test
typedef struct {
    item jewel[N_JEWELS];
    int max_rip;
    int tot;
} data;

int computeValue(data *set, int *choices, int n);   //Computes the total value of a necklace in the very end of the recursion, when we the necklace is completed
int computeSET (data *set, int *choices, int *bestvalue, int *bestchoices, int k);    //Wrapper for recursive function, k is for the necklace length we are looking for
int computeSET_R (data *set, int *amount, int *choices, int recursionLevel, int k, int pos, int *bestvalue, int *bestchoices);   //Recursive function used to compute best set of solution. It returns 1 if the solution we have found is long enough for what we are looking for, else 0
int checkSolution (data *set, int *choices, int actual, int previous, int *amount, int operation); //Operation is used as a switch for the function.
    //[Operation == 0] checks if the new jewel we want to insert in the solution set is allowed or not (y the restriction and the repetition). If yes, it proceeds by adding that jewel to the solution, if not, it skips to the next one.
    //[Operation == 1] checks in any time of the recursion if the number of sapphire exceeds the number of emeralds
    //[Operation == 2] checks in the very end if the solution is valid according to the relation between zapphire and emeralds
void readSET(FILE *fp, data *set);  //Acquisition of data from FILE
int computeTotSET (data set);   //Computes the total amount of jewels
void printSET (data set);   //Prints information about the particular set of the test
void checkFILE (FILE *fp, char *filename);
int main(int argc, const char *argv[]){
    
     //Checking command line parameters
     if (argc != 2){
     fprintf(stderr, "ERROR: insert only program and FILE name.\n\n");
     exit(EXIT_FAILURE);
     }
     
    char filename[MAX_FILENAME_LENGTH];
    strcpy(filename, argv[1]);
    data set;   //Stores all data from FILE for every test
    int *choices = NULL;    //Stores in every position a number between 0 and N_JEWELS in order to indicate that in that position will be a particular jewel due to the number stored. 0 for zapphire, 1 for rubie and so on...
    int *bestchoices = NULL;    //Stores the best solution we have found so far
    int bestvalue = 0;  //Stores the best value of necklace we have found so far in the recursion
    int value_max = 0;  //Stores the best value of necklace we have found by the iteration of recursive function to every expected length necklace
    int length = 0;     //Stores the length of the best solution set (necklace)
    int test_number;    //Stores number of tests in the FILE
    int i = 0, j = 0;   //Counters
    char jewels[N_JEWELS] = {'S', 'R', 'T', 'E'};   //Used to print necklace composition, using bestchoices[] as a selector for which jewel has to be printed
    
    //FILE opening
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    checkFILE(fp, filename);
    fscanf(fp, "%d", &test_number);
    
    
    //Running TESTS from FILE
    for (i = 0; i < test_number; i++){
        //Acquisition of data
        readSET(fp, &set);
        
        //Printing information about a particular set of jewels
        printf("TEST #%d\n", i+1);
        printSET(set);
        
        //Dynamic allocation for solution arrays. We can't know in advance the maximum length of the necklace but we know it can be no longer than the total amount of jewels avaiable
        choices = (int*) calloc(set.tot, sizeof(int));
        bestchoices = (int*) calloc(set.tot, sizeof(int));
        if (choices == NULL || bestchoices == NULL){
            fprintf(stderr, "ERROR: some errors occured while trying to compute results for the test.\n\n");
            exit(EXIT_FAILURE);
        }
        
        
        //Printing the result
        value_max = 0;  //Resetting value_max for the new set of data
        length = 0; //Resetting length for the new set of data
        bestvalue = 0;  //Resetting bestvalue for the new set of data
        
        //Just like exercise 01 from LAB07, the idea here is to move from the longest necklace to the shortest, computing every solution set, looking for the best necklace value for that length, and, if it is better than any previous one, we upload the "value_max"
        for (j = set.tot; j >= 1; j--)
            
            //If the recursive function returned 1 (gave us a solution) and that value of solution is better than any previous value found, we upload data and move to next iteration (decreasing expected length)
            if (computeSET(&set, choices, &bestvalue, bestchoices, j) && bestvalue > value_max){
                length = j;
                value_max = bestvalue;
            }
        
        //Printing results
        printf("Best necklace with %d jewels has a total value of %d\n", length, value_max);
        printf("Necklace composition: ");
        
        //Printing necklace composition
        for (int k = 0; k < length; k++)
            printf("%c", jewels[bestchoices[k]]);
        printf(".\n\n");
        
        //Deallocating space in memory for next test
        free(choices);
        free(bestchoices);
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}
//Wrapper function used to copy original amounts of jewels for a particular test
int computeSET (data *set, int *choices, int *bestvalue, int *bestchoices, int k){
    int i;
    
    //Copy of the orginal amounts that will be modified by the recursive function
    int amount[N_JEWELS];
    for (i = 0; i < N_JEWELS; i++)
        amount[i] = set->jewel[i].amount;
    
    //Main function
    return computeSET_R(set, amount, choices, 0, k, -1, bestvalue, bestchoices);
    
}
int computeSET_R (data *set, int *amount, int *choices, int recursionLevel, int k, int pos, int *bestvalue, int *bestchoices){
    int i;
    int j;
    int value_tmp;
    
    //If necklace has reached maximum length avaiable
    if (recursionLevel >= k){
        
        //We check if the solution is valid or not (if number of zapphire used is greater than number of emeralds used)
        if (checkSolution(set, choices, 0, 0, amount, 2) == 0){
            return 0;   //If not, we return 0 to the caller function (we go backtracking)
        }
        
        //If it's good, we check if the current value is greater than the actual best value and, in case, upload it
        else {
            value_tmp = computeValue(set, choices, recursionLevel-1);
            if (value_tmp > (*bestvalue)){
                //Uploading value
                (*bestvalue) = value_tmp;
                
                //Uploading solution array
                for (j = 0; j <= recursionLevel-1; j++)
                    bestchoices[j] = choices[j];
            }
            return 1;   //We return 1 (we have finished our necklace)
        }
    }
    
    //Checking partial solution, we check in every moment if the number of zapphire used is greater than the total amount of emeralds. Obv it can't be possible due to the restriction of the problem
    if (checkSolution(set, choices, 0, 0, amount, 1) == 0)
        return 0;   //We return 0 to the caller function (we go backtracking) if the solution is not allowed by the restriction
    
    //Choosing for every position one of the N_JEWELS jewels.
    for (i = 0; i < N_JEWELS; i++){
        
        //If we did not run out of that jewel, we can take that
        if (amount[i] > 0){
            
            //If we are not in the first instance of recursion (so taking the first jewel) when all the choices are good, we check if the current jewel we have in mind to use is allowed to be used by the problem or not
            if (pos != -1){
                if (checkSolution(set, choices, i, pos, amount, 0) == 0)
                    //If not, we move to the next jewel
                    continue;
            }
            
            //After having checked, we can now take that element
            amount[i]--;                    //Decreasing jewel avaiable
            choices[recursionLevel] = i;    //Inserting jewel in the solution array
            
            //Recursively move to the next position in the necklace
            if (computeSET_R(set, amount, choices, recursionLevel+1, k, recursionLevel, bestvalue, bestchoices))
                return 1;
            
            //Backtracking, we know have in mind to take a different jewel so we re-add the current one and move to the next one
            amount[i]++;
        }
    }
    
    return 0;   //Returning zero if the solution has not reached the length we were looking for
}
int computeValue(data *set, int *choices, int n){
    int i;
    int value_tot = 0;
    
    //Computing value for the necklace
    for (i = 0; i <= n; i++)
        value_tot = value_tot + (set->jewel[choices[i]].value);
    
    return value_tot;
}
//Check if the current jewel we want to add in the necklace is allowed by the problem or not.
int checkSolution (data *set, int *choices, int actual, int previous, int *amount, int operation){
    
    if (operation == 0){
        //Restriction about previous and actual jewels
        if (((choices[previous] == 0) || (choices[previous] == 2)) && (actual == 2 || actual == 3))
            return 0;
        if (((choices[previous] == 3) || (choices[previous] == 1)) && (actual == 1 || actual == 0))
            return 0;
        
        //Restriction about repetition
        int i = 0;
        while (choices[previous-i] == actual && (previous-i) >= 0)
            ++i;
        if (i == set->max_rip)
            return 0;
    }
    else if (operation == 1){
        //Restriction about taken sapphire and emeralds
        if ((set->jewel[0].amount - amount[0]) > (set->jewel[3].amount))
            return 0;
    }
    else if (operation == 2){
        //Restriction about all sapphire and emeralds
        if ((set->jewel[0].amount - amount[0]) > (set->jewel[3].amount) - amount[3])
            return 0;
    }
    
    return 1;   //Solution is valid
}
//Acquisition of data
void readSET(FILE *fp, data *set){
    int i;
    for (i = 0; i < N_JEWELS; i++)
        fscanf(fp, "%d", &set->jewel[i].amount);    //Reading amounts of jewels
    for (i = 0; i < N_JEWELS; i++)
        fscanf(fp, "%d", &set->jewel[i].value);     //Reading values of jewels
    fscanf(fp, "%d", &set->max_rip);                //Reading max_rip value
    set->tot = computeTotSET(*set);                 //Computing total amount of jewels
}
void checkFILE (FILE *fp, char *filename){
    if (fp == NULL){
        fprintf(stderr, "[ERROR]\nSome errors occurred while trying to open FILE: %s.\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
void printSET (data set){
    printf("Sapphires: %d [%d] - ", set.jewel[0].amount, set.jewel[0].value);
    printf("Rubies: %d [%d] - ", set.jewel[1].amount, set.jewel[1].value);
    printf("Topaz: %d [%d] - ", set.jewel[2].amount, set.jewel[2].value);
    printf("Emeralds: %d [%d] - ", set.jewel[3].amount, set.jewel[3].value);
    
    //ADD here new jewels
    
    printf("TOT: %d ", set.tot);
    printf("{Max_rip allowed: %d}\n", set.max_rip);
}
int computeTotSET (data set){
    int i;
    int result = 0;
    for (i = 0; i < N_JEWELS; i++)
        result = result + set.jewel[i].amount;
    return result;
}
