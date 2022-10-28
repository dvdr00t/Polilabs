//
//  main.c
//  Lab.0 Es 1
//
//  Created by Davide Arcolini on 26/09/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define LENGTH_MAX 30

void erase_value(int *array, int length);
int main()
{
    int *array, *array_copy;
    int length;
    int i, j;
    int last_position_original = 0, last_position = 0;
    int counter, counter_max = 0;
    int position;
    int number_of_strings = 0;
    
    
    //Dynamic allocation of memory for the array
    do {
        printf("Set length for the array: ");
        scanf("%d", &length);
        
        if (length > LENGTH_MAX){
            printf("ERROR: max value for length: %d\n\n", LENGTH_MAX);
        }
    } while (length > LENGTH_MAX);
    
    
    //Creating the arrays
    
    array = (int*) malloc(length * sizeof(int));
    array_copy = (int*) malloc(length * sizeof(int));
    
    
    //Check if the memory has been successfully allocated by malloc or not
    if (array == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    else {
        // Memory has been successfully allocated
        printf("Memory successfully allocated using malloc.\n");
    }
    
    
    //Getting elements of the array
        
    for (i = 0; i < length; ++i){
        printf("Insert value n.[%d]: ", i+1);
        scanf("%d", &array[i]);
        
        array_copy[i] = 0;
    }
    
    //Analyzing the array
    
    for (i = 0; i < length; ++i){
        
        
        //Case 1: the current element is a non-null value
        if (array[i] != 0){
            
            counter = 0;
            for (position = i; array[position] != 0 && position != length - 1; ++position){
                ++counter;
            }
            
            //Case 1.1: the new string of elements is longer than the previous one
            if (counter > counter_max){
                erase_value(array_copy, length);
                number_of_strings = 0;
                last_position = 0;
                position = i;
                
                for (j = last_position; j < counter; ++j){
                    array_copy[j] = array[position];
                    ++position;
                }
                
                ++number_of_strings;
                counter_max = counter;
                last_position = last_position + counter;
                last_position_original = i + counter - 1;
            }
            
            //Case 1.2: the new string of elements is equal to the previous one
            else if (counter == counter_max){
                position = last_position_original + 1;
                for (j = last_position; j < last_position + counter; ++j){
                    array_copy[j] = array[position];
                    ++position;
                }
                ++number_of_strings;
                last_position_original = last_position_original + counter;
                last_position = last_position + counter;
            }
            
            //Case 1.3: the new string of elements is shorter than the previous one
            else if (counter < counter_max){
                last_position_original = last_position_original + 1;
            }
        }
        
        
        //Case 2: the current element is a null value
        else {
            if (i == 0){
                last_position_original = last_position_original;
            }
            else {
            last_position_original = last_position_original + 1;
            }
        }
        
        i = last_position_original;
    }
    
    
    //Printing the arrays
    for (i = 0; i < number_of_strings*counter_max; ++i){
        printf("%d ", array_copy[i]);
        if ((i+1) % counter_max == 0){
            printf("\n");
        }
    }
    
    
}

void erase_value(int *array, int length){
    int i;
    for (i = 0; i < length; ++i){
        array[i] = 0;
    }
}
