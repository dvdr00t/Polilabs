//
//  main.c
//  Laboratorio 0 - Es 3
//
//  Created by Davide Arcolini on 27/09/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

void right_rotation (int *array, int length, int p);
void left_rotation (int *array, int length, int p);
int main()
{
    int *array;
    int length;
    int i, p;
    
    //Getting length of the array
    printf("Set length of the array: ");
    scanf("%d", &length);
    
    //Dynamically creating the array
    array = (int*) malloc(length * sizeof(int));
    
    //Getting elements of the array
    printf("Insert elements in the array separated by tabspace.\n");
    for (i = 0; i < length; i++){
        scanf("%d", &array[i]);
    }
    
    //Executing rotation
    printf("Insert rotation value. Negative values are for left side rotation, positive values are for right side rotazione. Press 0 to end.\n\n");
    do {
        
        printf("Insert rotation value: ");
        scanf("%d", &p);
        
        //Case 1: end of rotation
        if (p == 0){
            exit(EXIT_SUCCESS);
        }
        
        //Case 2: negative value of p, left rotation
        else if (p < 0){
            left_rotation(array, length, p);
        }
        
        //Case 3: positive value of p, right rotation
        else {
            right_rotation(array, length, p);
        }

        
        //Printing the new array
        printf("[ ");
        for (i = 0; i < length; i++){
            printf("%d ", array[i]);
        }
        printf("]\n");
        
    } while (p != 0);
    
}

void left_rotation(int *array, int length, int p)       //Using temporary array to store blocks
{
    
    int *temp_array;
    int i, j;
    
    p = abs(p);
    temp_array = (int*) malloc(p * sizeof(int));
    
    for (i = 0; i < p; ++i){
        temp_array[i] = array[i];
    }
    
    for (i = p; i < length; ++i){
        array[i-p] = array[i];
    }
    
    for (i = length - p, j = 0; i < length; ++i){
        array[i] = temp_array[j];
        ++j;
    }
    
    
}

void  right_rotation(int *array, int length, int p)
{
    int *temp_array;
    int i, j;

    temp_array = (int*) malloc(p * sizeof(int));
    
    for (i = length - p, j = 0; i < length; ++i){
        temp_array[j] = array[i];
        ++j;
    }
    
    for (i = length - p - 1; i >= 0; --i){
        array[i+p] = array[i];
    }
    
    for (i = 0; i < p; ++i){
        array[i] = temp_array[i];
    }
}
