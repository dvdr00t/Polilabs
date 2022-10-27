//
//  main.c
//  Laboratorio 1 - Es 2
//
//  Created by Davide Arcolini on 06/10/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 200

char** saving_dictionary(int *code_n);
int my_strcmp (char *word, char **dictionary, int *dollar_numbers, FILE *fp, int position, int row, unsigned long size);
void file_check(FILE *fp);
int main (/*int argc, const char * argv[]*/)
{
    /*
    //Checking parameters from command line
    if (argc != 3){
        printf("ERROR: insert only program name, source file name and dictionary file name.\n\n");
        exit(EXIT_FAILURE);
    }
     */
    
    FILE *fp_text, *fp_dictionary, *fp_newtext;
    int s, size;
    int i, j;
    int *dollar_numbers;
    char **dictionary, word[MAX_LENGTH], word_tmp[MAX_LENGTH];
    
    //Opening dictionary file text and checking
    fp_dictionary = fopen("dizionario.txt", "r");
    file_check(fp_dictionary);
    
    //Dynamic allocation of memory for dictionary
    fscanf(fp_dictionary, "%d\n", &s);
    dictionary = (char**) malloc(s * sizeof(char*));
    dollar_numbers = (int*) malloc(s * sizeof(int));
    for (i = 0; i < s; i++){
        dictionary[i] = (char*) malloc(MAX_LENGTH * sizeof(char));
    }
    
    //Saving dictionary file text in memory
    i = 0;
    while (fgets(word_tmp, MAX_LENGTH, fp_dictionary) != NULL){
        sscanf(word_tmp, "%*c%d%*c %s", &dollar_numbers[i], dictionary[i]);
        ++i;
    }
    
    //Closing dictionary file text
    fclose(fp_dictionary);
    
    //Opening sorgente file text and checking
    fp_text = fopen("original_text.txt", "r");
    file_check(fp_text);
    fp_newtext = fopen("crypto_text.txt", "w");
    file_check(fp_newtext);
    
    //Reading word to word and comparing it with dictionary
    while ((fscanf(fp_text, "%s", word)) != EOF){
        
        size = (int)strlen(word);
        for (i = 0; i < size; ++i){
            for (j = 0; j < s; j++){
                if (word[i] == dictionary[j][0]){
                    if (my_strcmp(word, dictionary, dollar_numbers, fp_newtext, i, j, size) == 0){
                        i = i + (int)size;
                        break;
                    }
                }
            }
            fprintf(fp_newtext, "%c", word[i]);
        }
        fprintf(fp_newtext, " ");
    }
}
void file_check (FILE *fp){
    if (fp == NULL){
        printf("ERROR: file opening failed!\n\n");
        exit(EXIT_FAILURE);
    }
}
int my_strcmp (char *word, char **dictionary, int *dollar_numbers, FILE *fp, int position, int row, unsigned long size){
    int i;
    for (i = 0; i < strlen(dictionary[row]); i++){
        if (word[position] != dictionary[row][i]){
            return 1;
        }
        ++position;
    }
    fprintf(fp, "$%d$", dollar_numbers[row]);
    return 0;
}
