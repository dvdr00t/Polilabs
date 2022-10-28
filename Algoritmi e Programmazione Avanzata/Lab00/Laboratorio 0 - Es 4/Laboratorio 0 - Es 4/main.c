//
//  main.c
//  Laboratorio 0 - Es 4
//
//  Created by Davide Arcolini on 28/09/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int sum_matrix_elements(int **array, int rows_number, int columns_number);
int min(int a, int b);
void matrix_printing(int **array, int rows_number, int columns_number);
int main(/*int argc, const char *argv[]*/)
{
    
    /*
    //Checking command line
    if (argc != 2){
        fprintf(stderr, "Error: insert program name and text file name.\n\n");
        exit(EXIT_FAILURE);
    }
    
    */
    
    FILE *fp;
    int **array, **sub_array, **max_sub_array;
    int rows_number, columns_number, dim;
    int i, j, k, h, i_temp, j_temp;
    int sum, sum_max = 0;
    
    //Opening FILE and checking
    fp = fopen("matrix.txt", "r");
    if (fp == NULL){
        fprintf(stderr, "Error: file opening failed!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Getting dimensions for the matrix
    fscanf(fp, "%d %d", &rows_number, &columns_number);
    
    //Dynamic allocation of the matrix
    array = (int**) malloc(rows_number * sizeof(int));
    for (i = 0; i < rows_number; ++i){
        array[i] = (int*) malloc(columns_number * sizeof(int));
    }
    
    //Getting elements for the matrix
    for (i = 0; i < rows_number; ++i){
        for (j = 0; j < columns_number; ++j){
            fscanf(fp, "%d", &array[i][j]);
        }
    }
    
    
    //Printing original matrix
    matrix_printing(array, rows_number, columns_number);
    
    
    //Asking for submatrix and analizing
    do {
        printf("Set value for submatrix research: ");
        scanf("%d", &dim);
        
        //Checking the dim value
        if (dim < 1 || dim > min(rows_number, columns_number)){
            printf("Error: value must be included between 1 and the minimum size value of the matrix.\n\n");
        }
        else {
            
            //Dynamic allocation of the submatrix
            sub_array = (int**) malloc(dim * sizeof(int));
            for (i = 0; i < dim; ++i){
                sub_array[i] = (int*) malloc(dim * sizeof(int));
            }
            max_sub_array = (int**) malloc(dim * sizeof(int));
            for (i = 0; i < dim; ++i){
                max_sub_array[i] = (int*) malloc(dim * sizeof(int));
            }
            
            //Getting the submatrix values
            for (i = 0; i <= rows_number - dim; ++i){
                for (j = 0; j <= columns_number - dim; ++j){
                    
                    i_temp = i;
                    for (h = 0; h < dim; ++h){
                        j_temp = j;
                        for (k = 0; k < dim; ++k){
                            sub_array[h][k] = array[i_temp][j_temp];
                            ++j_temp;
                        }
                        ++i_temp;
                    }
                    
                    printf("\n");
                    matrix_printing(sub_array, dim, dim);
                }
                
            }
            
            
            //Checking sum of the elements in the matrix
            sum = sum_matrix_elements(sub_array, dim, dim);
            if (sum > sum_max){
                for (i = 0; i < dim; ++i){
                    for (j = 0; j < dim; ++j){
                        max_sub_array[i][j] = sub_array[i][j];
                    }
                }
            }
            
            printf("Matrix with the largest sum of elements (%d) is:\n\n", sum);
            matrix_printing(max_sub_array, dim, dim);
        }
    } while (dim != 0);
    

    
    
    
    //Closing FILE
    fclose(fp);
    
    exit(EXIT_SUCCESS);
    
    
}
void matrix_printing(int **array, int rows_number, int columns_number)
{
    int i, j;
    for (i = 0; i < rows_number; ++i){
        for (j = 0; j < columns_number; ++j){
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}
int min(int a, int b)
{
    if (a >= b){
        return b;
    }
    else
        return a;
}
int sum_matrix_elements(int **array, int rows_number, int columns_number)
{
    int i, j, sum = 0;
    for (i = 0; i < rows_number; ++i){
        for (j = 0; j < columns_number; ++j){
            sum = sum + array[i][j];
        }
    }
    return sum;
}
