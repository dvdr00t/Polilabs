//
//  main.c
//  Laboratorio 0 - Es 2
//
//  Created by Davide Arcolini on 27/09/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH_MAX 20

int conta(char *string, int length);
int main(/*int argc, const char *argv[]*/)
{
 
    /*
    //Checking if the number of elements on the command line is correct
    if (argc != 2){
        printf("ERROR: incorrect number of elements!\n\n");
        exit(EXIT_FAILURE);
    }
     */
    
    FILE *fp;
    int length, N;
    int i;
    int sum = 0;
    char string[LENGTH_MAX];
    
    //Opening file
    fp = fopen("testo.txt", "r");
    if (fp == NULL){
        fprintf(stderr, "ERROR: FILE opening failed!\n\n");
        exit(EXIT_FAILURE);
    }
    
    //Getting length of sub_string
    printf("Set length of sub_string: ");
    scanf("%d", &length);
    
    //Reading number of rows and counting
    fscanf(fp, "%d", &N);
    for (i = 0; i < N; ++i){
        fscanf(fp, "%s", string);
        sum = sum + conta(string, length);
    }
    
    printf("Total amount of sub_string with two vocals of length %d is: %d.\n\n", length, sum);
    
    
}
int conta(char *string, int length)
{
    int i, j;
    int cont = 0;
    int result = 0;
    
    for (i = 0; i <= strlen(string) - length; ++i){
        for (j = i; j < i + length; ++j){
            if (string[j] == 'a' || string[j] == 'e' || string[j] == 'i' || string[j] == 'o' || string[j] == 'u'){
                ++cont;
            }
        }
        if (cont == 2){
            ++result;
        }
        cont = 0;
    }
    return result;
}
