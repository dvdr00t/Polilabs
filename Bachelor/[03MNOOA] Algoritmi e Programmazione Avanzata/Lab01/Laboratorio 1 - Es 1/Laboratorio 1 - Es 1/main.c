
//  main.c
//  Laboratorio 1 - Esercizio 1
//
//  Created by Davide Arcolini on 05/10/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TEAM 20
#define MAX_GAMES 20

int* initialize_chart(int n_teams);                     //Set chart values to zero
int** getting_matrix (FILE *fp, int *nr, int *nc);      //Dynamic allocation + saving elements in the matrix from file
int maximum (int *chart, int n_teams);                  //Algorithm for finding the maximum in an array (first occurence is the saved one)
void rank(int *chart, int **game, int n_teams, int i);  //Saving in corresponding row of chart the daily points every team made

int main (/*int argc, char *argv[]*/)
{
    /*
     //Checking line command arguments
     if (argc != 2){
     fprintf(stderr, "ERROR: insert only program name and file name.\n\n");
     exit(EXIT_FAILURE);
     }
     */
    
    FILE *fp;
    /*char *filename;*/
    int **game, *chart;
    int n_teams, n_games;
    int i;
    
    //Opening FILE and checking
    /* filename = argv[1];*/
    fp = fopen(/*filename*/"torneo.txt", "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: FILE opening failed.\n\n");
        exit(EXIT_FAILURE);
    }
    
    game = getting_matrix(fp, &n_teams, &n_games);    //Assign to the pointer (pointers of pointers) "game" the matrix one pointed in the function
    fclose(fp);
    chart = initialize_chart(n_teams);

    //Printing chart for every games
    for (i = 0; i < n_games; ++i){
        
        //Getting chart
        rank(chart, game, n_teams, i);
        //Finding maximum in a vector
        printf("Team number %d is winning during %d day.\n", maximum(chart, n_teams)+1, i+1);
    }
    
    exit(EXIT_SUCCESS);
    
}
int** getting_matrix (FILE *fp, int *nr, int *nc){
    
    int i, j;
    int rows = 0, columns = 0;
    int **matrix = NULL;
    char line[MAX_GAMES];
    
    //Reading file for the first time in order to get the dimensions (rows and columns) of the matrix
    while (fgets(line, MAX_GAMES, fp) != NULL){
        ++rows;
    }
    columns = (((int)strlen(line))/2) + 1;  //Columns value is given by considering "n" numbers in "line" and "n-1" spaces. So, basically, dividing by two and adding one to get the amount of number "n"
    rewind(fp);
    
    
    //Dynamic allocation of space for matrix
    matrix = malloc(rows * sizeof(int*));
    if (matrix == NULL){
        fprintf(stderr, "ERROR: not enough space in memory!\n\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < rows; i++){
        matrix[i] = malloc(columns * sizeof(int));
        if (matrix[i] == NULL){
            fprintf(stderr, "ERROR: not enough space in memory!\n\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //Getting elements and store them in matrix
    for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }
    
    //Save in the part of memory pointed by nr/nc the value row/column (*nr is pointing to n_teams in the main, *nc to n_games)
    *nr = rows;
    *nc = columns;
    
    //Return the pointer to the complete matrix
    return matrix;
}

int* initialize_chart(int n_teams){
    int *array = NULL;
    int i;
    
    //Dynamic allocation of space for array
    array = malloc(n_teams * sizeof(int));
    if (array == NULL){
        fprintf(stderr, "ERROR: not enough space in memory!\n\n");
    }
    
    //Setting values to zero
    for (i = 0; i < n_teams; i++){
        array[i] = 0;
    }
    return array;
}

void rank(int *chart, int **game, int n_teams, int i){
    
    int j;
    for (j = 0; j < n_teams; ++j){
        chart[j] += game[j][i];     //Saving in corresponding row of chart the daily points every team made
    }
    
}
int maximum (int *chart, int n_teams){ //Algorithm for finding the maximum in an array (first occurence is the saved one)
    int i;
    int max_value = 0, position = 0;
    for (i = 0; i < n_teams; ++i){
        if (chart[i] > max_value){
            max_value = chart[i];
            position = i;
        }
    }
    return position;
}
