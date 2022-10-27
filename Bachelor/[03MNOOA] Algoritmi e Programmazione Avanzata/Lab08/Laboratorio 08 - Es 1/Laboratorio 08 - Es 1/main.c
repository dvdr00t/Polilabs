//
//  main.c
//  Laboratorio 08 - Es 1
//
//  Created by Davide Arcolini on 12/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME_LENGTH 50

//STRUCT OF ACTIVITY
typedef struct {
    int start;
    int end;
    int period;
} data;

int N_activities;
int *BEST_choices = NULL;
int bestPeriod = 0;

void printSOLUTION (data *activities);
void copyCHOICES(int *choices_DEST, int *choices_START);
int computeSOLUTION_PERIOD (int *TMP_choices, data *activities);
int checkCOMPATIBILITY (int *TMP_choices, data *activities, int index_one);
int checkACTIVITIES (int index_one, int index_two, data *activities);
void computeSOLUTION_R (data *activities, int *chosen, int recursionLevel);
void computeSOLUTION (data *activities);
int computePERIOD (data activity);
data* readACTIVITIES (void);
int main(/*int argc, const char *argv[]*/) {
    
    /*
     //CHECKIN TERMINAL PARAMETERS
     if (argc != 1){
        fprintf(stderr, "ERROR: insert only program name!\n\n");
        exit(EXIT_FAILURE)
     }
     */
    
    //READING DATA
    data *activities = NULL;
    activities = readACTIVITIES();
    
    //DYNAMIC ALLOCATION FOR BEST_SOLUTION ARRAY
    BEST_choices = (int *) calloc(N_activities, sizeof(int));
    if (BEST_choices == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //COMPUTING SOLUTION
    computeSOLUTION(activities);
    printSOLUTION(activities);
    
}
data* readACTIVITIES (void){
    
    //FILE OPENING AND CHECKING
    char filename[MAX_FILENAME_LENGTH] = "att.txt";
    fprintf(stdout, "INSERT FILE NAME: ");
    scanf("%s", filename);
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s", filename);
        exit(EXIT_FAILURE);
    }
    
    //READ NUMBER OF ACTIVITIES FROM FILE
    fscanf(fp, "%d\n", &N_activities);
    
    //DYNAMIC ALLOCATION OF SPACE IN MEMORY FOR ARRAY OF STRUCT
    data *activities = NULL;
    activities = (data*) malloc(N_activities * sizeof(data));
    if (activities == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //READ DATA FROM FILE
    int i;
    for (i = 0; i < N_activities; i++) {
        fscanf(fp, "%d %d\n", &activities[i].start, &activities[i].end);
        activities[i].period = computePERIOD(activities[i]);
    }
    
    return activities;
    
}
int computePERIOD (data activity){
    return (activity.end - activity.start);
}
void computeSOLUTION (data *activities){
    
    //DYNAMIC ALLOCATION FOR TMP_SOLUTION ARRAY
    int *TMP_choices = NULL;
    TMP_choices = (int *) calloc(N_activities, sizeof(int));
    if (TMP_choices == NULL){
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    computeSOLUTION_R(activities, TMP_choices, 0);
}
void computeSOLUTION_R (data *activities, int *TMP_choices, int recursionLevel){
    int i;
    
    //TERMINAL CONDITION
    if (recursionLevel >= N_activities){
        //CHECKING IF TMP_SOLUTION IS BETTER THAN BEST SOLUTION
        int periodTOT = computeSOLUTION_PERIOD(TMP_choices, activities);
        if (periodTOT > bestPeriod){
            bestPeriod = periodTOT;
            copyCHOICES(BEST_choices, TMP_choices);
        }
        
        return;
    }
    
    
    //CHOOSING EVERY ACTIVITY
    for (i = 0; i < N_activities; i++){
        
        //CHECKING IF WE HAVE ALREADY CHOSEN THAT ACTIVITY
        if (TMP_choices[i] == 0) {
            
            //CHECKING IF THE NEW ACTIVITY IS COMPATIBLE WITH THE PREVIOUS ONEs
            if (checkCOMPATIBILITY(TMP_choices, activities, i)){
                
                //CHOOSING THE ACTIVITY
                TMP_choices[i] = 1;
                computeSOLUTION_R(activities, TMP_choices, recursionLevel+1);
                
                //LEAVING THAT ACTIVITY
                TMP_choices[i] = 0;
                computeSOLUTION_R(activities, TMP_choices, recursionLevel+1);
            }
        }
    }
    return;
    
}
int checkCOMPATIBILITY (int *TMP_choices, data *activities, int index_one){
    int i;
    for (i = 0; i < N_activities; i++)
        if (TMP_choices[i] == 1)
            if(!checkACTIVITIES(index_one, i, activities))
                return 0;
    
    return 1;
}
int computeSOLUTION_PERIOD (int *TMP_choices, data *activities) {
    int i;
    int period = 0;
    
    for (i = 0; i < N_activities; i++)
        if (TMP_choices[i] == 1)
            period = period + activities[i].period;
    
    return period;
}
void copyCHOICES(int *choices_DEST, int *choices_START){
    int i;
    for (i = 0; i < N_activities; i++)
        choices_DEST[i] = choices_START[i];
}
int checkACTIVITIES (int index_one, int index_two, data *activities) {
    if (activities[index_one].start < activities[index_two].end)
        return 0;
    return 1;
}
void printSOLUTION (data *activities){
    int i;
    int period = 0;
    printf("\nActivities chosen:\n");
    for (i = 0; i < N_activities; i++)
        if (BEST_choices[i] == 1){
            printf("%d %d\n", activities[i].start, activities[i].end);
            period = period + activities[i].period;
        }
    
    printf("\nTOTAL PERIOD: %d\n\n", period);
}
