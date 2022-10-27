//
//  main.c
//  Laboratorio 10 - Es 01
//
//  Created by Davide Arcolini on 07/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILENAME_LENGTH 30
#define N_JEWELS 4

/* --- DATA --- */

//Struct which stores all data for every test
typedef struct {
    int amount[N_JEWELS];   //Amount of jewels avaiable. (.amount[0] is for the first jewel (zapphire), .amount[1] is for the second jewel (rubie) and so on...)
    int tot;    //Total number of jewels in the set
} data;

int ****S = NULL;   //Matrix storing maximum necklace lenght for MEMOIZATION
int ****R = NULL;   //Matrix storing maximum necklace lenght for MEMOIZATION
int ****T = NULL;   //Matrix storing maximum necklace lenght for MEMOIZATION
int ****E = NULL;   //Matrix storing maximum necklace lenght for MEMOIZATION


/* --- FUNCTIONS --- */

//Main functions used to compute recursively the maximum lenght of a necklace that starts with a particular jewel. Every iteration result is stored in the 4D matrix that starts with the particular jewel (MEMOIZATION STRUCTURE)
int fS (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds);
int fR (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds);
int fT (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds);
int fE (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds);

int**** malloc4D (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds);  //Dynamic allocation for a 4D matrix
void free4D (int**** matrix4D, int N_sapphires, int N_rubies, int N_topaz, int N_emeralds);   //Free of allocated matrix
void readSET(FILE *fp, data *set);  //Read data from FILE and store them in the data structure
int computeTotSET (data set);   //Compute the total amount of jewel in a set
int max (int A, int B); //Return the max value between two integers

int main(int argc, const char *argv[]) {
    
    //CHECKING TERMINAL PARAMETERS
    if (argc != 1) {
        fprintf(stderr, "ERROR: insert only program name!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //GETTING FILE INFORMATION
    char filename[MAX_FILENAME_LENGTH];
    fprintf(stdout, "Insert FILE name: ");
    fscanf(stdin, "%s", filename);
    
    //FILE OPENING
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to open FILE: %s\n\n", filename);
        exit(EXIT_FAILURE);
    }
    
    /* --- DATA --- */
    
    int maxS; //maxS is the maximum lenght value of the necklace that starts with the sapphire
    int maxR; //maxR is the maximum lenght value of the necklace that starts with the rubie
    int maxT; //maxT is the maximum lenght value of the necklace that starts with the topaz
    int maxE; //maxE is the maximum lenght value of the necklace that starts with the emerald
    
    data set;   //Set of data storing amounts of jewels available
    
    //RUNNING TESTS
    int N_tests;
    fscanf(fp, "%d", &N_tests);
    for (int i = 0; i < N_tests; i++) {
        readSET(fp, &set);
        
        //DYNAMIC ALLOCATION FOR 4D MATRIX
        S = malloc4D(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        R = malloc4D(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        T = malloc4D(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        E = malloc4D(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        
        //COMPUTING VALUES
        maxS = fS(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        maxR = fR(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        maxT = fT(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        maxE = fE(set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        
        //FREE
        free4D(S, set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        free4D(R, set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        free4D(T, set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        free4D(E, set.amount[0], set.amount[1], set.amount[2], set.amount[3]);
        
        printf("TEST #%d\n", i+1);
        printf("Maximum necklace lenght value is: %d\n\n", max(maxS, max(maxR, max(maxT, maxE))));
    }
}


int fS (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds) {
    
    //TERMINAL CONDITION
    if (N_sapphires <= 0)
        return 0;
    
    //MEMOIZATION RETURN VALUE
    if (S[N_sapphires][N_rubies][N_topaz][N_emeralds] != -1)
        return S[N_sapphires][N_rubies][N_topaz][N_emeralds];
    
    //RECURSION
    --N_sapphires;  //Decreasing number of sapphire
    //Uploading matrix by recursively computing the lenght value
    S[N_sapphires+1][N_rubies][N_topaz][N_emeralds] = 1 + max(fS(N_sapphires, N_rubies, N_topaz, N_emeralds), fR(N_sapphires, N_rubies, N_topaz, N_emeralds));
    
    return S[N_sapphires+1][N_rubies][N_topaz][N_emeralds];
}
int fR (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds) {
    
    //TERMINAL CONDITION
    if (N_rubies <= 0)
        return 0;
    
    //MEMOIZATION RETURN VALUE
    if (R[N_sapphires][N_rubies][N_topaz][N_emeralds] != -1)
        return R[N_sapphires][N_rubies][N_topaz][N_emeralds];
    
    //RECURSION
    --N_rubies;  //Decreasing number of sapphire
    //Uploading matrix by recursively computing the lenght value
    R[N_sapphires][N_rubies+1][N_topaz][N_emeralds] = 1 + max(fE(N_sapphires, N_rubies, N_topaz, N_emeralds), fT(N_sapphires, N_rubies, N_topaz, N_emeralds));
    
    return R[N_sapphires][N_rubies+1][N_topaz][N_emeralds];
}
int fT (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds) {
    
    //TERMINAL CONDITION
    if (N_topaz <= 0)
        return 0;
    
    //MEMOIZATION RETURN VALUE
    if (T[N_sapphires][N_rubies][N_topaz][N_emeralds] != -1)
        return T[N_sapphires][N_rubies][N_topaz][N_emeralds];
    
    //RECURSION
    --N_topaz;  //Decreasing number of sapphire
    //Uploading matrix by recursively computing the lenght value
    T[N_sapphires][N_rubies][N_topaz+1][N_emeralds] = 1 + max(fS(N_sapphires, N_rubies, N_topaz, N_emeralds), fR(N_sapphires, N_rubies, N_topaz, N_emeralds));
    
    return T[N_sapphires][N_rubies][N_topaz+1][N_emeralds];
}
int fE (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds) {
    
    //TERMINAL CONDITION
    if (N_emeralds <= 0)
        return 0;
    
    //MEMOIZATION RETURN VALUE
    if (E[N_sapphires][N_rubies][N_topaz][N_emeralds] != -1)
        return E[N_sapphires][N_rubies][N_topaz][N_emeralds];
    
    //RECURSION
    --N_emeralds;  //Decreasing number of sapphire
    //Uploading matrix by recursively computing the lenght value
    E[N_sapphires][N_rubies][N_topaz][N_emeralds+1] = 1 + max(fE(N_sapphires, N_rubies, N_topaz, N_emeralds), fT(N_sapphires, N_rubies, N_topaz, N_emeralds));
    
    return E[N_sapphires][N_rubies][N_topaz][N_emeralds+1];
}

int**** malloc4D (int N_sapphires, int N_rubies, int N_topaz, int N_emeralds) {
    
    int ****tmp = NULL;
    
    tmp = (int ****) malloc((1 + N_sapphires) * sizeof(int ***));
    if (tmp == NULL) {
        fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (1 + N_sapphires); i++) {
        tmp[i] = (int ***) malloc((1 + N_rubies) * sizeof(int **));
        if (tmp[i] == NULL) {
            fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
            exit(EXIT_FAILURE);
        }
        
        for (int j = 0; j < (1 + N_rubies); j++) {
            tmp[i][j] = (int **) malloc((1 + N_topaz) * sizeof(int *));
            if (tmp[i][j] == NULL) {
                fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
                exit(EXIT_FAILURE);
            }
            
            for (int k = 0; k < (1 + N_topaz); k++) {
                tmp[i][j][k] = (int *) malloc((1 + N_emeralds) * sizeof(int));
                if (tmp [i][j][k] == NULL) {
                    fprintf(stderr, "ERROR: some errors occurred while trying to store data in memory.\n\n");
                    exit(EXIT_FAILURE);
                }
                
                //INITIALIZING 4D MATRIX
                for (int l = 0; l < (1 + N_emeralds); l++)
                    tmp[i][j][k][l] = -1;
            }
        }
    }
    
    return tmp;
}
void free4D (int**** matrix4D, int N_sapphires, int N_rubies, int N_topaz, int N_emeralds) {
    for (int i = 0; i < N_sapphires; i++) {
        for (int j = 0; j < N_rubies; j++) {
            for (int k = 0; k < N_topaz; k++)
                free(matrix4D[i][j][k]);
            free(matrix4D[i][j]);
        }
        free(matrix4D[i]);
    }
    free(matrix4D);
}
void readSET(FILE *fp, data *set) {
    for (int i = 0; i < N_JEWELS; i++)
        fscanf(fp, "%d", &set->amount[i]);
    
    set->tot = computeTotSET(*set);
}
int computeTotSET (data set) {
    int result = 0;
    for (int i = 0; i < N_JEWELS; i++)
        result = result + set.amount[i];
    return result;
}
int max (int A, int B) {
    if (A > B)
        return A;
    return B;
}
