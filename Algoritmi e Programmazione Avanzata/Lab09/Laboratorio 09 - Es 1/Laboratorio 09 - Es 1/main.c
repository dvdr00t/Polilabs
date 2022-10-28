//
//  main.c
//  Laboratorio 09 - Es 1
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
    int start;  //Starting point
    int end;    //Ending point
    int period; //Total period
} data;

int N_activities;   //Number of activities read from FILE

//SOLUTION FUNCTIONS (DYNAMIC PROGRAMMING) + AUXILIARS
void computeSOLUTION (data *activities);    //Main function for solution
int checkACTIVITIES (int index_one, int index_two, data *activities);   //Check if two activities are compatible or not
int latest_COMPATIBILITY (data *activities, int i); //Search for the latest activity int the sorted set of activities which is compatible with the added one
int search_ACTIVITY (int *opt, int l);  //Search for the latest activity which was added to the [0, ..., l] set of activities compatible with the added one
int getMAX (int A, int B);  //Return the max value between two integers
void LISprint(data *activities, int *pos, int i);   //Recursively printing the solution

//SORTING FUNCTIONS + AUXILIARS
void sortACTIVITIES (data *activities); //Wrapper function
void quickSort (data *activities, int left, int right); //Implemenetation of quicksort algorithm
int compareACTIVITIES (data activity_one, data activity_two);   //Return 1 if activity_one starts first than activity_two, else 0. If two activities start at the same time, we want to sort them in a way that the longest activity comes first
void swap (data *activities, int i, int j); //Swap two activities
int partition (data *activities, int left, int right);  //Split the set of data in two subset, one filled with elements that have to be (for some sorting reason) on the left side of the pivot, and one subset with elements that have to be in the right side of the pivot, and returning the new position for the pivot

//READ FUNCTIONS + AUXILIARS
data* readACTIVITIES (void);    //Reads activities from FILE and stores them in memory
int computePERIOD (data activity);  //Return the period of an activity
void showACTIVITIES (data *activities); //Show sorted activities

int main(int argc, const char *argv[]) {
    
    
     //CHECKIN TERMINAL PARAMETERS
     if (argc != 1){
     fprintf(stderr, "ERROR: insert only program name!\n\n");
         exit(EXIT_FAILURE);
     }
    
    //READING DATA
    data *activities = NULL;
    activities = readACTIVITIES();
    
    //SORTING DATA
    sortACTIVITIES(activities);
    
    //SHOWING SORTED ACTIVITIES
    showACTIVITIES(activities);
    
    //COMPUTING SOLUTION AND PRINTING
    computeSOLUTION(activities);
    
    //DEALLOCATION OF SPACE FOR ARRAY USED
    free(activities);
    
    exit(EXIT_SUCCESS);
}


/* ----  FUNCTIONS  ---- */

//SOLUTION FUNCTIONS (DYNAMIC PROGRAMMING)
void computeSOLUTION (data *activities) {
    
    //DYNAMIC ALLOCATION OF SPACE FOR SOLUTION ARRAYS
    /* Tracks the position of the activities in the solution-set of activities. For every activities:
     * "-1" means that that activity is not preceded by any other activity
     * "n" where n is a number between 0 and N_activities means that that activity is preceded by the n-th activity
     */
    int *pos = NULL;
    
    // Tracks for every activity (from 0 to N_activity) the best value of the solution set due to the selection of the best_value latest_compatible activity
    int *opt = NULL;
    
    opt = (int *) calloc(N_activities, sizeof(int));
    if (opt == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    pos = (int *) calloc(N_activities, sizeof(int));
    if (pos == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    
    //INITIALIZING THE SOLUTION ARRAYS
    opt[0] = computePERIOD(activities[0]);
    pos[0] = -1;
    
    //"ADDING" EVERY ACTIVITY TO THE SOLUTION SET
    for (int i = 1; i < N_activities; i++) {
        
        //UPLOADING CURRENT PERIOD (PERIOD OF THE ADDED ACTIVITY)
        int curr_period = activities[i].period;
        
        //BINARY-SEARCH FOR THE LATEST ACTIVITY WHICH IS COMPATIBLE WITH THE ADDED ACTIVITY (TIME COMPLEXITY: nlog(n))
        int l = latest_COMPATIBILITY(activities, i);
        
        //IF THERE IS A COMPATIBLE ACTIVITY FOR THE ADDED ONE
        if (l != -1) {
            //ADDING TO THE CURRENT PERIOD THE BEST VALUE COMPUTED FOR THE SUB-ARRAY [0, ..., l]
            curr_period = curr_period + opt[l];
            
            //SEARCHING FOR THE ACTIVITY THAT PRECEEDS THE ADDED ONE
            pos[i] = search_ACTIVITY(opt, l);
        }
        
        //IF THERE IS NOT A COMPATIBLE ACTIVITY FOR THE ADDED ONE, SET THIS AS A "FIRST ACTIVITY"
        else
            pos[i] = -1;
        
        //COMPUTE THE BEST SOLUTION VALUE FOR SUB ARRAY [0, ..., i]
        opt[i] = getMAX(curr_period, opt[i-1]);
    }
    
    //PRINTING SOLUTION
    printf("\nLongest set of compatible activities period is: %d\n", opt[N_activities-1]);
    printf("Activities chosen are:\n");
    LISprint(activities, pos, N_activities-1);
    
    //DEALLOCATING MEMORY
    free(opt);
    free(pos);
}
int getMAX (int A, int B) {
    if (A >= B)
        return A;
    return B;
}
int latest_COMPATIBILITY (data *activities, int i) {
    
    //BINARY-SEARCH BASED-IMPLEMENTATION
    int left = 0;
    int right = i - 1;
    
    while (left <= right) {
        
        //COMPUTING MEDIUM INEDEX OF THE SUB-ARRAY
        int medium = (left + right)/2;
        
        //CHECKING COMPATIBILITY
        if (activities[medium].end <= activities[i].start)
            
            //CHECKING IF IT IS THE RIGHT-MOST ELEMENT AND RETURNING MEDIUM-INDEX IN CASE IT IS
            if (activities[medium + 1].end <= activities[i].start)
                left = medium + 1;  //UPLOADING LEFT INDEX
            else
                return medium;
        else
            right = medium - 1; //UPLOADING RIGHT INDEX
    }
    
    //RETURN UNSUCCESFULL
    return -1;
}
int search_ACTIVITY (int *opt, int l) {
    int i;
    
    //SEARCHING FOR THE INDEX OF THE ACTIVITY WHICH WAS THE LAST ADDED TO THE SET OF ACTIVITIES COMPATIBLE WITH THE ADDED ONE
    for (i = 0; i <= l; i++)
        if (opt[i] == opt[l])
            return i;
    return 0;
}
int checkACTIVITIES (int index_one, int index_two, data *activities) {
    if (activities[index_one].start < activities[index_two].end && activities[index_two].start < activities[index_one].end)
        return 0;
    return 1;
}
void LISprint(data *activities, int *pos, int i) {
    if (pos[i] == -1) {
        printf("%d %d - %d\n", activities[i].start, activities[i].end, activities[i].period);
        return;
    }
    LISprint(activities, pos, pos[i]);
    printf("%d %d - %d\n", activities[i].start, activities[i].end, activities[i].period);
}
            

//SORTING FUNCTIONS + AUXILIARS
void sortACTIVITIES (data *activities) {
    
    //IMPLEMENTATION OF QUICKSORT ALGORITHM
    int left = 0;
    int right = N_activities-1;
    
    quickSort(activities, left, right);
}
void quickSort (data *activities, int left, int right) {
    int pivot_index;
    
    //TERMINAL CONDITION
    if (right <= left)
        return;
    
    //RECURSIVELY COMPUTING INDEX OF PIVOT AND SORTING LEFT AND RIGHT SUBSET OF ELEMENTS. [NB]: no ricombination
    pivot_index = partition (activities, left, right);
    quickSort(activities, left, pivot_index-1);
    quickSort(activities, pivot_index+1, right);
}
int compareACTIVITIES (data activity_one, data activity_two) {
    
    //ACTIVITIES WITH A ENDING POINT THAT COMES EARLIE IN TIME, COME FIRST IN THE SORTED ARRAY
    if (activity_one.end < activity_two.end)
        return 1;
    
    //FOR ACTIVITIES WITH THE SAME ENDING POINT
    if (activity_one.end == activity_two.end){
        
        //ACTIVITIES WITH A LARGER PERIOD OF TIME COME FIRST IN THE SORTED ARRAY
        if (activity_one.period > activity_two.period)
            return 1;
        
        return 0;
    }
    
    return 0;
}
void swap (data *activities, int i, int j) {
    data tmp;
    tmp = activities[i];
    activities[i] = activities[j];
    activities[j] = tmp;
}
int partition (data *activities, int left, int right) {
    int i = left-1;
    int j = right;
    
    //CHOOSING PIVOT (RIGHT-MOST ELEMENT
    data pivot = activities[right];
    
    //LOOP TO SORT ACTIVITIES
    for(; ;){
        
        //MOVING ACTIVITIES TO THE LEFT SIDE OF THE PIVOT
        while (compareACTIVITIES(activities[++i], pivot));
        while (!compareACTIVITIES(activities[--j], pivot))
            if (j == left)
                break;
        if (i >= j)
            break;
        
        swap(activities, i, j);
    }
    
    swap(activities, i, right);
    
    //RETURNING NEW INDEX OF THE PIVOT
    return i;
}

//READ FUNCTIONS + AUXILIARS
data* readACTIVITIES (void) {
    
    //FILE OPENING AND CHECKING
    char filename[MAX_FILENAME_LENGTH];
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
int computePERIOD (data activity) {
    return (activity.end - activity.start);
}
void showACTIVITIES (data *activities) {
    int i;
    fprintf(stdout, "FORMAT: [start end - period]\n\n");
    for (i = 0; i < N_activities; i++)
        fprintf(stdout, "%d %d - %d\n", activities[i].start, activities[i].end, activities[i].period);
}
