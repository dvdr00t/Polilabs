//
//  main.c
//  Laboratorio 5 - Es 2
//
//  Created by Davide Arcolini on 14/11/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILENAME_LENGHT 15

void separa(int **mat, int rows, int columns, int **array_white, int **array_black);
int** reading_file(FILE *fp, int *rows, int *columns);  //Reading data from the file and store them in a 2D array
void checking_file(FILE *fp, char *filename);   //Checking correct opening for a file
int main(/*int argc, char *argv[]*/){
    
    /*
    //Checking command line parameters
    if (argc != 2){
        fprintf(stderr, "ERROR: insert only program and file name.\n\n");
        exit(EXIT_FAILURE);
     }
    */
    char filename[MAX_FILENAME_LENGHT] = "data.txt";
    //strcpy(filename, argv[1]);
    int rows, columns, i;
    int **mat = NULL, *array_white = NULL, *array_black = NULL;
    
    //FILE opening
    FILE *fp;
    fp = fopen(filename, "r");  //FILE opening
    checking_file(fp, filename);    //Checking if the file has been opened well
    mat = reading_file(fp, &rows, &columns);    //Reading data from file
    fclose(fp); //File closing
    
    separa(mat, rows, columns, &array_white, &array_black);
    for (i = 0; i < rows; i++)
        free(mat[i]);
    free(mat);
    
    //Printing results
    printf("Arry of white cells:");
    for (i = 0; i < (rows*columns)/2 + 1; i++){
        printf(" %d", array_white[i]);
    }
    printf(".\n");
    free(array_white);  //Deallocating of memory for array_white
    
    printf("Arry of black cells:");
    for (i = 0; i < (rows*columns)/2; i++){
        printf(" %d", array_black[i]);
    }
    printf(".\n\n");
    free(array_black);  //Deallocating memory for array_black
    
    exit(EXIT_SUCCESS);
}
void separa(int **mat, int rows, int columns, int **array_white, int **array_black){
    int i, j, k_white = 0, k_black = 0;
    
    //Dynamic allocation of memory for the arrays
    (*array_white) = (int*) malloc(((rows*columns)/2 + 1) * sizeof(int));
    if ((*array_white) == NULL){
        fprintf(stderr, "ERROR: not enough space in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    (*array_black) = (int*) malloc((rows*columns)/2 * sizeof(int));
    if ((*array_black) == NULL){
        fprintf(stderr, "ERROR: not enough space in memory.\n\n");
        exit(EXIT_FAILURE);
    }

    //Copying numbers stored in the "white cells" in array_white and data stored in the "black cells" in array_black
    for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){
            if ((i + j) % 2 == 0)
                (*array_white)[k_white++] = mat[i][j];
            else
                (*array_black)[k_black++] = mat[i][j];
        }
    }
}
int** reading_file(FILE *fp, int *rows, int *columns){
    int i, j;
    int **mat = NULL;
    fscanf(fp, "%d %d\n", rows, columns);
    
    //Dynamic allocation of memory for the 2D array
    mat = (int**) malloc((*rows) * sizeof(int*));
    if (mat == NULL){
        fprintf(stderr, "ERROR: not enough space in memory.\n\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < (*rows); i++){
        mat[i] = (int *) malloc((*columns) * sizeof(int));
        if (mat[i] == NULL){
            fprintf(stderr, "ERROR: not enough space in memory.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //Reading numbers from the file and storing them in the 2D array
    for (i = 0; i < (*rows); i++)
        for (j = 0; j < (*columns); j++)
            fscanf(fp, "%d", &mat[i][j]);
    
    //Returnng the double pointer to the 2D array
    return mat;
}
void checking_file (FILE *fp, char *filename){  //Check if a file is opening well
    if (fp == NULL){
        fprintf(stderr, "ERROR: file %s not found!\n\n", filename);
        exit(EXIT_FAILURE);
    }
}
